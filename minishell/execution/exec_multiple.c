/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execute.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmouafik <rmouafik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 12:37:38 by rmouafik          #+#    #+#             */
/*   Updated: 2025/07/09 11:34:03 by rmouafik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../minishell.h"

void execute_multiple(t_cmd *cmd, t_env **env_copy)
{
    int 	pipe_fd[2];
    int 	prev_fd = -1;
    int 	pid;
    char 	**env_arr;
    t_cmd 	*current = cmd;
    int		status = 0;
    int     last_pid = -1;

    env_arr = env_to_arr(*env_copy);
    while (current)
    {
        if (current->next != NULL)
        {
            if (pipe(pipe_fd) == -1)
            {
                perror("pipe");
                exit(1);
            }
        }
        pid = fork();
        if (pid == -1)
        {
            perror("fork");
            exit(1);
        }
        if (pid == 0)
        {
            ft_redirect(current);
            if (prev_fd != -1)
            {
                dup2(prev_fd, STDIN_FILENO);
                close(prev_fd);
            }
            if (current->next != NULL)
            {
                close(pipe_fd[0]);
                dup2(pipe_fd[1], STDOUT_FILENO);
                close(pipe_fd[1]);
            }
			if (is_builtin(current))
				cmd_built(current, env_copy, &status);
			else
            	child_process(current, env_arr, env_copy);
            exit(status);
        }
        last_pid = pid;
        if (prev_fd != -1)
            close(prev_fd);
        if (current->next != NULL)
        {
            close(pipe_fd[1]);
            prev_fd = pipe_fd[0];
        }
        else
            prev_fd = -1;
        current = current->next;
    }
    signal(SIGINT, SIG_IGN);
    waitpid(last_pid, &status, 0);
    while (wait(NULL) > 0);
	if (WIFEXITED(status))
		(*env_copy)->exit_status = WEXITSTATUS(status);
    else if (WIFSIGNALED(status))
	{
    	(*env_copy)->exit_status = 128 + WTERMSIG(status);
    	if (WTERMSIG(status) == SIGQUIT)
			ft_putendl_fd("Quit", 2);
	}
    free_args(env_arr);
    free_cmd_list(current);
	// printf("-->%d\n", last_status);
}

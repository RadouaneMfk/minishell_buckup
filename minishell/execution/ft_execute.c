/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execute.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmouafik <rmouafik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 12:37:38 by rmouafik          #+#    #+#             */
/*   Updated: 2025/09/21 13:16:41 by rmouafik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	cmd_built(t_cmd *cmd, t_env **env_copy, int *status)
{
	int	save_in;
	int	save_out;

	save_in = dup(STDIN_FILENO);
	save_out = dup(STDOUT_FILENO);
	if (ft_redirect_buil(cmd) == -1)
	{
		dup2(save_in, STDIN_FILENO);
		dup2(save_out, STDOUT_FILENO);
		close(save_in);
		close(save_out);
		return ;
	}
	*status = run_builtin(cmd, env_copy);
	dup2(save_in, STDIN_FILENO);
	dup2(save_out, STDOUT_FILENO);
	close(save_in);
	close(save_out);
	return ;
}

void	exec_helper(t_cmd *cmd, t_env **env_copy, int status, char **env_arr)
{
	cmd_built(cmd, env_copy, &status);
	(*env_copy)->exit_status = status;
	free_args(env_arr);
}

void	execute_one(t_cmd *cmd, t_env **env_copy)
{
	char	**env_arr;

	int (pid), (status);
	env_arr = env_to_arr(*env_copy);
	if (is_builtin(cmd))
	{
		exec_helper(cmd, env_copy, status, env_arr);
		return ;
	}
	pid = fork();
	if (pid == 0)
		child_process(cmd, env_arr, env_copy);
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	waitpid(pid, &status, 0);
	setup_signals();
	if (WIFEXITED(status))
		(*env_copy)->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		(*env_copy)->exit_status = 128 + WTERMSIG(status);
		if (WTERMSIG(status) == SIGQUIT)
			ft_putendl_fd("Quit", 2);
	}
	free_args(env_arr);
}

int	ft_execute(t_cmd *cmd, t_env **env_copy, char *input, t_token *res)
{
	t_cmd	*current;

	current = cmd;
	while (current)
	{
		if (ft_herdoc(current, env_copy, res) == -1)
			return (0);
		current = current->next;
	}
	if (cmd->next == NULL)
		execute_one(cmd, env_copy);
	else
		execute_multiple(cmd, env_copy);
	return (0);
}

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

#include "../minishell.h"

// void	execute_multiple(t_cmd *cmd, t_env **env_copy)
// {
// 	t_help	var;
// 	int		pipe_fd[2];
// 	char	**env_arr;
// 	t_cmd	*current;
// 	int		last_pid;

// 	var.prev_fd = -1;
// 	current = cmd;
// 	var.status = 0;
// 	last_pid = -1;
// 	signal(SIGINT, signal_handler);
// 	signal(SIGQUIT, SIG_IGN);
// 	env_arr = env_to_arr(*env_copy);
// 	while (current)
// 	{
// 		if (current->next != NULL)
// 		{
// 			if (pipe(pipe_fd) == -1)
// 			{
// 				perror("pipe");
// 				exit(1);
// 			}
// 		}
// 		var.pid = fork();
// 		if (var.pid == -1)
// 		{
// 			perror("fork");
// 			if (current->next != NULL)
// 			{
// 				close(pipe_fd[0]);
// 				close(pipe_fd[1]);
// 			}
// 			if (var.prev_fd != -1)
// 				close(var.prev_fd);
// 			(*env_copy)->exit_status = 1;
// 			return ;
// 		}
// 		if (var.pid == 0)
// 		{
// 			ft_redirect(current);
// 			if (var.prev_fd != -1)
// 			{
// 				dup2(var.prev_fd, STDIN_FILENO);
// 				close(var.prev_fd);
// 			}
// 			if (current->next != NULL)
// 			{
// 				close(pipe_fd[0]);
// 				dup2(pipe_fd[1], STDOUT_FILENO);
// 				close(pipe_fd[1]);
// 			}
// 			if (is_builtin(current))
// 				cmd_built(current, env_copy, &var.status);
// 			else
// 				child_process(current, env_arr, env_copy);
// 			exit(var.status);
// 		}
// 		last_pid = var.pid;
// 		if (var.prev_fd != -1)
// 			close(var.prev_fd);
// 		if (current->next != NULL)
// 		{
// 			close(pipe_fd[1]);
// 			var.prev_fd = pipe_fd[0];
// 		}
// 		else
// 			var.prev_fd = -1;
// 		current = current->next;
// 	}
// 	signal(SIGINT, SIG_IGN);
// 	waitpid(last_pid, &var.status, 0);
// 	while (wait(NULL) > 0)
// 		;
// 	if (WIFEXITED(var.status))
// 		(*env_copy)->exit_status = WEXITSTATUS(var.status);
// 	else if (WIFSIGNALED(var.status))
// 	{
// 		(*env_copy)->exit_status = 128 + WTERMSIG(var.status);
// 		if (WTERMSIG(var.status) == SIGQUIT)
// 			ft_putendl_fd("Quit", 2);
// 	}
// 	free_args(env_arr);
// 	free_cmd_list(current);
// }

static void	init_exec(t_help *var, t_cmd *cmd, char ***env_arr, t_env *env_copy)
{
	var->prev_fd = -1;
	var->status = 0;
	*env_arr = env_to_arr(env_copy);
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, SIG_IGN);
	(void)cmd;
}

void	exec_child(t_cmd *current, t_help *var, t_env **env_c, char **env_arr)
{
	ft_redirect(current);
	if (var->prev_fd != -1)
	{
		dup2(var->prev_fd, STDIN_FILENO);
		close(var->prev_fd);
	}
	if (current->next != NULL)
	{
		close(var->pipe_fd[0]);
		dup2(var->pipe_fd[1], STDOUT_FILENO);
		close(var->pipe_fd[1]);
	}
	if (is_builtin(current))
		cmd_built(current, env_c, &var->status);
	else
		child_process(current, env_arr, env_c);
	exit(var->status);
}

int	handle_fork(t_cmd *current, t_help *var, t_env **env_copy, char **env_arr)
{
	if (current->next != NULL && pipe(var->pipe_fd) == -1)
	{
		perror("pipe");
		exit(1);
	}
	var->pid = fork();
	if (var->pid == -1)
	{
		perror("fork");
		if (current->next != NULL)
		{
			close(var->pipe_fd[0]);
			close(var->pipe_fd[1]);
		}
		if (var->prev_fd != -1)
			close(var->prev_fd);
		(*env_copy)->exit_status = 1;
		return (-1);
	}
	if (var->pid == 0)
		exec_child(current, var, env_copy, env_arr);
	return (0);
}

void	finish_exec(t_help *var, int last_pid, t_env **env_copy, char **env_arr)
{
	signal(SIGINT, SIG_IGN);
	waitpid(last_pid, &var->status, 0);
	while (wait(NULL) > 0)
		;
	if (WIFEXITED(var->status))
		(*env_copy)->exit_status = WEXITSTATUS(var->status);
	else if (WIFSIGNALED(var->status))
	{
		(*env_copy)->exit_status = 128 + WTERMSIG(var->status);
		if (WTERMSIG(var->status) == SIGQUIT)
			ft_putendl_fd("Quit", 2);
	}
	free_args(env_arr);
}

void	execute_multiple(t_cmd *cmd, t_env **env_copy)
{
	t_help	var;
	char	**env_arr;
	t_cmd	*current;
	int		last_pid;

	init_exec(&var, cmd, &env_arr, *env_copy);
	current = cmd;
	last_pid = -1;
	while (current)
	{
		if (handle_fork(current, &var, env_copy, env_arr) == -1)
			return ;
		last_pid = var.pid;
		if (var.prev_fd != -1)
			close(var.prev_fd);
		if (current->next != NULL)
		{
			close(var.pipe_fd[1]);
			var.prev_fd = var.pipe_fd[0];
		}
		else
			var.prev_fd = -1;
		current = current->next;
	}
	finish_exec(&var, last_pid, env_copy, env_arr);
}

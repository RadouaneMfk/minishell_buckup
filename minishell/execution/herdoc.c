/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   herdoc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmouafik <rmouafik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 11:16:20 by rmouafik          #+#    #+#             */
/*   Updated: 2025/09/22 11:01:22 by rmouafik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// char	*expand_string_her(char *word, char **envp, t_env *env_head)
// {
// 	t_help	var;
// 	char	*var_name;
// 	char	*tmp;
// 	char	*res;
// 	char	*str;

// 	var.i = 0;
// 	var.start = 0;
// 	res = ft_strdup("");
// 	if (!word)
// 		return (ft_strdup(""));
// 	while (word[var.i])
// 	{
// 		if (word[var.i] == '$')
// 		{
// 			var.i++;
// 			if (word[var.i] == '?')
// 			{
// 				str = ft_itoa(env_head->exit_status);
// 				tmp = ft_strjoin(res, str);
// 				free(res);
// 				free(str);
// 				res = tmp;
// 				var.i++;
// 				continue ;
// 			}
// 			var.start = var.i;
// 			while (word[var.i] && is_valid_env_char(word[var.i]))
// 				var.i++;
// 			var_name = ft_substr(word, var.start, var.i - var.start);
// 			var.value = get_env_value_par(var_name, envp);
// 			free(var_name);
// 			if (var.value)
// 			{
// 				tmp = ft_strjoin(res, var.value);
// 				free(res);
// 				res = tmp;
// 			}
// 		}
// 		else
// 		{
// 			tmp = ft_strjoin_char(res, word[var.i]);
// 			free(res);
// 			res = tmp;
// 			var.i++;
// 		}
// 	}
// 	return (res);
// }

char	*expand_variable(char *word, t_help *var, char *res, t_env *env_head)
{
	char *(var_name), *(tmp), *(str), **(env_arr);
	env_arr = env_to_arr(env_head);
	if (word[var->i] == '?')
	{
		str = ft_itoa(env_head->exit_status);
		tmp = ft_strjoin(res, str);
		free(res);
		free(str);
		var->i++;
		return (tmp);
	}
	var->start = var->i;
	while (word[var->i] && is_valid_env_char(word[var->i]))
		var->i++;
	var_name = ft_substr(word, var->start, var->i - var->start);
	var->value = get_env_value_par(var_name, env_arr);
	free(var_name);
	if (var->value)
	{
		tmp = ft_strjoin(res, var->value);
		free(res);
		return (tmp);
	}
	return (res);
}

char	*expand_string_her(char *word, t_env *env_head)
{
	t_help	var;
	char	*res;
	char	*tmp;

	var.i = 0;
	res = ft_strdup("");
	if (!word)
		return (ft_strdup(""));
	while (word[var.i])
	{
		if (word[var.i] == '$')
		{
			var.i++;
			res = expand_variable(word, &var, res, env_head);
		}
		else
		{
			tmp = ft_strjoin_char(res, word[var.i]);
			free(res);
			res = tmp;
			var.i++;
		}
	}
	return (res);
}

void	run_herdoc(t_redriection *tmp, int fd, t_env **env_copy, t_token *res)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (!ft_strcmp(line, tmp->file_or_delim))
		{
			free(line);
			break ;
		}
		if (!res->quoted)
			line = expand_string_her(line, *env_copy);
		ft_putendl_fd(line, fd);
		free(line);
	}
}

int	h_chi(t_redriection *tmp, int heredoc_fd[2], t_env **env_copy, t_token *res)
{
	int	pid;
	int	status;

	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_IGN);
		close(heredoc_fd[0]);
		run_herdoc(tmp, heredoc_fd[1], env_copy, res);
		close(heredoc_fd[1]);
		exit(0);
	}
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	waitpid(pid, &status, 0);
	setup_signals();
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		(*env_copy)->exit_status = 1;
		write(1, "\n", 1);
		return (close(heredoc_fd[0]), close(heredoc_fd[1]), -1);
	}
	close(heredoc_fd[1]);
	return (heredoc_fd[0]);
}

int	ft_herdoc(t_cmd *cmd, t_env **env_copy, t_token *res)
{
	t_redriection	*tmp;
	int				heredoc_fd[2];

	tmp = cmd->red;
	while (tmp)
	{
		if (tmp->type == HEREDOC)
		{
			if (pipe(heredoc_fd) == -1)
			{
				perror("pipe");
				exit(1);
			}
			cmd->herdoc_fd = h_chi(tmp, heredoc_fd, env_copy, res);
			if (cmd->herdoc_fd == -1)
				return (-1);
		}
		tmp = tmp->next;
	}
	return (0);
}

// int	ft_herdoc(t_cmd *cmd, t_env **env_copy, t_token *res)
// {
// 	t_redriection	*tmp;
// 	int				pid;
// 	int				heredoc_fd[2];
// 	int				status;

// 	tmp = cmd->red;
// 	while (tmp)
// 	{
// 		if (tmp->type == HEREDOC)
// 		{
// 			if (pipe(heredoc_fd) == -1)
// 			{
// 				perror("pipe");
// 				exit(1);
// 			}
// 			pid = fork();
// 			if (pid == 0)
// 			{
// 				signal(SIGINT, SIG_DFL);
// 				signal(SIGQUIT, SIG_IGN);
// 				close(heredoc_fd[0]);
// 				run_herdoc(tmp, heredoc_fd[1], env_copy, res);
// 				close(heredoc_fd[1]);
// 				exit(0);
// 			}
// 			signal(SIGINT, SIG_IGN);
// 			signal(SIGQUIT, SIG_IGN);
// 			waitpid(pid, &status, 0);
// 			setup_signals();
// 			if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
// 			{
// 				(*env_copy)->exit_status = 130;
// 				write(1, "\n", 1);
// 				close(heredoc_fd[0]);
// 				close(heredoc_fd[1]);
// 				return (-1);
// 			}
// 			close(heredoc_fd[1]);
// 			cmd->herdoc_fd = heredoc_fd[0];
// 		}
// 		tmp = tmp->next;
// 	}
// 	return (0);
// }

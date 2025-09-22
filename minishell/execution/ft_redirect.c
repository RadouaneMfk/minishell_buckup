/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_redirect.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmouafik <rmouafik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 09:54:52 by rmouafik          #+#    #+#             */
/*   Updated: 2025/09/21 09:52:59 by rmouafik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	in_redirect(t_cmd *cmd)
{
	int				fd;
	t_redriection	*tmp;

	tmp = cmd->red;
	while (tmp)
	{
		if (tmp->type == HEREDOC)
		{
			dup2(cmd->herdoc_fd, STDIN_FILENO);
			close(cmd->herdoc_fd);
		}
		if (tmp->type == REDIR_IN)
		{
			fd = open(tmp->file_or_delim, O_RDONLY, 0777);
			if (fd < 0)
				return (perror("infile error!"), exit(1));
			if (dup2(fd, STDIN_FILENO) == -1)
			{
				perror("dup failed!");
				exit(1);
			}
			close(fd);
		}
		tmp = tmp->next;
	}
}

void	open_append(int *fd, t_redriection **tmp)
{
	*fd = open((*tmp)->file_or_delim, O_CREAT | O_RDWR | O_APPEND, 0777);
}

void	out_redirect(t_cmd *cmd)
{
	int				fd;
	t_redriection	*tmp;

	tmp = cmd->red;
	while (tmp)
	{
		if (tmp->type == APPEND || tmp->type == REDIR_OUT)
		{
			if (tmp->type == APPEND)
				open_append(&fd, &tmp);
			else
				fd = open(tmp->file_or_delim, O_CREAT | O_RDWR | O_TRUNC, 0777);
			if (fd < 0)
				return (perror("outfile error!"), exit(1));
			if ((tmp->next == NULL) 
				|| (tmp->next && tmp->next->type != APPEND 
					&& tmp->next->type != REDIR_OUT))
			{
				if (dup2(fd, STDOUT_FILENO) == -1)
					return (perror("dup failed!"), exit(1));
				close(fd);
			}
		}
		tmp = tmp->next;
	}
}

void	ft_redirect(t_cmd *cmd)
{
	in_redirect(cmd);
	out_redirect(cmd);
}

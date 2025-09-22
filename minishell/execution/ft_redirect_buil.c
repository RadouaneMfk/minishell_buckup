/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_redirect_buil.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmouafik <rmouafik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 09:54:12 by rmouafik          #+#    #+#             */
/*   Updated: 2025/09/21 10:34:40 by rmouafik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	in_redirect_buil(t_cmd *cmd)
{
	int				fd;
	t_redriection	*tmp;

	tmp = cmd->red;
	while (tmp)
	{
		if (tmp->type == REDIR_IN)
		{
			fd = open(tmp->file_or_delim, O_RDONLY, 0777);
			if (fd < 0)
				return (perror("infile error!"), -1);
			if (dup2(fd, STDIN_FILENO) == -1)
			{
				perror("dup failed!");
				return (-1);
			}
			close(fd);
		}
		tmp = tmp->next;
	}
	return (1);
}

int	ft_dup_failed(int *fd)
{
	if (dup2(*fd, STDOUT_FILENO) == -1)
		return (perror("dup failed!"), -1);
	return (0);
}

int	out_redirect_buil(t_cmd *cmd)
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
				return (perror("outfile error"), -1);
			if ((tmp->next == NULL) 
				|| (tmp->next && tmp->next->type != APPEND 
					&& tmp->next->type != REDIR_OUT))
			{
				ft_dup_failed(&fd);
				close(fd);
			}
		}
		tmp = tmp->next;
	}
	return (1);
}

int	ft_redirect_buil(t_cmd *cmd)
{
	if (in_redirect_buil(cmd) == -1)
		return (-1);
	if (out_redirect_buil(cmd) == -1)
		return (-1);
	return (0);
}

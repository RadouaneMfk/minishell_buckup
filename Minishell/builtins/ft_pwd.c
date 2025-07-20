/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmouafik <rmouafik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 11:03:47 by rmouafik          #+#    #+#             */
/*   Updated: 2025/06/26 10:34:32 by rmouafik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_pwd(t_env *env_copy)
{
	char	*path;

	(void)env_copy;
	path = getcwd(NULL, 0);
	printf("%s\n", path);
	free(path);
	return (0);
}

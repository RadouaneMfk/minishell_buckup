/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmouafik <rmouafik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 11:02:51 by rmouafik          #+#    #+#             */
/*   Updated: 2025/07/02 10:53:20 by rmouafik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_echo(char **arr, t_env *env_copy)
{
	int	i;

	i = 1;
	while (arr[i])
	{
		if (i == 1 && !ft_strcmp(arr[i], "-n"))
			i++;
		if (arr[i])
			ft_putstr_fd(arr[i], 1);
		if (arr[i + 1] != NULL)
			write(1, " ", 1);
		i++;
	}
	if (ft_strcmp(arr[1], "-n"))
		write(1, "\n", 1);
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmouafik <rmouafik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 11:02:49 by rmouafik          #+#    #+#             */
/*   Updated: 2025/09/16 11:27:36 by rmouafik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_digit(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[0] == '-')
			i++;
		if (!ft_isdigit(str[i]))
			return (1);
		i++;
	}
	return (0);
}

int	check_num(char **arr)
{
	int	i;
	int	j;

	i = 1;
	if (check_digit(arr[1]))
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(arr[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		exit(255);
	}
	while (arr[i])
	{
		j = 0;
		while (ft_isdigit(arr[i][j]))
			j++;
		i++;
	}
	return (i - 1);
}

void	print_error_exit(char *str)
{
	ft_putstr_fd("minishell: exit: ", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd(": numeric argument required\n", 2);
}

void	print_err_free(char *str, t_cmd *cmd, t_env **env_copy)
{
	ft_putstr_fd("minishell: exit: ", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd(": numeric argument required\n", 2);
	free_env(*env_copy);
	free_cmd_list(cmd);
	exit(5);
}

int	ft_exit(char **arr, t_env **env_copy, t_cmd *cmd)
{
	int	ex_status;

	printf("exit\n");
	if (arr[1] == NULL)
	{
		ex_status = (*env_copy)->exit_status;
		free_env(*env_copy);
		free_cmd_list(cmd);
		exit(ex_status);
	}
	if (check_num(arr) != 1)
		return (ft_putstr_fd(ERROR_ARG, 2), 1);
	else if (arr[1] != NULL)
	{
		if (check_digit(arr[1]))
			print_err_free(arr[1], cmd, env_copy);
		if (!check_long(arr[1]))
			print_error_exit(arr[1]);
		free_env(*env_copy);
		exit(ft_atoi(arr[1]));
	}
	return (0);
}

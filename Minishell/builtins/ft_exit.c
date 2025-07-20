/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmouafik <rmouafik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 11:02:49 by rmouafik          #+#    #+#             */
/*   Updated: 2025/07/02 10:59:38 by rmouafik         ###   ########.fr       */
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

int	check_long(const char *str)
{
	unsigned long long	result;
	unsigned long long	limit;
	int					sign;
	int					i;

	result = 0;
	sign = 1;
	i = 0;
	while ((str[i] == 32) || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign *= -1;
		i++;
	}
	if (sign < 0)
		limit = LLONG_MIN;
	else
		limit = LLONG_MAX;
	while (ft_isdigit(str[i]))
	{
		if (result > (limit - (str[i] - '0')) / 10)
			return (0);
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (1);
}

int	ft_exit(char **arr, t_env **env_copy)
{
	printf("exit\n");
	if (check_num(arr) != 1)
		return (ft_putstr_fd(ERROR_ARG, 2), 1);
	if (arr[1] == NULL)
		exit(0); // i will update this to be the last status (signals)
	else if (arr[1] != NULL)
	{
		if (check_digit(arr[1]))
		{
			ft_putstr_fd("minishell: exit: ", 2);
			ft_putstr_fd(arr[1], 2);
			ft_putstr_fd(": numeric argument required\n", 2);
			exit(255);
		}
		if (!check_long(arr[1]))
		{
			ft_putstr_fd("minishell: exit: ", 2);
			ft_putstr_fd(arr[1], 2);
			ft_putstr_fd(": numeric argument required\n", 2);
		}
		exit(ft_atoi(arr[1]));
	}
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmouafik <rmouafik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 13:09:38 by rmouafik          #+#    #+#             */
/*   Updated: 2025/09/22 12:01:58 by rmouafik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	print_error(char *str)
{
	ft_putstr_fd("minishell: export: `", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
}

void	key_value_alloc(t_help var, char *str)
{
	free(var.key);
	var.ptr_value = ft_strchr(str, '=');
	var.value = var.ptr_value + 1;
	var.pos = var.ptr_value - str;
	var.key = ft_substr(str, 0, var.pos);
}

int	check_exp(char *str, t_env **env_copy)
{
	if (str && str[0] == '\0')
		return (print_error(str), 1);
	if (str == NULL)
		print_export(env_copy);
	return (0);
}

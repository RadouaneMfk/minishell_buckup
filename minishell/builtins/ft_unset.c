/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmouafik <rmouafik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 10:37:35 by rmouafik          #+#    #+#             */
/*   Updated: 2025/09/15 11:15:14 by rmouafik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	remove_env(char *key, t_env	**env_copy)
{
	t_env (*current), 
	(*previous);
	current = *env_copy;
	previous = NULL;
	while (current)
	{
		if (!ft_strcmp(current->key, key))
		{
			if (previous == NULL)
			{
				*env_copy = current->next;
				free(current->key);
				free(current->value);
				free(current);
				return ;
			}
			previous->next = current->next;
			free(current->key);
			free(current->value);
			free(current);
			return ;
		}
		previous = current;
		current = current->next;
	}
}

int	check_valid(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (i == 0 && (!ft_isalpha(str[i]) && str[i] != '_'))
			return (1);
		else if (i != 0 && (!ft_isalnum(str[i]) && str[i] != '_'))
			return (1);
		i++;
	}
	return (0);
}

int	ft_unset(char **arr, t_env **env_copy)
{
	int	i;

	i = 1;
	while (arr[i])
	{
		if (check_valid(arr[i]) || arr[i] == NULL)
		{
			ft_putstr_fd("minishell: unset: `", 2);
			ft_putstr_fd(arr[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			return (1);
		}
		remove_env(arr[i], env_copy);
		i++;
	}
	return (0);
}

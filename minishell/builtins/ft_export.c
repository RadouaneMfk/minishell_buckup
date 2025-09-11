/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmouafik <rmouafik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 11:03:15 by rmouafik          #+#    #+#             */
/*   Updated: 2025/09/11 13:08:58 by rmouafik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_contain_equal(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '=')
			return (1);
		i++;
	}
	return (0);
}

void	add_update_env(t_env **env_copy, char *key, char *value)
{
	t_env	*head;
	t_env	*new_node;

	head = *env_copy;
	while (head)
	{
		if (!ft_strcmp(head->key, key))
		{
			free(head->value);
			head->value = ft_strdup(value);
			return ;
		}
		head = head->next;
	}
	new_node = *env_copy;
	new_node = malloc(sizeof(t_env));
	new_node->key = ft_strdup(key);
	new_node->value = ft_strdup(value);
	new_node->next = NULL;
	env_add_back(env_copy, new_node);
}

void	print_export(t_env **env_copy)
{
	t_env	*head;

	head = *env_copy;
	while (head)
	{
		printf("declare -x ");
		if (head->value == NULL)
			printf("%s\n", head->key);
		else
		{
			printf("%s=", head->key);
			printf("\"%s\"\n", head->value);
		}
		head = head->next;
	}
}

int	check_args(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (i == 0 && (!ft_isalpha(str[i]) && str[i] != '_'))
			return (1);
		else if (i != 0 
			&& (!ft_isalnum(str[i]) && str[i] != '_' && str[i] != '='))
			return (1);
		i++;
	}
	return (0);
}

int	ft_export(char **arr, t_env **env_copy)
{
	char	*value;
	char	*ptr_value;
	char	*key;
	int		pos;
	int		i;

	i = 1;
	if (arr[1] == NULL)
		print_export(env_copy);
	while (arr[i])
	{
		if (check_args(arr[i]) || arr[i] == NULL)
		{
			ft_putstr_fd("minishell: export: `", 2);
			ft_putstr_fd(arr[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			return (1);
		}
		if (is_contain_equal(arr[i]))
		{
			ptr_value = ft_strchr(arr[i], '=');
			value = ptr_value + 1;
			pos = ptr_value - arr[i];
			key = ft_substr(arr[i], 0, pos);
		}
		else
		{
			key = ft_strdup(arr[i]);
			value = ft_strdup(get_env_value(env_copy, key));
		}
		add_update_env(env_copy, key, value);
		free(key);
		i++;
	}
	return (0);
}

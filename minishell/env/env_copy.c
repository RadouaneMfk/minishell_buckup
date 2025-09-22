/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_copy.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmouafik <rmouafik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 10:52:52 by rmouafik          #+#    #+#             */
/*   Updated: 2025/09/18 09:59:14 by rmouafik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_env	*ft_lstlast_env(t_env *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next != NULL)
		lst = lst->next;
	return (lst);
}

void	env_add_back(t_env **env_list, t_env *new_node)
{
	t_env	*tmp;

	if (!env_list || !new_node)
		return ;
	if (*env_list == NULL)
		*env_list = new_node;
	else
	{
		tmp = ft_lstlast_env(*env_list);
		tmp->next = new_node;
	}
}

void	empty_env(t_env *env_node, t_env **env_head)
{
	char	*path;

	env_node = malloc(sizeof(t_env));
	path = getcwd(NULL, 0);
	env_node->key = ft_strdup("PWD");
	env_node->value = ft_strdup(path);
	env_node->next = NULL;
	env_add_back(env_head, env_node);
	env_node = malloc(sizeof(t_env));
	env_node->key = ft_strdup("SHLVL");
	env_node->value = ft_strdup("1");
	env_node->next = NULL;
	env_add_back(env_head, env_node);
	env_node = malloc(sizeof(t_env));
	env_node->key = ft_strdup("_");
	env_node->value = ft_strdup("./minishell");
	env_node->next = NULL;
	env_add_back(env_head, env_node);
}

void	env_copy(char **envp, t_env	**env_head)
{
	int		i;
	int		pos;
	t_env	*env_node;
	char	*ptr_value;

	i = 0;
	*env_head = NULL;
	if (!envp || !*envp)
		empty_env(env_node, env_head);
	while (envp[i])
	{
		env_node = malloc(sizeof(t_env));
		ptr_value = ft_strchr(envp[i], '=');
		pos = ptr_value - envp[i];
		env_node->key = ft_substr(envp[i], 0, pos);
		env_node->value = ft_strdup(ptr_value + 1);
		env_node->next = NULL;
		env_add_back(env_head, env_node);
		i++;
	}
}

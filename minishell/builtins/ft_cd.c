/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmouafik <rmouafik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 11:02:54 by rmouafik          #+#    #+#             */
/*   Updated: 2025/06/26 10:23:36 by rmouafik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_env_value(t_env **env_copy, char *key)
{
	t_env	*head;

	head = *env_copy;
	while (head)
	{
		if (!ft_strcmp(head->key, key))
			return (head->value);
		head = head->next;
	}
	return (NULL);
}

void	update_env(t_env **env_copy, char *key, char *value)
{
	t_env	*head;

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
}

void	add_env(t_env **env_copy, char *key, char *value)
{
	t_env	*new_node;

	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return ;
	new_node->key = ft_strdup(key);
	new_node->value = ft_strdup(value);
	new_node->next = NULL;
	env_add_back(env_copy, new_node);
}

int	ft_cd(char *path, t_env **env_copy)
{
	char	*old_path;
	char	*pwd;
	char	*home;

	pwd = getcwd(NULL, 0);
	home = get_env_value(env_copy, "HOME");
	if (path == NULL || !ft_strcmp(path, "~"))
	{
		if (get_env_value(env_copy, "HOME") == NULL)
		{
			printf("minishell: cd: HOME not set\n");
			free(pwd);
			return (1);
		}
		chdir(home);
		return (free(pwd), 0);
	}
	if (!ft_strcmp(path, "-"))
	{
		if (get_env_value(env_copy, "OLDPWD") == NULL)
		{
			printf("minishell: cd: OLDPWD not set\n");
			free(pwd);
			return (1);
		}
		old_path = get_env_value(env_copy, "OLDPWD");
		free(pwd);
		pwd = getcwd(NULL, 0);
		if (chdir(old_path) == 0)
			printf("%s\n", old_path);
	}
	if (chdir(path) != 0 && ft_strcmp(path, "-") 
		&& ft_strcmp(path, "~") && path != NULL)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		free(pwd);
		return (1);
	}
	if (get_env_value(env_copy, "OLDPWD") == NULL)
		add_env(env_copy, "OLDPWD", pwd);
	if (get_env_value(env_copy, "OLDPWD") != NULL)
		update_env(env_copy, "OLDPWD", pwd);
	char *cwd = getcwd(NULL, 0);
	if (get_env_value(env_copy, "PWD") != NULL)
		update_env(env_copy, "PWD", cwd);
	free(pwd);
	free(cwd);
	return (0);
}

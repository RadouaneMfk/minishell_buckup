/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shelvl.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmouafik <rmouafik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 11:04:55 by rmouafik          #+#    #+#             */
/*   Updated: 2025/09/10 11:08:20 by rmouafik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../minishell.h"

void	ft_update_shelvl(t_env *env_list)
{
	while (env_list)
	{
		if (ft_strncmp(env_list->key, "SHLVL", 6) == 0)
		{
			free(env_list->value);
			int lvl = ft_atoi(env_list->value);
			lvl++;
			env_list->value = ft_itoa(lvl); 
		}
		env_list = env_list->next;
	}
}

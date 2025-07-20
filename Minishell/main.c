/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmouafik <rmouafik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 10:35:46 by rmouafik          #+#    #+#             */
/*   Updated: 2025/07/09 11:34:43 by rmouafik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	if (!s1 || !s2)
		return 1;
	while (s1[i] && s2[i])
	{
		if ((unsigned char)s1[i] != (unsigned char)s2[i])
			return (1);
		i++;
	}
	return (s1[i] - s2[i]);
}

int	is_builtin(t_cmd *cmd)
{
	if (!ft_strcmp(cmd->args[0], "env") || !ft_strcmp(cmd->args[0], "ENV"))
		return (1);
	if (!ft_strcmp(cmd->args[0], "pwd") || !ft_strcmp(cmd->args[0], "PWD"))
		return (1);
	if (!ft_strcmp(cmd->args[0], "cd"))
		return (1);
	if (!ft_strcmp(cmd->args[0], "echo") || !ft_strcmp(cmd->args[0], "ECHO"))
		return (1);
	if (!ft_strcmp(cmd->args[0], "unset"))
		return (1);
	if (!ft_strcmp(cmd->args[0], "exit"))
		return (1);
	if (!ft_strcmp(cmd->args[0], "export"))
		return (1);
	return 0;
}
void	run_builtin(t_cmd *cmd, t_env **env_copy)
{
	if (!ft_strcmp(cmd->args[0], "env") || !ft_strcmp(cmd->args[0], "ENV"))
		ft_env(*env_copy);
	if (!ft_strcmp(cmd->args[0], "pwd") || !ft_strcmp(cmd->args[0], "PWD"))
		ft_pwd(*env_copy);
	if (!ft_strcmp(cmd->args[0], "cd"))
		ft_cd(cmd->args[1], env_copy);
	if (!ft_strcmp(cmd->args[0], "echo") || !ft_strcmp(cmd->args[0], "ECHO"))
		ft_echo(cmd->args, *env_copy);
	if (!ft_strcmp(cmd->args[0], "unset"))
		ft_unset(cmd->args, env_copy);
	if (!ft_strcmp(cmd->args[0], "exit"))
		ft_exit(cmd->args, env_copy);
	if (!ft_strcmp(cmd->args[0], "export"))
		ft_export(cmd->args, env_copy);
}

char **env_to_arr(t_env *env_head)
{
	char	**env_arr;

	int	i = 0;
	env_arr = malloc(sizeof(char **) * 1337);
	while (env_head)
	{
		char *tmp = ft_strjoin(env_head->key, "=");
		env_arr[i] = ft_strjoin(tmp, env_head->value);
		free(tmp);
		i++;
		env_head = env_head->next;
	}
	return (env_arr);
}

int main(int ac, char *av[], char **envp)
{
    char 	*input;
	char	**env_arr;
	t_token *res;
    t_cmd	*cmd;
    t_env	*env_head;

	env_copy(envp, &env_head);
	ft_update_shelvl(env_head);
    while(1)
    {
		char *pwd = getcwd(NULL, 0);
        input = readline(ft_strjoin(pwd, "$> "));
        if(input == NULL)
            break ;
        if(*input)
            add_history(input);
		res = tokenize(input);
        cmd = parse_cmd(res);
		env_arr = env_to_arr(env_head);
		// print_cmd(cmd);
		if (cmd->args[0])
			ft_execute(cmd, &env_head, input);
    }
}

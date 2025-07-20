/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmouafik <rmouafik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 09:30:31 by haboucha          #+#    #+#             */
/*   Updated: 2025/07/09 11:35:03 by rmouafik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "lib/libft.h"
# include <unistd.h>
# include <limits.h>
# include <fcntl.h>
# include <sys/wait.h>

// ------------ parsing -----------------

typedef enum e_type {
    WORD,
    PIPE,
    REDIR_IN,
    REDIR_OUT,
    APPEND,
    HEREDOC,
} t_type;

typedef struct s_token
{
    char *value;
    t_type type;
    struct s_token *next;
} t_token;

typedef struct s_redriection
{
    t_type type;
    char *file_or_delim;
    struct s_redriection *next;
} t_redriection;

typedef struct s_cmd
{
    char *cmd;
    char **args;
    t_redriection *red;
    int herdoc_fd;
    struct s_cmd *next;
} t_cmd;

t_token *cretae_token(char *value, t_type type);
void append_token(t_token **head,t_token *new);
int handle_APPEND(char *input,int i,t_token **head);
int handle_herdoc(char *input,int i,t_token **head);
int handle_outfile(char *input,int i,t_token **head);
int handle_intfile(char *input,int i,t_token **head);
int handle_pipe(char *input,int i,t_token **head);
int handle_word(char *input, int i, t_token **head);
t_token *tokenize(char *input);
t_cmd *parse_cmd(t_token *token);
void print_cmd(t_cmd *cmd);

// -------------- builtins ----------------

typedef struct s_env
{
	char	*key;
	char	*value;
    int     status;
	struct s_env *next;
} t_env;

void	env_add_back(t_env **env_list, t_env *new_node);
void	env_copy(char **envp, t_env	**env_head);
void	ft_update_shelvl(t_env *env_list);
int 	ft_env(t_env *env_copy);
int		ft_pwd(t_env *env_copy);
int		ft_cd(char *path, t_env **env_copy);
int		ft_strcmp(char *s1, char *s2);
int		ft_echo(char **arr, t_env *env_copy);
int		ft_unset(char **arr, t_env **env_copy);
int 	ft_exit(char **arr, t_env **env_copy);
int		ft_export(char **arr, t_env **env_copy);
char	*get_env_value(t_env **env_copy, char *key);
int     is_builtin(t_cmd *cmd);
void	run_builtin(t_cmd *cmd, t_env **env_copy);
int     ft_execute(t_cmd *cmd, t_env **env_copy, char *input);
char    **env_to_arr(t_env *env_head);

#define ERROR_ARG "minishell: exit: too many arguments\n"

#endif
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmouafik <rmouafik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 09:30:31 by haboucha          #+#    #+#             */
/*   Updated: 2025/09/11 12:43:04 by rmouafik         ###   ########.fr       */
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
# include <signal.h>
# include <termios.h>

#define RED "\033[1;31m"
#define RESET "\033[0m"
#define GREEN "\001\033[0;32m\002"
extern int g_signal;
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
    char new_quote;
    struct s_token *next;
}t_token;

typedef struct s_redriection
{
    t_type type;
    char *file_or_delim;
    struct s_redriection *next;
} t_redriection;

typedef struct s_cmd
{
    char	*cmd;
    char	**args;
    int		pipe_count;
    t_redriection *red;
    int herdoc_fd;
    struct s_cmd *next;
} t_cmd;

// t_token *cretae_token(char *value, t_type type);
// void append_token(t_token **head,t_token *new);
// int handle_APPEND(char *input,int i,t_token **head);
// int handle_herdoc(char *input,int i,t_token **head);
// int handle_outfile(char *input,int i,t_token **head);
// int handle_intfile(char *input,int i,t_token **head);
// int handle_pipe(char *input,int i,t_token **head);
// int handle_word(char *input, int i, t_token **head);
// t_token *tokenize(char *input);
// t_cmd *parse_cmd(t_token *token);
// void print_cmd(t_cmd *cmd);
// int ft_isspace(int c);

int check_quotes(char *input);
int check_pipe_syntaxe(char *input);
int check_redirection_syntaxe(char *input);
int check_all_syntaxe(char *input);

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
void free_cmd_list(t_cmd *cmd);
void free_token_list(t_token *token);


int ft_isspace(int c);

typedef struct s_env
{
    char	*key;
    char	*value;
    int     status;
    int     exit_status;
    struct s_env *next;
} t_env;



void expand_token_list(t_token **head,char **envp, t_env *env_head);
char *expand_string(char *word,char **envp, t_env *env_head);
char *get_env_value_par(char *var,char **envp);
int ft_stncmp(char *s1,char *s2,int n);
int is_valid_env_char(char c);


// -------------- builtins ----------------


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
int     run_builtin(t_cmd *cmd, t_env **env_copy);
int     ft_execute(t_cmd *cmd, t_env **env_copy, char *input);
char    **env_to_arr(t_env *env_head);
void    execute_multiple(t_cmd *cmd, t_env **env_copy);
void    child_process(t_cmd *cmd, char **env_arr, t_env **env_copy);
int     ft_herdoc(t_cmd *cmd, t_env **env_copy);
void    cmd_built(t_cmd *cmd, t_env **env_copy, int *status);
void    setup_signals(void);
void    handle_end(t_env *env);
void    signal_handler(int sig);
void    setup_signals(void);
void	free_args(char **args);
void    ft_redirect(t_cmd *cmd);
void    free_env(t_env *head);

#define ERROR_ARG "minishell: exit: too many arguments\n"

#endif
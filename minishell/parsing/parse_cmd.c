/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haboucha <haboucha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 12:50:48 by haboucha          #+#    #+#             */
/*   Updated: 2025/07/07 13:27:59 by haboucha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void initilisation(t_cmd *cmd)
{
    cmd->cmd = NULL;
    cmd->args = NULL;
    // cmd->append = 0;
    cmd->red = NULL;
    // heredoc->delimiter = NULL;
    // heredoc->quoted =0;
    
}

int count_word_in_token(t_token *token)
{
    int count = 0;
    while(token && token->type != PIPE)
    {
        if(token->type == WORD)
        {
            count++;
        }
        token = token->next;
    }
    return(count);  
}
int count_redirect_in_token(t_token *token)
{
    int count = 0;
    while(token && token->type != PIPE)
    {
        if(token->type == REDIR_OUT)
            count++;
        token = token->next;
    }
    return (count);
}
int count_heredoc_in_token(t_token *token)
{
    int count = 0;
    while(token && token->type != PIPE)
    {
        if(token->type == HEREDOC)
            count++;
        token = token->next;
    }
    return(count);
}

void print_cmd(t_cmd *cmd)
{
    int i;
    // int j;
    while(cmd)
    {
        i = 0;
        printf("cmd: %s\n",cmd->cmd);
        if(cmd->args)
        {
            while(cmd->args[i])
            {
                printf("args[%d]: %s\n",i,cmd->args[i]);
                i++;
                
            }
        }
        t_redriection *red = cmd->red;
        while(red)
        {
            printf("redirection: type = %d file=%s\n",red->type,red->file_or_delim);
            red = red->next;
        }
        cmd = cmd->next;
    }
}

t_cmd *add_back_cmd(t_cmd **cmd,t_cmd *new_cmd)
{
    t_cmd *tmp;
    if(!cmd || !new_cmd)
        return NULL;
    if(!*cmd)
        *cmd = new_cmd;
    else
    {
        tmp = *cmd;
        while(tmp->next)
            tmp= tmp->next;
        tmp->next = new_cmd;

    }
    return(new_cmd);
}
int count_pipe(t_token *token)
{
    int i = 0;
    while(token)
    {
        if(token->type == PIPE)
            i++;
        token = token->next;
    }
    return i;
}

char *remove_quotes(char *str)
{
    int i = 0;
    int j  =0;
    int len = ft_strlen(str);
    char *res = malloc(len +1);
    if(!res)
        return NULL;
    while(str[i])
    {
        if(str[i] != '\'' && str[i] != '"')
            res[j++] = str[i];
        i++;
    }
    res[j] = '\0';
    return res;
}

t_redriection *new_red(t_type type,char *file)
{
    t_redriection *red = malloc(sizeof(t_redriection));
    if(!red)
        return(NULL);
    red->file_or_delim = ft_strdup(file);
    red->type = type;
    red->next = NULL;
    return red;
}

void add_red(t_type type, t_cmd *cmd,char *file)
{
    t_redriection *red = new_red(type,file);
    if(!cmd->red)
        cmd->red = red;
    else
    {
        t_redriection *tmp =cmd->red;
        while(tmp->next)
            tmp = tmp->next;
        tmp->next = red;
    }
}


t_cmd *new_cmd(t_token *token)
{
    t_cmd *cmd;
    t_redriection *red;
    cmd = malloc(sizeof(t_cmd));
    if(!cmd)
        return NULL;
    initilisation(cmd);
    red = malloc(sizeof(t_redriection));
    if(!red)
        return(free(cmd),NULL);
    int nbr_args = count_word_in_token(token);
    //int nbr_red = count_redirect_in_token(token);
   // int nbr_here= count_heredoc_in_token(token);
    cmd->args = malloc(sizeof(char *) * (nbr_args + 2));
    if(!cmd->args)
        return(free(cmd),NULL);
    int i= 0;
    while(token && token->type != PIPE)
    {
        if(token->type == WORD)
        {
            cmd->args[i]= ft_strdup(token->value);
            if(!cmd->cmd)
                cmd->cmd=ft_strdup(token->value);
            i++;
        }
        else if(token->type == REDIR_OUT || token->type == APPEND || token->type == REDIR_IN 
                || token->type == HEREDOC)
        {
            if(token->next)
                add_red(token->type,cmd,token->next->value);
            token=token->next;
        }
        token = token->next;
    }
    cmd->args[i] = NULL;
    cmd->next =NULL;
    return(cmd);
}

t_cmd *parse_cmd(t_token *token)
{
    t_cmd *head = NULL;
    t_cmd *cmd;
    
    while(token)
    {
        cmd = new_cmd(token);
        add_back_cmd(&head,cmd);
        while(token && token->type != PIPE)
            token = token->next;
        if(token && token->type == PIPE)
            token = token->next;
    }
    return(head);
}
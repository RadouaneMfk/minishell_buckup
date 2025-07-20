/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmouafik <rmouafik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 12:38:01 by haboucha          #+#    #+#             */
/*   Updated: 2025/07/09 11:34:06 by rmouafik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int ft_isspace(int c)
{
    return(c == 32  || (c >= 9 && c <= 13));
}

t_token *cretae_token(char *value, t_type type)
{
    t_token *token = malloc(sizeof(t_token));
    if(!token)
        return NULL;
    token->value = ft_strdup(value);
    token->type = type;
    token->next = NULL;
    return (token);
}

void append_token(t_token **head,t_token *new)
{
    if(!*head)
    {
        *head = new;
    }
    else
    {
        t_token *tmp = *head;
        while(tmp->next)
            tmp=tmp->next;
        tmp->next = new;
    }
}

/*******************HANDLE APPEND************************/
int handle_APPEND(char *input,int i,t_token **head)
{
    t_token *new_token;
    char *word;
    word = ft_substr(input,i,2);
    new_token = cretae_token(word,APPEND);
    append_token(head,new_token);
    free(word);
    i+=2;
    return i;
}
/*********************HANDLE HERDOC****************************/
int handle_herdoc(char *input,int i,t_token **head)
{
    char *word;
    t_token *new;
     word = ft_substr(input,i,2);
    new = cretae_token(word,HEREDOC);
    append_token(head,new);
    free(word);
    i+=2;
    return i;
}
/*************************HANDLE OUTFILE************************/
int handle_outfile(char *input,int i,t_token **head)
{
    char *word;
    t_token *new;
    word = ft_substr(input,i,1);
    new = cretae_token(word,REDIR_OUT);
    append_token(head,new);
    free(word);
    i++;
    return(i);
}
/*************************HANDLE INFILE************************/
int handle_intfile(char *input,int i,t_token **head)
{
    char *word;
    t_token *new;
    word = ft_substr(input,i,1);
    new = cretae_token(word,REDIR_IN);
    append_token(head,new);
    free(word);
    i++;
    return(i);
}
/*************************HANDLE PIPE************************/
int handle_pipe(char *input,int i,t_token **head)
{
    char *word;
    t_token *new;
    word = ft_substr(input,i,1);
    new = cretae_token(word,PIPE);
    append_token(head,new);
    free(word);
    i++;
    return(i);
}
/***********************handle word**************************/
int handle_word(char *input, int i, t_token **head)
{
    int start ;
    char *word;
    t_token *new;
    char q;
    start = i;
    q = 0;
     while (input[i] && (!ft_isspace(input[i]) && input[i] != '>' && input[i] != '<' && input[i] != '|'))
    {
        if (input[i] == '"' || input[i] == '\'')
        {
            char q = input[i++];
            while (input[i] && input[i] != q)
                i++;
            if (input[i] == q)
                i++;
        }
        else
            i++;
    }
    word = ft_substr(input,start,i - start);
    new = cretae_token(word,WORD);
    append_token(head,new);
    return(i);
}

t_token *tokenize(char *input)
{
    int i = 0;
    t_token *head = NULL;
    while(input[i] != '\0')
    {
        if(ft_isspace(input[i]))
            i++;
        // else if(input[i]=='"' || input[i] == '\'')
        //     i = handle_quotes(input,i,&head);
        else if(input[i] == '>' && input[i + 1] =='>')
            i = handle_APPEND(input,i,&head);
        else if(input[i] == '<' && input[i + 1] == '<')
            i = handle_herdoc(input,i,&head);
        else if(input[i] == '>')
            i =  handle_outfile(input,i,&head);
        else if(input[i] == '<')
            i = handle_intfile(input,i,&head);
        else if(input[i] == '|')
            i = handle_pipe(input,i,&head);
        else
            i = handle_word(input,i,&head);
    }
    return (head);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmouafik <rmouafik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 14:58:54 by haboucha          #+#    #+#             */
/*   Updated: 2025/09/10 11:20:16 by rmouafik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int is_valid_env_char(char c)
{
    return((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '_');
}
int ft_stncmp(char *s1,char *s2,int n)
{
    int i = 0;
    while(s1[i] && s2[i] && i < n)
    {
        if(s1[i] > s2[i])
            return 1;
        else if(s1[i] < s2[i])
            return -1;
        i++;
    }
    if(i < n)
    {
        if(s1[i] > s2[i])
            return 1;
        else if(s1[i] < s2[i])
            return -1;
    }
    return 0;    
}

char *get_env_value_par(char *var,char **envp)
{
    int i = 0;
    int len = ft_strlen(var);
    while(envp[i])
    {
        if(ft_stncmp(envp[i],var,len) == 0 && envp[i][len] == '=')
            return(envp[i] + len + 1);
        i++;  
    }
    return(NULL);
}

char *ft_strjoin_char(char *s1,char c)
{
    int i = 0;
    int len = ft_strlen(s1) + 1;
    char *p = malloc(len + 1);
    if(!p)
        return NULL;
    while(s1[i])
    {
        p[i] = s1[i];
        i++;
    }
    p[i] = c;
    i++;
    p[i] ='\0';
    return p;
}
int count(int nbr)
{
    int count = 0;
    if(nbr == 0)
        return(1); 
    else if(nbr < 0)
    {
        nbr *=-1;
        count++;
    }
    while(nbr > 0)
    {
        nbr = nbr / 10;
        count++;
    }
    return(count);
}

char *ft_itoa(int nbr)
{
    int i;
    int len =count(nbr);

    i = len -1;
    char *p =malloc(len + 1);
    if(!p)
        return(NULL);
    if(nbr == 0)
    {
        p[0] ='0';
        p[1] ='\0';
        return(p);
    }
    if(nbr < 0)
    {
        p[0] = '-';
        nbr *= -1;
    }
    while(nbr > 0)
    {
        p[i] = (nbr % 10) + '0';
        nbr = nbr /10;
    i--;
    }
    p[len] ='\0';
    return(p);
}

char *expand_string(char *word,char **envp, t_env *env_head)
{
    int i =0;
    int start = 0;
    char *var_name;
    char *value;
    char *tmp;
    char quote = 0;
    char *resulat = ft_strdup("");
    if(!word)
        return(ft_strdup(""));
    while(word[i])
    {
        if(word[i] == '\'' && quote == 0)
        {
            quote = '\'';
            i++;
        }
        else if(word[i] == '\'' && quote == '\'')
        {
            quote = 0;
            i++;
        }
        else if(word[i] == '"' && quote == 0)
        {
            quote = '"';
            i++;
        }
        else if(word[i] == '"' && quote == '"')
        {
            quote = 0;
            i++;
        }
        else if(word[i] == '$' && quote !='\'' )
        {
            i++;
            if(word[i] == '?')
            {
                char *str = ft_itoa(env_head->exit_status);
                tmp = ft_strjoin(resulat,str);
                free(resulat);
                free(str);
                resulat = tmp;
                i++;
            }
            start = i;
            while(word[i] && is_valid_env_char(word[i]))
                i++;
            var_name = ft_substr(word,start,i -start);
            value = get_env_value_par(var_name,envp);
            free(var_name);
            if(value)
            {
                    tmp = ft_strjoin(resulat,value);
                    free(resulat);
                    resulat = tmp;
            }
        }
        else
        {
            tmp = ft_strjoin_char(resulat,word[i]);
            free(resulat);
            resulat =tmp;
            i++;
        }
    }
    return(resulat);
}

// int has_quotes(char *s)
// {
//     int i = 0;
//     while(s[i])
//     {
//         if(s[i] == '\'' || s[i] == '"')
//             return 1;
//         i++;
//     }
//     return 0;
// }

void expand_token_list(t_token **head,char **envp, t_env *env_head)
{
    t_token *tmp = *head;
    t_token *prev = NULL;
    while(tmp)
    {
        if(tmp->type == HEREDOC)
        {
            if(tmp->next)
                tmp=tmp->next->next;
            else
                tmp = tmp->next;
            continue;
            
        }
        if(tmp->type == WORD && tmp->value && tmp->new_quote != '\'')
        {
            char *expanded = expand_string(tmp->value,envp, env_head);
            free(tmp->value);
            tmp->value = expanded;
            if(tmp->value[0] == '\0')
            {
                t_token *to_free =tmp;
                if(prev)
                    prev->next = tmp->next;
                else
                    *head =tmp->next;
                tmp = tmp->next;
                free(to_free->value);
                free(to_free);
                continue;
            }
        }
        prev = tmp;
        tmp = tmp->next;
    }
}
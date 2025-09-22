/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmouafik <rmouafik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 14:58:54 by haboucha          #+#    #+#             */
/*   Updated: 2025/09/20 11:00:10 by rmouafik         ###   ########.fr       */
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
    free(str);
    res[j] = '\0';
    return res;
}

// t_token *export_value(char *export)
// {
//     t_token *new_token;
//     new_token = cretae_token(export,WORD);
//     if(!new_token)
//         return (NULL);
//     return(new_token);
// }

// void free_split(char **value)
// {
//     int i = 0;
//     while(value[i])
//     {
//         free(value[i]);
//         i++;
//     }
//     free(value);
// }

// t_token *export_value(char *export)
// {
//     t_token *new_token;
//     t_token *head;
//     t_token *tmp;
//     char    **value;
//     int i = 0;

//     value =ft_split(export,' ');
//     if(!value || !value[0])
//     {
//         free_split(value);
//         return NULL;
//     }
//     tmp = cretae_token(value[i],WORD);
//     if(!tmp)
//     {
//         free_split(value);
//         return NULL;
//     }
//     head = tmp;
//     i++;
//     while(value[i])
//     {
//         new_token = cretae_token(value[i],WORD);
//         if(!new_token)
//         {
//             free_split(value);
//             return NULL;
//         }
//         tmp->next = new_token;
//         tmp = tmp->next;
//         i++;
//     }
//     free_split(value);
//     return head;
// }

char *expand_string(char *word,char **envp, t_env *env_head, int *f)
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
            (*f)=1;
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

int    has_quotes(char *s)
{
    int    i;

    if (!s)
        return (1);
    i = 0;
    while (s[i])
    {
        if (s[i] == '\'' || s[i] == '"')
            return (0);
        i++;
    }
    return (1);
}

void expand_token_list(t_token **head,char **envp, t_env *env_head)
{
    t_token *tmp = *head;
    t_token *prev = NULL;
    t_token *res;
    int f;
    
    while(tmp)
    {
        f=0;
        if (tmp->type == HEREDOC && tmp->next)
        {
            if (!has_quotes(tmp->next->value))
                (*head)->quoted = 1;
            tmp->next->value = remove_quotes(tmp->next->value);
            tmp = tmp->next->next;
            continue;
        }
        if((tmp->type == REDIR_IN || tmp->type == REDIR_OUT || tmp->type == APPEND) && tmp->next)
        {
            if(tmp->next->value && tmp->new_quote != '\'')
            {
                char *expanded = expand_string(tmp->next->value,envp,env_head, &f);
                free(tmp->next->value);
                tmp->next->value = expanded;
                if(expanded[0] == '\0' && f == 1)
                {
                    tmp = tmp->next->next;
                    continue;
                }
            }
            tmp = tmp->next->next;
            continue;
        }
        if(tmp->type == WORD && tmp->value && tmp->new_quote != '\'')
        {
            char *expanded = expand_string(tmp->value,envp,env_head, &f);
            free(tmp->value);
            tmp->value = expanded;
            // int i = 0;
            // while(tmp->value[i])
            // {
            //     if(isspace(tmp->value[i]))
            //     {
            //         res = export_value(expanded);
            //         if(prev)
            //             prev->next = res;
            //         else
            //             *head = res;
            //         t_token *last = res;
            //         while(last->next)
            //             last = last->next;
            //         last->next = tmp->next;
            //         free(tmp->value);
            //         free(tmp);
            //         tmp =last;
            //     }
            //     i++;
            // }
            if(tmp->value[0] == '\0' && f == 1)
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
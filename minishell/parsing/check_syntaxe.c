/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntaxe.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmouafik <rmouafik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 10:30:19 by haboucha          #+#    #+#             */
/*   Updated: 2025/09/11 12:24:45 by rmouafik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/****************check syntaxe quotes*****************/
int check_quotes(char *input)
{
    int i = 0;
    char quote_char = 0;
    while(input[i])
    {
        if((input[i] == '\'' || input[i] == '"') && !quote_char)
            quote_char = input[i];
        else if(input[i] == quote_char)
            quote_char = 0;
        i++;
    }
    if(quote_char)
        return (1);
    else 
        return 0;
}

/****************check syntaxe pipe*****************/
int check_pipe_syntaxe(char *input)
{
    int i =0;
    int len = ft_strlen(input) ;
    int end_input = len - 1;
    char quote = 0;
    while(ft_isspace(input[i]))
        i++;
    if(input[i] == '|')
        return 1;
    while(end_input >= 0 && ft_isspace(input[end_input]))
        end_input--;
    if(input[end_input] == '|')
        return 1;
    while(input[i])
    {
         if ((input[i] == '\'' || input[i] == '"') && !quote)
            quote = input[i];
        else if (input[i] == quote)
            quote = 0;
        if(!quote)
        {
            if(input[i] == '|' && input[i+1] && input[i+1]=='|')
                return 1;
            if(input[i] == '|')
            {
                int j = i + 1;
                while (ft_isspace(input[j]))
                    j++;
                if(input[j] == '|')
                    return 1;  
            }
        }
        i++;    
    }
    return 0;
}
/****************check syntaxe redirection*****************/

int check_redirection_syntaxe(char *input)
{
    int i = 0;
    char redirect;
    int count;
    char quote = 0;
    while(input[i])
    {
        if ((input[i] == '"' || input[i] == '\'') && !quote)
        {
            quote = input[i];
            i++;
            while (input[i] && input[i] != quote)
                i++;
            if (input[i] == quote)
                quote = 0;
            i++;
            continue;
        }
        if(input[i]== '>' || input[i] == '<')
        {
            redirect = input[i];
            count = 0;
            while(input[i] == redirect)
            {
                count++;
                i++;
            }
            if(count > 2)
                return 1;
            while(ft_isspace(input[i]))
                i++;
            if(input[i] == '\0' || input[i] == '|' || input[i] == '>' || input[i] == '<')
                return 1;
        }
        else
            i++;   
        }
   return 0;
}
/********************check all syntaxe**************/
int check_all_syntaxe(char *input)
{
    if (check_pipe_syntaxe(input))
    {
        ft_putendl_fd("Minishell: syntax error near unexpected token `|'", 2);
        return 1;
    }
    if (check_quotes(input))
    {
        ft_putendl_fd("Minishell: syntax error near unexpected token `\"'", 2);
        return 1;
    }
    if (check_redirection_syntaxe(input))
    {
        ft_putendl_fd("Minishell: syntax error near unexpected token `newline'", 2);
        return 1;
    }
    return 0;
}


/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execute.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmouafik <rmouafik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 12:37:38 by rmouafik          #+#    #+#             */
/*   Updated: 2025/07/09 11:34:03 by rmouafik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void signal_handler(int sig)
{
    if (sig == SIGINT)
    {
        write(1,"\n",1);
        rl_on_new_line();
        rl_replace_line("", 0);
        rl_redisplay();
        g_signal = SIGINT;
    }
    else if (sig == SIGQUIT)
        return ;
}

void disable_echoctl(void)
{
    struct termios term;

    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag &= ~ECHOCTL;
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

void    free_env(t_env *head)
{
    t_env *tmp;

    while (head)
    {
        tmp = head->next;
        free(head->key);
        free(head->value);
        free(head);
        head = tmp;
    }

}

void handle_end(t_env *env)
{
    rl_clear_history();
    free_env(env);
    write(1,"exit\n", 6);
    exit(env->exit_status);
}
void setup_signals(void)
{
    disable_echoctl();
    signal(SIGINT, signal_handler);
    signal(SIGQUIT, SIG_IGN);
}

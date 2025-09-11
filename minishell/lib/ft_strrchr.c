/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmouafik <rmouafik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 18:12:25 by rmouafik          #+#    #+#             */
/*   Updated: 2024/11/18 09:27:58 by rmouafik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	char		b;
	const char	*last;

	last = NULL;
	b = (char)c;
	while (*s)
	{
		if (*s == b)
		{
			last = s;
		}
		s++;
	}
	if (last)
	{
		return ((char *)last);
	}
	if (b == '\0')
	{
		return ((char *)s);
	}
	return (NULL);
}

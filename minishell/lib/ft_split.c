/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmouafik <rmouafik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 12:52:07 by rmouafik          #+#    #+#             */
/*   Updated: 2024/11/18 09:27:05 by rmouafik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	ft_countwords(char const *s, char c)
{
	size_t	count;
	int		in_word;

	count = 0;
	in_word = 0;
	while (*s)
	{
		if (*s != c && in_word == 0)
		{
			in_word = 1;
			count++;
		}
		else if (*s == c)
			in_word = 0;
		s++;
	}
	return (count);
}

static char	**free_split(char **split, size_t i)
{
	while (i > 0)
		free(split[--i]);
	free(split);
	return (NULL);
}

static char	**alloc_words(char **arr, char const *s, char c)
{
	size_t	start;
	size_t	end; 
	size_t	word_idx;

	start = 0;
	end = 0;
	word_idx = 0;
	while (s[start])
	{
		while (s[start] == c)
			start++;
		if (s[start] == '\0')
			break ;
		end = start;
		while (s[end] != c && s[end] != '\0')
			end++;
		arr[word_idx] = ft_substr(s, start, end - start);
		if (!arr[word_idx])
			return (free_split(arr, word_idx));
		word_idx++;
		start = end;
	}
	arr[word_idx] = NULL;
	return (arr);
}

char	**ft_split(char const *s, char c)
{
	char	**arr;

	if (!s)
		return (NULL);
	arr = (char **)malloc(sizeof(char *) * (ft_countwords(s, c) + 1));
	if (!arr)
		return (NULL);
	return (alloc_words(arr, s, c));
}

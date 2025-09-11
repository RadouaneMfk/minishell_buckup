/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmouafik <rmouafik@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 08:39:48 by rmouafik          #+#    #+#             */
/*   Updated: 2024/11/18 09:24:51 by rmouafik         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	void	*arr;
	size_t	total_size;

	total_size = count * size;
	if (size && count > total_size / size)
		return (NULL);
	arr = (void *)malloc(total_size);
	if (arr == NULL)
		return (NULL);
	ft_bzero(arr, total_size);
	return (arr);
}

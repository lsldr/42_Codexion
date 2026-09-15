/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_uint.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuleime <asuleime@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/10 14:42:25 by asuleime          #+#    #+#             */
/*   Updated: 2026/09/15 11:10:26 by asuleime         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	check_uint(char **argv, int i)
{
	char			*raw_str;
	int				j;
	unsigned long	s_ul;

	raw_str = argv[i];
	j = strlen(raw_str);
	if (j > 10)
		return (0);
	s_ul = 0;
	while (--j > -1)
		if (raw_str[j] < 48 || raw_str[j] > 57)
			return (0);
	s_ul = ft_strtoul(raw_str, strlen(raw_str));
	if (s_ul >> 32)
		return (0);
	return (1);
}

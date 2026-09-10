/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_uint.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuleime <asuleime@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/10 14:42:25 by asuleime          #+#    #+#             */
/*   Updated: 2026/09/10 15:16:28 by asuleime         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

/*Does not have checks for chars being digits (handled in is_uint()*/
unsigned long	ft_strtoul(char *s, int s_len)
{
	int				i;
	unsigned long	ul;

	i = -1;
	ul = 0;
	while (++i < s_len)
		ul = ul * 10 + s[i] - 48;
	return (ul);
}

int is_uint(char *s)
{
	int				i;
	int				len;
	unsigned char	c;
	unsigned int	s_ul;

	i = -1;
	len = strlen(s);
	if (len > 10)
		return (0);
	while (++i < len)
	{
		c = (unsigned char)*(s + i);
		if (c < 48 || c > 57)
			return (0);
	}
	s_ul = ft_strtoul(s, len);
}

int	verify_uint(char **argv, int i)
{
	char	*raw_str;

	raw_str = *(argv + i);
	if (!is_uint(raw_str):
			return (0);
	return (1);
}

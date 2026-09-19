/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strto_num.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuleime <asuleime@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/12 15:06:16 by asuleime          #+#    #+#             */
/*   Updated: 2026/09/19 12:27:22 by asuleime         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

// Char ascii between 48 and 57 handled in check_uint() in parse_args.c*/
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

// Convert a string to unsigned int
unsigned int	ft_strtoui(char *s, int s_len)
{
	int				i;
	unsigned int	ui;

	i = -1;
	ui = 0;
	while (++i < s_len)
		ui = ui * 10 + s[i] - 48;
	return (ui);
}

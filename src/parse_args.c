/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuleime <asuleime@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/10 11:27:58 by asuleime          #+#    #+#             */
/*   Updated: 2026/09/12 11:26:41 by asuleime         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	fill_pargs(char **argv, t_pargs *args_p)
{
	int	i;

	i = 0;
	while (++i < SCHEDULER)
	{
		args_p->i
	}
}

int	parse_args(char **argv, t_pargs *args_p)
{
	unsigned int	i;

	i = 0;
	if (strcmp(argv[8], "fifo") != 0 && strcmp(argv[8], "edf") != 0)
		return (fprintf(stderr, "Scheduler must be 'fifo' or 'edf'.\n"), 1);
	while (++i < 8)
		if (!verify_uint(argv, i))
			return (1);
	fill_args(argv, args_p);
	return (0);
}

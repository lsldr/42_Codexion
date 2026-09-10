/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuleime <asuleime@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/10 11:27:58 by asuleime          #+#    #+#             */
/*   Updated: 2026/09/10 14:45:20 by asuleime         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	parse_args(char **argv, t_pargs *args_p)
{
	unsigned int	i;

	i = 0;
	if (strcmp(argv[8], "fifo") != 0 && strcmp(argv[8], "edf") != 0)
		return (fprintf(stderr, "Scheduler must be set to 'fifo' or 'edf'.\n") \
				, 1);
	while (++i < 8)
	{
		if (!verify_uint(argv, i)
				return (1);
	}
	return 0;
}

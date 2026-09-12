/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuleime <asuleime@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/10 11:06:26 by asuleime          #+#    #+#             */
/*   Updated: 2026/09/12 11:15:39 by asuleime         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "src/codexion.h"

int	main(int argc, char **argv)
{
	t_pargs	*args_p;

	args_p = (t_pargs *)malloc(sizeof(t_pargs));
	if (argc != 9)
		return (fprintf(stderr, "8 arguments must be passed.\n"), 1);
	if (parse_args(argv, args_p))
		return (fprintf(stderr, "Invalid arguments.\n"), 1);
	return (0);
}

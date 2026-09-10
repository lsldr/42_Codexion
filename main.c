/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuleime <asuleime@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/10 11:06:26 by asuleime          #+#    #+#             */
/*   Updated: 2026/09/10 11:30:01 by asuleime         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "src/codexion.h"

int main(int argc, char** argv)
{
	t_pargs	args;

	if (argc != 9)
		return (fprintf(stderr, "8 arguments must be passed.\n"), 1);
	if (parse_args(argv, &args))
		return (fprintf(stderr, "Invalid arguments.\n"), 1);
	return (0);
}

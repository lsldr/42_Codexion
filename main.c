/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuleime <asuleime@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/10 11:06:26 by asuleime          #+#    #+#             */
/*   Updated: 2026/09/21 16:08:23 by asuleime         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "src/codexion.h"

int	main(int argc, char **argv)
{
	t_data	*data;

	if (argc != 9)
		return (fprintf(stderr, "8 arguments must be passed.\n"), 1);
	data = malloc(sizeof(t_data));
	if (!data)
		return (fprintf(stderr, "Data malloc failed.\n"), 1);
	memset(data, 0, sizeof(t_data));
	if (parse_args(argv, data))
	{
		free(data);
		return (fprintf(stderr, "Invalid arguments.\n"), 1);
	}
	codexion(data);
	return (0);
}

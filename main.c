/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuleime <asuleime@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/10 11:06:26 by asuleime          #+#    #+#             */
/*   Updated: 2026/09/15 11:56:04 by asuleime         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "src/codexion.h"

/* void	print_data(t_data *data)
{
	printf("%u\n", data->n_coders);
	printf("%u\n", data->burnout_t);
	printf("%u\n", data->compile_t);
	printf("%u\n", data->debug_t);
	printf("%u\n", data->refactor_t);
	printf("%u\n", data->req_compiles);
	printf("%u\n", data->d_cooldown);
	printf("%d\n", (int)data->scheduler);
} */

int	main(int argc, char **argv)
{
	t_data	*data;

	if (argc != 9)
		return (fprintf(stderr, "8 arguments must be passed.\n"), 1);
	data = malloc(sizeof(t_data));
	if (parse_args(argv, data))
		return (clean_all(data), fprintf(stderr, "Invalid arguments.\n"), 1);
	codexion(data);
	return (0);
}

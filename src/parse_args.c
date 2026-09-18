/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuleime <asuleime@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/10 11:27:58 by asuleime          #+#    #+#             */
/*   Updated: 2026/09/18 21:29:01 by asuleime         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

// Match the argv order of a numeric program argument with the fields in `data`
static void	nparg_router(unsigned int ui_arg, int i, t_data *data)
{
	if (i == 2)
		data->burnout_t = ui_arg;
	if (i == 3)
		data->compile_t = ui_arg;
	if (i == 4)
		data->debug_t = ui_arg;
	if (i == 5)
		data->refactor_t = ui_arg;
	if (i == 6)
		data->req_compiles = ui_arg;
	if (i == 7)
		data->d_cooldown = ui_arg;
	return ;
}

// Initialize some fields of data from the numeric program arguments
static int	fill_npargs(char **argv, t_data *data)
{
	int				i;
	unsigned int	ui_arg;

	i = 1;
	ui_arg = ft_strtoui(argv[i], strlen(argv[i]));
	if (ui_arg > 300 || ui_arg == 0)
		return (fprintf(stderr, "Only 1-300 coders accepted.\n"), 1);
	data->n_coders = (unsigned short)ui_arg;
	while (++i < 8)
	{
		ui_arg = ft_strtoui(argv[i], strlen(argv[i]));
		nparg_router(ui_arg, i, data);
	}
	return (0);
}

int	parse_args(char **argv, t_data *data)
{
	unsigned int	i;

	i = 0;
	if (!strcmp(argv[8], "fifo"))
		data->scheduler = FIFO;
	else if (!strcmp(argv[8], "edf"))
		data->scheduler = EDF;
	else
		return (fprintf(stderr, "Scheduler must be fifo or edf\n"), 1);
	while (++i < 8)
		if (!check_uint(argv, i))
			return (fprintf(stderr, "%s cannot be uint!\n", argv[i]), 1);
	if (fill_npargs(argv, data))
		return (1);
	return (0);
}

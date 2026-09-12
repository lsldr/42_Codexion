/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuleime <asuleime@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/10 11:15:56 by asuleime          #+#    #+#             */
/*   Updated: 2026/09/12 11:23:11 by asuleime         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <string.h>

enum
{
	N_CODERS = 1,
	BURNOUT_T,
	COMPILE_T,
	DEBUG_T,
	REFACTOR_T,
	REQ_COMPILES,
	D_COOLDOWN,
	SCHEDULER
};

typedef struct s_pargs
{
	unsigned int	n_coders;
	unsigned int	burnout_t;
	unsigned int	compile_t;
	unsigned int	debug_t;
	unsigned int	refactor_t;
	unsigned int	req_compiles;
	unsigned int	d_cooldown;
	char			*scheduler;
}	t_pargs;

int		parse_args(char **argv, t_pargs *args_p);

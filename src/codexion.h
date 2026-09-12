/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuleime <asuleime@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/10 11:15:56 by asuleime          #+#    #+#             */
/*   Updated: 2026/09/12 17:32:45 by asuleime         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <string.h>

typedef enum e_scheduler
{
	EDF,
	FIFO
}	t_scheduler;

typedef struct s_dongle
{
	pthread_mutex_t	*d_mutex;
}	t_dongle;

typedef struct s_coder
{
	unsigned short	coder_num;
	t_dongle		*l_dongle;
	t_dongle		*r_dongle;
}	t_coder;

// All "*_t" fields and d_cooldown represent number of milliseconds
typedef struct s_data
{
	unsigned int	n_coders;
	unsigned int	burnout_t;
	unsigned int	compile_t;
	unsigned int	debug_t;
	unsigned int	refactor_t;
	unsigned int	req_compiles;
	unsigned int	d_cooldown;
	t_scheduler		scheduler;
	pthread_mutex_t	*log_mutex;
	t_coder			*coders;
	t_dongle		*dongles;
}	t_data;

int				parse_args(char **argv, t_data *data);
int				check_uint(char **argv, int i);

unsigned long	ft_strtoul(char *s, int s_len);
unsigned int	ft_strtoui(char *s, int s_len);

int				fill_npargs(char **argv, t_data *data);
void			nparg_router(unsigned int *ui_arg, int i, t_data *data);

void			codexion(t_data *data);
void			init_coders(t_data *data);
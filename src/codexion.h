/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuleime <asuleime@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/10 11:15:56 by asuleime          #+#    #+#             */
/*   Updated: 2026/09/14 11:41:37 by asuleime         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

typedef enum e_scheduler
{
	EDF,
	FIFO
}	t_scheduler;

typedef struct s_pqueue
{
	unsigned long	min_req_t;
	unsigned long	min_bout_ddl;
	t_coder			*front_coder;
	t_coder			*back_coder;
}	t_pqueue;

typedef struct s_dongle
{
	unsigned short	id;
	pthread_mutex_t	*mutex;
	t_pqueue		*queue;
	unsigned long	free_t;
}	t_dongle;

typedef struct s_coder
{
	unsigned short	coder_num;
	unsigned int	cc_count;
	pthread_t		*thr;
	pthread_cond_t	*cond;
	unsigned long	last_cc_t;
	t_dongle		*l_dongle;
	t_dongle		*r_dongle;
	t_data			*data;
}	t_coder;

typedef struct s_monitor
{
	pthread_t		*thr;
	bool			is_end;
	pthread_mutex_t	*end_mutex;
	t_coder			*coders;
}	t_monitor;

// All "*_t" fields and d_cooldown represent number of milliseconds
typedef struct s_data
{
	unsigned short	n_coders;
	unsigned int	burnout_t;
	unsigned int	compile_t;
	unsigned int	debug_t;
	unsigned int	refactor_t;
	unsigned int	req_compiles;
	unsigned int	d_cooldown;
	t_scheduler		scheduler;
	t_monitor		*monitor;
	pthread_mutex_t	*log_mutex;
	t_coder			*coders;
	t_dongle		*dongles;
}	t_data;

int				parse_args(char **argv, t_data *data);
int				check_uint(char **argv, int i);

unsigned long	ft_strtoul(char *s, int s_len);
unsigned int	ft_strtoui(char *s, int s_len);

void			codexion(t_data *data);
void			init_coders(t_data *data);
bool			validate_data(t_data *data);

void			coder_routine(void *arg);
void			monitor_routine(void *arg);

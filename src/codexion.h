/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuleime <asuleime@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/10 11:15:56 by asuleime          #+#    #+#             */
/*   Updated: 2026/09/15 12:13:20 by asuleime         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

// Scheduling policy: earliest deadline first || first-in-first-out
typedef enum e_scheduler
{
	EDF,
	FIFO
}	t_scheduler;

typedef struct s_request
{
	unsigned short	coder_num;	// The tie-breaker: lower value priority
	unsigned long	key;		// Req time (FIFO) or deadline (EDF)
	pthread_cond_t	*cond;		// A coder waits on this cond
	bool			is_granted;
}	t_request;

// Binary Min-Heap queue for each dongle
typedef struct s_pqueue
{
	t_request		*items[4];
	int				size;
}	t_pqueue;

typedef struct s_dongle
{
	unsigned short	id;
	pthread_mutex_t	mutex;
	t_pqueue		queue;		// Priority queue
	bool			in_use;
	unsigned long	avlb_at;
}	t_dongle;

typedef struct s_coder
{
	unsigned short	coder_num;
	pthread_t		thr;
	pthread_cond_t	cond;		// For waiting in a dongle's queue
	pthread_mutex_t	c_mutex;	// Protection of last_cc_t and cc_count
	unsigned int	cc_count;	// Number of compiles completed
	unsigned long	last_cc_t;	// Timestampt of the last compile's start
	t_dongle		*l_dongle;
	t_dongle		*r_dongle;
	t_data			*data;
}	t_coder;

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

	unsigned long	start_t;	// Start timestamp in ms
	bool			is_end;
	pthread_mutex_t	end_mutex;	// Protect is_end
	pthread_mutex_t	log_mutex;	// Avoid interleaving messages

	// The monitor thread checks coders' last_cc_t and cc_count
	pthread_t		monitor_thr;
	t_coder			*coders;
	t_dongle		*dongles;
}	t_data;

int				parse_args(char **argv, t_data *data);
int				check_uint(char **argv, int i);

void			clean_all(t_data *data);

unsigned long	ft_strtoul(char *s, int s_len);
unsigned int	ft_strtoui(char *s, int s_len);

void			codexion(t_data *data);
void			init_ds(t_data *data);
bool			check_ds(t_data *data);

void			*c_routine(void *arg);
void			*m_routine(void *arg);

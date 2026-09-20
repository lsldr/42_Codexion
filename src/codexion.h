/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asuleime <asuleime@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/10 11:15:56 by asuleime          #+#    #+#             */
/*   Updated: 2026/09/20 17:53:24 by asuleime         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <time.h>
# include <unistd.h>

// Scheduling policy: earliest deadline first || first-in-first-out
typedef enum e_scheduler
{
	EDF,
	FIFO
}	t_scheduler;

// Forward declaration as there is a t_data * field in t_coder
typedef struct s_data	t_data;

typedef struct s_request
{
	unsigned short	coder_num;	// The tie-breaker: lower value in priority
	unsigned long	key;		// Request time (FIFO) or deadline (EDF)
	pthread_cond_t	*cond;		// Points to coder->cond
}	t_request;

// Binary Min-Heap queue for each dongle
// "pqueue" stands for "priority queue"
typedef struct s_pqueue
{
	t_request	items[4];
	int			size;
}	t_pqueue;

// All "_t" suffixes below are used to indicate time
// Pthread type names also have it,
// but they are prepended with "pthread_"
typedef struct s_dongle
{
	unsigned short	id;
	pthread_mutex_t	mutex;		// Mutex that coders lock
	t_pqueue		queue;		// Priority queue
	bool			in_use;
	unsigned long	free_t;		// Timestamp of when the dongle will be free
}	t_dongle;

typedef struct s_coder
{
	unsigned short	coder_num;
	pthread_t		thr;
	pthread_cond_t	cond;		// For waiting in a dongle's queue
	pthread_mutex_t	c_mutex;	// Protection of last_cc_t and cc_count
	unsigned int	cc_count;	// Number of compiles completed
	unsigned long	last_cc_t;	// Timestamp of the last compile's start
	t_dongle		*l_dongle;
	t_dongle		*r_dongle;
	t_data			*data;
}	t_coder;

// All "*_t" fields and d_cooldown represent number of milliseconds
typedef struct s_data
{
	// 8 program arguments from argv
	unsigned short	n_coders;
	unsigned int	burnout_t;
	unsigned int	compile_t;
	unsigned int	debug_t;
	unsigned int	refactor_t;
	unsigned int	req_compiles;
	unsigned int	d_cooldown;
	t_scheduler		scheduler;
	// Below are the fields created to manage the simulation
	unsigned long	start_t; // Start timestamp in ms
	bool			is_end;
	pthread_mutex_t	end_mutex; // Protect is_end
	pthread_cond_t	end_cond;
	pthread_mutex_t	log_mutex; // Avoid interleaving messages
	// The monitor thread checks coders' last_cc_t and cc_count
	pthread_t		monitor_thr;
	t_coder			*coders;
	t_dongle		*dongles;
}	t_data;

// Program argument parsing and type checking functions
int				parse_args(char **argv, t_data *data);
int				check_uint(char **argv, int i);

// String to unsigned numeric type conversion utils
unsigned long	ft_strtoul(char *s, int s_len);
unsigned int	ft_strtoui(char *s, int s_len);

// Data structure and pthread initialization functions
int				init_ds(t_data *data);
int				init_threads(t_data *data);

// Master cleaning and simulation functions
void			clean_all(t_data *data);
void			codexion(t_data *data);

// Time and lifecycle utils
unsigned long	get_time_ms(void);
void			set_time(t_data *data);
bool			is_simulation_over(t_data *data);
bool			log_action(t_coder *coder, char *action);
bool			coder_sleep(t_coder *coder, unsigned int duration_ms);

// Coder compile cycle and dongle operations
bool			compile_cycle(t_coder *coder);
void			release_dongle(t_dongle *dongle, unsigned int cooldown_ms);
bool			acquire_dongles(t_coder *coder, t_dongle *l_dongle,\
								t_dongle *r_dongle);

// Thread routine functions
void			*c_routine(void *arg);
void			*m_routine(void *arg);

// Priority queue (min-heap) operations and utilities
void			heap_push(t_pqueue *q, t_request req);
t_request		heap_pop(t_pqueue *q);
unsigned short	heap_peek(t_pqueue *queue);
void			heap_remove(t_pqueue *q, unsigned short coder_num);
bool			has_priority(t_request req_a, t_request req_b);
void			exit_queues(t_coder *coder);
void			pop_queues(t_coder *coder);


#endif
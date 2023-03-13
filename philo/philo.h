/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjourno <mjourno@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 13:29:06 by mjourno           #+#    #+#             */
/*   Updated: 2023/03/13 12:30:13 by mjourno          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

//for write
# include <unistd.h>
//for malloc
# include <stdlib.h>
//for printf
# include <stdio.h>
//for gettimeofday
# include <sys/time.h>
//for threads
#include <pthread.h>

typedef enum	e_state
{
	AVAILABLE,
	USED
}	t_state;

typedef struct s_philosopher
{
	//arguments
	int				nb_philo;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				nb_times_to_eat;
	//enum to know forks state
	t_state			*forks;
	//time of threads start
	struct timeval	*time_of_day_start;
	//mutexes
	pthread_mutex_t	*print;
	pthread_mutex_t	*toggle_fork;
	//philosopher's index
	int				index;
}	t_philosopher;

typedef struct s_philo
{
	//arguments
	int				nb_philo;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				nb_times_to_eat;
	//enum to know forks state
	t_state			*forks;
	//time of threads start
	struct timeval	*time_of_day_start;
	//philosopher threads
	pthread_t		*threads;
	//mutexes
	pthread_mutex_t	*print;
	pthread_mutex_t	*toggle_fork;
	//philosophers
	t_philosopher	**philosopher;
}	t_philo;

//error.c
int		write_error(char *error);

//parsing.c
int		parsing(int argc, char **argv, t_philo *philo);
int		special_cases(t_philo *philo);

//free.c
void	free_philo(t_philo *philo);

//prerequisites.c
int	init_prerequisites(t_philo *philo);

//threads.c
int	init_threads(t_philo *philo);

#endif

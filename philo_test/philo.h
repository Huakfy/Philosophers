/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjourno <mjourno@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 13:29:06 by mjourno           #+#    #+#             */
/*   Updated: 2023/03/21 17:10:58 by mjourno          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

//for write and usleep
# include <unistd.h>
//for malloc
# include <stdlib.h>
//for printf
# include <stdio.h>
//for gettimeofday
# include <sys/time.h>
//for threads
#include <pthread.h>

//this enumeration is used to see the state of each philosopher
typedef enum	e_philo_state
{
	START,
	THINKING,
	EATING,
	SLEEPING
}	t_state_philo;

//Structure for each philosopher
typedef struct s_philosopher
{
	//arguments
	int				nb_philo;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				nb_times_to_eat;
	//time of threads's start
	long			*time_of_day_start;
	//mutexes
	pthread_mutex_t	*print;
	pthread_mutex_t	**fork;
	pthread_mutex_t	*toggle_fork;
	pthread_mutex_t	*meal;
	pthread_mutex_t	*death;
	//philosopher's index
	int				index;
	//values to keep track of progress
	int				nb_times_eaten;
	long			last_time_eaten;
	t_state_philo	state_philo;
	int				*philo_died;
}	t_philosopher;

//Arguments structure
typedef struct s_philo
{
	//arguments
	int				nb_philo;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				nb_times_to_eat;
	//time of threads start
	long			time_of_day_start;
	//philosopher threads
	pthread_t		*threads;
	//mutexes
	pthread_mutex_t	*print;
	pthread_mutex_t	**fork;
	pthread_mutex_t	*meal;
	pthread_mutex_t	*death;
	//philosophers
	t_philosopher	**philosopher;
	//track if a philo died
	int				philo_died;
}	t_philo;

//error.c
int		write_error(char *error);

//parsing.c
int		parsing(int argc, char **argv, t_philo *philo);

//free.c
void	free_philo(t_philo *philo);

//prerequisites.c
int		init_prerequisites(t_philo *philo);

//threads.c
int		init_threads(t_philo *philo);
int		check_death(t_philosopher	*philosopher);

//eat_sleep_think.c
int		eat(t_philosopher *philosopher);
int		philo_sleep(t_philosopher *philosopher);
int		think(t_philosopher *philosopher);
void	check_end_threads(t_philo *philo);

//forks_utils.c
int		forks_available(t_philosopher *philosopher);
void	put_down_forks(t_philosopher *philosopher);
int		think_sleep(t_philosopher *philosopher);

//time.c
int		init_start_time(t_philo *philo);
long	now_time(t_philosopher *philosopher);

#endif
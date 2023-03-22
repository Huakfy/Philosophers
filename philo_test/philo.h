/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjourno <mjourno@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 13:29:06 by mjourno           #+#    #+#             */
/*   Updated: 2023/03/22 16:02:25 by mjourno          ###   ########.fr       */
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
# include <pthread.h>

//Structure for each philosopher
typedef struct s_philosopher
{
	int				nb_philo;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				nb_times_to_eat;
	long			*time_of_day_start;
	pthread_mutex_t	*print;
	pthread_mutex_t	**fork;
	pthread_mutex_t	*toggle_fork;
	pthread_mutex_t	*meal;
	pthread_mutex_t	*death;
	int				index;
	int				nb_times_eaten;
	long			last_time_eaten;
	int				*philo_died;
}	t_philosopher;

//Arguments structure
typedef struct s_philo
{
	int				nb_philo;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				nb_times_to_eat;
	long			time_of_day_start;
	pthread_t		*threads;
	pthread_mutex_t	*print;
	pthread_mutex_t	**fork;
	pthread_mutex_t	*meal;
	pthread_mutex_t	*death;
	t_philosopher	**philosopher;
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
void	check_end_threads(t_philo *philo);

//eat_sleep_think.c
int		eat(t_philosopher *philosopher);
int		philo_sleep(t_philosopher *philosopher);
int		think(t_philosopher *philosopher);
int		verify_nb_times_eaten(t_philo *philo);

//forks_utils.c
int		forks_available(t_philosopher *philosopher);
void	put_down_forks(t_philosopher *philosopher);
int		think_sleep(t_philosopher *philosopher);

//time.c
int		init_start_time(t_philo *philo);
long	now_time(t_philosopher *philosopher);

//routine.c
int		check_death(t_philosopher	*philosopher);
void	*start_routine(void *arg);
int		times_eaten(t_philo *philo, int i);
int		time_death(t_philo *philo, int i);

#endif

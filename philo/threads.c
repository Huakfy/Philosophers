/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjourno <mjourno@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 12:14:55 by mjourno           #+#    #+#             */
/*   Updated: 2023/03/14 15:54:47 by mjourno          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	die(t_philosopher *philosopher)
{
	pthread_mutex_lock(philosopher->print);
	gettimeofday(philosopher->now, NULL);
	printf("%ld %d died\n",((philosopher->now->tv_sec - philosopher->time_of_day_start->tv_sec) * 1000) + ((philosopher->now->tv_usec - philosopher->time_of_day_start->tv_usec) / 1000), philosopher->index);
	pthread_mutex_unlock(philosopher->print);
}

static void	take_fork(t_state *fork, t_philosopher *philosopher)
{
	pthread_mutex_lock(philosopher->print);
	gettimeofday(philosopher->now, NULL);
	printf("%ld %d has taken a fork\n",((philosopher->now->tv_sec - philosopher->time_of_day_start->tv_sec) * 1000) + ((philosopher->now->tv_usec - philosopher->time_of_day_start->tv_usec) / 1000), philosopher->index);
	*fork = USED;
	pthread_mutex_unlock(philosopher->print);
}

static int	eat(t_philosopher *philosopher)
{
	//if sleep->eat print thinking before
	pthread_mutex_lock(philosopher->print);
	gettimeofday(philosopher->now, NULL);
	printf("%ld %d is eating\n",((philosopher->now->tv_sec - philosopher->time_of_day_start->tv_sec) * 1000) + ((philosopher->now->tv_usec - philosopher->time_of_day_start->tv_usec) / 1000), philosopher->index);
	pthread_mutex_unlock(philosopher->print);
	gettimeofday(philosopher->last_time_eaten, NULL);
	if (philosopher->time_to_eat > philosopher->time_to_die)
	{
		usleep((philosopher->time_to_die) * 1000);
		return (1);
	}
	usleep(philosopher->time_to_eat * 1000);
	return (0);
}

static int	forks_available(t_philosopher *philosopher)
{
	pthread_mutex_lock(philosopher->toggle_fork);
	if (philosopher->index != philosopher->nb_philo)
	{
		if (philosopher->forks[philosopher->index - 1] == AVAILABLE
			&& philosopher->forks[philosopher->index] == AVAILABLE)
		{
			take_fork(&philosopher->forks[philosopher->index - 1], philosopher);
			take_fork(&philosopher->forks[philosopher->index], philosopher);
			pthread_mutex_unlock(philosopher->toggle_fork);
			return (1);
		}
	}
	else
	{
		if (philosopher->forks[philosopher->index - 1] == AVAILABLE
			&& philosopher->forks[0] == AVAILABLE)
		{
			take_fork(&philosopher->forks[philosopher->index - 1], philosopher);
			take_fork(&philosopher->forks[0], philosopher);
			pthread_mutex_unlock(philosopher->toggle_fork);
			return (1);
		}
	}
	pthread_mutex_unlock(philosopher->toggle_fork);
	return (0);
}

static void	put_down_forks(t_philosopher *philosopher)
{
	if (philosopher->index != philosopher->nb_philo)
	{
		pthread_mutex_lock(philosopher->toggle_fork);
		philosopher->forks[philosopher->index - 1] = AVAILABLE;
		philosopher->forks[philosopher->index] = AVAILABLE;
		pthread_mutex_unlock(philosopher->toggle_fork);
	}
	else
	{
		pthread_mutex_lock(philosopher->toggle_fork);
		philosopher->forks[philosopher->index - 1] = AVAILABLE;
		philosopher->forks[0] = AVAILABLE;
		pthread_mutex_unlock(philosopher->toggle_fork);
	}
}

static int	philo_sleep(t_philosopher *philosopher)
{
	pthread_mutex_lock(philosopher->print);
	gettimeofday(philosopher->now, NULL);
	printf("%ld %d is sleeping\n",((philosopher->now->tv_sec - philosopher->time_of_day_start->tv_sec) * 1000) + ((philosopher->now->tv_usec - philosopher->time_of_day_start->tv_usec) / 1000), philosopher->index);
	pthread_mutex_unlock(philosopher->print);
	if (philosopher->time_to_sleep > (philosopher->time_to_die + philosopher->time_to_eat))
	{
		usleep((philosopher->time_to_die + philosopher->time_to_eat) * 1000);
		return (1);
	}
	usleep(philosopher->time_to_sleep * 1000);
	return (0);
}

static void	think(t_philosopher *philosopher)
{
	pthread_mutex_lock(philosopher->print);
	gettimeofday(philosopher->now, NULL);
	printf("%ld %d is thinking\n",((philosopher->now->tv_sec - philosopher->time_of_day_start->tv_sec) * 1000) + ((philosopher->now->tv_usec - philosopher->time_of_day_start->tv_usec) / 1000), philosopher->index);
	pthread_mutex_unlock(philosopher->print);
	usleep(philosopher->time_to_eat * 1000);
	//verify if going to die
}

static void	*start_routine(void	*arg)
{
	t_philosopher	*philosopher;

	philosopher = (t_philosopher *)arg;
	//init last_time_eaten to start time
	gettimeofday(philosopher->last_time_eaten, NULL);
	//wait for all threads to be created
	pthread_mutex_lock(philosopher->print);
	pthread_mutex_unlock(philosopher->print);
	//while (1)
	//{
		//If 2 forks are available take them
		if (forks_available(philosopher))
		{
			//eat
			if (eat(philosopher))
			{
				die(philosopher);
				put_down_forks(philosopher);
				return (NULL);
			}
			put_down_forks(philosopher);
			//verify if dead
			//sleep
			if (philo_sleep(philosopher))
			{
				die(philosopher);
				return (NULL);
			}
			//verify if dead
			//continue ;
		}
		else
		{
			//think
			think(philosopher);
			//continue ;
		}
	//}
	return (NULL);
}

//Initiate all values
static int	init_philosopher_values(t_philo *philo, int i)
{
	philo->philosopher[i]->nb_philo = philo->nb_philo;
	philo->philosopher[i]->time_to_die = philo->time_to_die;
	philo->philosopher[i]->time_to_eat = philo->time_to_eat;
	philo->philosopher[i]->time_to_sleep = philo->time_to_sleep;
	philo->philosopher[i]->nb_times_to_eat = philo->nb_times_to_eat;
	philo->philosopher[i]->forks = philo->forks;
	philo->philosopher[i]->time_of_day_start = philo->time_of_day_start;
	philo->philosopher[i]->now = philo->now;
	philo->philosopher[i]->print = philo->print;
	philo->philosopher[i]->toggle_fork = philo->toggle_fork;
	philo->philosopher[i]->index = i + 1;
	philo->philosopher[i]->nb_times_eaten = 0;
	philo->philosopher[i]->last_time_eaten = malloc(sizeof(struct timeval));
	if (!philo->philosopher[i]->last_time_eaten)
	{
		free_philo(philo);
		return (write_error("Error\nMalloc of last_time_eaten timeval structure failed\n"));
	}
	return (0);
}

//Create a philosopher structure for each philosopher, initialize their values
//and start the threads
int	init_threads(t_philo *philo)
{
	int	i;

	printf("time to die: %d time to eat: %d time to sleep: %d nb times to eat: %d\n", philo->time_to_die, philo->time_to_eat, philo->time_to_sleep, philo->nb_times_to_eat);
	pthread_mutex_lock(philo->print);//Lock wathever mutex to wait for every thread to be created
	i = 0;
	while (i < philo->nb_philo)
	{
		philo->philosopher[i] = malloc(sizeof(t_philo));
		if (!philo->philosopher[i])
		{
			free_philo(philo);
			return (write_error("Error\nPhilosopher structure malloc failed\n"));
		}
		if (init_philosopher_values(philo, i))
			return (1);
		if (pthread_create(&philo->threads[i], NULL, &start_routine, philo->philosopher[i]) == -1)
		{
			free_philo(philo);
			return (write_error("Error\nThread creation failed\n"));
		}
		i++;
	}
	gettimeofday(philo->time_of_day_start, NULL);//Initialize start time
	pthread_mutex_unlock(philo->print);//Start threads
	return (0);
}

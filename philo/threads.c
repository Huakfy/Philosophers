/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjourno <mjourno@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 12:14:55 by mjourno           #+#    #+#             */
/*   Updated: 2023/03/13 17:28:15 by mjourno          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	 eat(t_philosopher *philosopher)
{
	if (!pthread_mutex_lock(philosopher->print))
	{
		gettimeofday(philosopher->now, NULL);
		printf("ms:%ld\n",((philosopher->now->tv_sec - philosopher->time_of_day_start->tv_sec) * 1000)
			+ ((philosopher->now->tv_usec - philosopher->time_of_day_start->tv_usec) / 1000));
		pthread_mutex_unlock(philosopher->print);
	}
	else
		eat(philosopher);
}

static void	take_fork(t_state fork, t_philosopher *philosopher)
{
	if (!pthread_mutex_lock(philosopher->print))
	{
		gettimeofday(philosopher->now, NULL);
		printf("%ld %d has taken a fork\n",((philosopher->now->tv_sec - philosopher->time_of_day_start->tv_sec) * 1000)
				+ ((philosopher->now->tv_usec - philosopher->time_of_day_start->tv_usec) / 1000), philosopher->index);
		fork = USED;
		pthread_mutex_unlock(philosopher->print);
	}
	else
		take_fork(fork, philosopher);
}

static int	forks_available(t_philosopher *philosopher)
{
	if (philosopher->index != philosopher->nb_philo)
	{
		if (philosopher->forks[philosopher->index - 1] == AVAILABLE
			&& philosopher->forks[philosopher->index] == AVAILABLE)
		{
			take_fork(philosopher->forks[philosopher->index - 1], philosopher);
			take_fork(philosopher->forks[philosopher->index], philosopher);
			return (1);
		}
	}
	else
	{
		if (philosopher->forks[philosopher->index - 1] == AVAILABLE
			&& philosopher->forks[0] == AVAILABLE)
		{
			take_fork(philosopher->forks[philosopher->index - 1], philosopher);
			take_fork(philosopher->forks[0], philosopher);
			return (1);
		}
	}
	return (0);
}

static void	*start_routine(void	*arg)
{
	t_philosopher	*philosopher;

	philosopher = (t_philosopher *)arg;
	//while (1)
	//{
		if (!pthread_mutex_lock(philosopher->toggle_fork))
		{
			//See if 2 forks are available
			if (forks_available(philosopher))
				eat(philosopher);
			pthread_mutex_unlock(philosopher->toggle_fork);
		}
	//}
	return (NULL);
}

//Initiate all values
static void	init_philosopher_values(t_philo *philo, int i)
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
}

//Create a philosopher structure for each philosopher, initialize their values
//and start the threads
int	init_threads(t_philo *philo)
{
	int	i;

	i = 0;
	while (i < philo->nb_philo)
	{
		philo->philosopher[i] = malloc(sizeof(t_philo));
		if (!philo->philosopher[i])
		{
			free_philo(philo);
			return (write_error("Error\nPhilosopher structure malloc failed\n"));
		}
		init_philosopher_values(philo, i);
		if (pthread_create(&philo->threads[i], NULL, &start_routine, philo->philosopher[i]) == -1)
		{
			free_philo(philo);
			return (write_error("Error\nThread creation failed\n"));
		}
		i++;
	}
	return (0);
}

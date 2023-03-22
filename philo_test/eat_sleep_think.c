/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eat_sleep_think.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjourno <mjourno@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 16:55:07 by mjourno           #+#    #+#             */
/*   Updated: 2023/03/22 11:18:13 by mjourno          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

//Eat
//We first check if a philosopher died while waiting at the mutex then we print
//philospher is thinking in case he is still sleeping (this should not happen
//since I added a new condition in the routine).
//We then print philosopher is eating and wait time_to_eat or less (if
//time_to_eat >= time_to_die) in which case the philosopher dies.
int	eat(t_philosopher *philosopher)
{
	long	time;

	philosopher->state_philo = EATING;
	pthread_mutex_lock(philosopher->print);
	time = now_time(philosopher);
	printf("%ld %d is eating\n", time, philosopher->index);
	philosopher->nb_times_eaten++;
	philosopher->last_time_eaten = time;
	pthread_mutex_unlock(philosopher->print);
	if (philosopher->time_to_eat >= philosopher->time_to_die)
	{
		usleep((philosopher->time_to_die) * 1000);
		return (1);
	}
	usleep(philosopher->time_to_eat * 1000);
	return (0);
}

//Sleep
//In my case sleep always happens after eating (if philosopher didn't die)
//We first check if a philosopher died while waiting at the mutex then we print
//philosopher is sleeping and wait time_to_sleep or less (if time_to_eat +
//time_to_sleep >= time_to_die) in which case the philosopher dies.
int	philo_sleep(t_philosopher *philosopher)
{
	philosopher->state_philo = SLEEPING;
	pthread_mutex_lock(philosopher->print);
	printf("%ld %d is sleeping\n",now_time(philosopher), philosopher->index);
	pthread_mutex_unlock(philosopher->print);
	if (philosopher->time_to_sleep + philosopher->time_to_eat
		>= philosopher->time_to_die)
	{
		usleep((philosopher->time_to_die - philosopher->time_to_eat) * 1000);
		return (1);
	}
	usleep(philosopher->time_to_sleep * 1000);
	return (0);
}

//Think
//Thinking happens either after sleeping or as first action (if forks aren't
//available).
//We first check if a philosopher died while waiting at the mutex then if the
//philosopher wasn't already thinking we print philosopher is thinking then if
//there is an odd number of philosophers we wait time_to_eat or less.
//If the philosopher was already thinking and is just in a loop waiting for
//forks to be available we check if he is supposed to die and make him sleep
//1µs before letting him check for forks again.
int	think(t_philosopher *philosopher)
{
	pthread_mutex_lock(philosopher->print);
	printf("%ld %d is thinking\n",now_time(philosopher),
		philosopher->index);
	pthread_mutex_unlock(philosopher->print);
	philosopher->state_philo = THINKING;
	usleep(((philosopher->time_to_die - (philosopher->time_to_eat
		+ philosopher->time_to_sleep)) / 2) * 1000);
	return (0);
}

int	verify_nb_times_eaten(t_philo *philo)
{
	int	i;

	i = 0;
	while (i < philo->nb_philo)
	{
		if (philo->philosopher[i]->nb_times_eaten < philo->nb_times_to_eat)
			return (0);
		i++;
	}
	return (1);
}

void	check_end_threads(t_philo *philo)
{
	int		i;
	long	time;

	i = 0;
	while (1)
	{
		if (philo->nb_times_to_eat != -1)
		{
			pthread_mutex_lock(philo->meal);
			time = now_time(philo->philosopher[i]);
			if (verify_nb_times_eaten(philo))
			{
				pthread_mutex_lock(philo->death);
				philo->philo_died = 1;
				pthread_mutex_unlock(philo->death);
				pthread_mutex_unlock(philo->meal);
				return ;
			}
			pthread_mutex_unlock(philo->meal);
		}
		pthread_mutex_lock(philo->meal);
		time = now_time(philo->philosopher[i]);
		if (time - philo->philosopher[i]->last_time_eaten > philo->time_to_die)
		{
			pthread_mutex_lock(philo->death);
			philo->philo_died = 1;
			pthread_mutex_unlock(philo->death);
			pthread_mutex_unlock(philo->meal);
			pthread_mutex_lock(philo->print);
			printf("%ld %d died\n", time, philo->philosopher[i]->index);
			pthread_mutex_unlock(philo->print);
			return ;
		}
		pthread_mutex_unlock(philo->meal);
		i++;
		if (i >= (philo->nb_philo - 1))
			i = 0;
		usleep (100);
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjourno <mjourno@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 15:18:17 by mjourno           #+#    #+#             */
/*   Updated: 2023/03/22 15:49:22 by mjourno          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_death(t_philosopher	*philosopher)
{
	pthread_mutex_lock(philosopher->death);
	if (*(philosopher->philo_died) == 1)
	{
		pthread_mutex_unlock(philosopher->death);
		return (1);
	}
	pthread_mutex_unlock(philosopher->death);
	return (0);
}

static int	init_routine(t_philosopher *philosopher)
{
	pthread_mutex_lock(philosopher->meal);
	philosopher->last_time_eaten = now_time(philosopher);
	pthread_mutex_unlock(philosopher->meal);
	if (philosopher->nb_times_to_eat == 0)
		return (1);
	if (philosopher->nb_philo == 1)
	{
		think(philosopher);
		usleep(philosopher->time_to_die * 1000);
		return (1);
	}
	return (0);
}

void	*start_routine(void *arg)
{
	t_philosopher	*philosopher;

	philosopher = (t_philosopher *)arg;
	if (init_routine(philosopher))
		return (NULL);
	while (1)
	{
		if (check_death(philosopher) || forks_available(philosopher))
			return (NULL);
		if (eat(philosopher))
		{
			put_down_forks(philosopher);
			return (NULL);
		}
		put_down_forks(philosopher);
		if (check_death(philosopher) || philo_sleep(philosopher))
			return (NULL);
		if (check_death(philosopher))
			return (NULL);
		think(philosopher);
	}
	return (NULL);
}

int	times_eaten(t_philo *philo, int i)
{
	long	time;

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
			return (1);
		}
		pthread_mutex_unlock(philo->meal);
	}
	return (0);
}

int	time_death(t_philo *philo, int i)
{
	pthread_mutex_lock(philo->meal);
	if (now_time(philo->philosopher[i])
		- philo->philosopher[i]->last_time_eaten > philo->time_to_die)
	{
		pthread_mutex_lock(philo->death);
		philo->philo_died = 1;
		pthread_mutex_unlock(philo->death);
		pthread_mutex_unlock(philo->meal);
		pthread_mutex_lock(philo->print);
		printf("%ld %d died\n", now_time(philo->philosopher[i]),
			philo->philosopher[i]->index);
		pthread_mutex_unlock(philo->print);
		return (1);
	}
	pthread_mutex_unlock(philo->meal);
	return (0);
}

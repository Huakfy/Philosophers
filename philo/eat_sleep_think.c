/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eat_sleep_think.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjourno <mjourno@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 16:55:07 by mjourno           #+#    #+#             */
/*   Updated: 2023/03/17 12:40:28 by mjourno          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	verify_nb_times_eaten(t_philosopher *philosopher)
{
	int	i;

	if (philosopher->nb_times_to_eat == -1)
		return ;
	i = 0;
	while (i < philosopher->nb_philo)
	{
		if (philosopher->philosopher[i]->nb_times_eaten < philosopher->nb_times_to_eat)
			return ;
		i++;
	}
	*(philosopher->philo_died) = 1;
	return ;
}

int	eat(t_philosopher *philosopher)
{
	pthread_mutex_lock(philosopher->print);
	if ((*philosopher->philo_died) == 1)
	{
		pthread_mutex_unlock(philosopher->print);
		return (0);
	}
	if (philosopher->state_philo == SLEEPING)
		printf("%ld %d is thinking\n",now_time(philosopher), philosopher->index);
	philosopher->state_philo = EATING;
	printf("%ld %d is eating\n",now_time(philosopher), philosopher->index);
	philosopher->nb_times_eaten++;
	verify_nb_times_eaten(philosopher);
	pthread_mutex_unlock(philosopher->print);
	philosopher->last_time_eaten = now_time(philosopher);
	if (philosopher->time_to_eat >= philosopher->time_to_die)
	{
		usleep((philosopher->time_to_die) * 1000);
		return (1);
	}
	usleep(philosopher->time_to_eat * 1000);
	return (0);
}

int	philo_sleep(t_philosopher *philosopher)
{
	philosopher->state_philo = SLEEPING;
	pthread_mutex_lock(philosopher->print);
	if ((*philosopher->philo_died) == 1)
	{
		pthread_mutex_unlock(philosopher->print);
		return (0);
	}
	printf("%ld %d is sleeping\n",now_time(philosopher), philosopher->index);
	pthread_mutex_unlock(philosopher->print);
	if (philosopher->time_to_sleep + philosopher->time_to_eat >= philosopher->time_to_die)
	{
		usleep((philosopher->time_to_die - philosopher->time_to_eat) * 1000);
		return (1);
	}
	usleep(philosopher->time_to_sleep * 1000);
	return (0);
}

int	think(t_philosopher *philosopher)
{
	struct timeval	now;
	long			now_ms;

	if (philosopher->state_philo != THINKING)
	{
		pthread_mutex_lock(philosopher->print);
		if ((*philosopher->philo_died) == 1)
		{
			pthread_mutex_unlock(philosopher->print);
			return (0);
		}
		printf("%ld %d is thinking\n",now_time(philosopher), philosopher->index);
		pthread_mutex_unlock(philosopher->print);
		philosopher->state_philo = THINKING;
		if (philosopher->nb_philo % 2 != 0)
		{
			if (think_sleep(philosopher))
				return (1);
		}
	}
	gettimeofday(&now, NULL);
	now_ms = (((now.tv_sec * 1000) + (now.tv_usec / 1000))) - *(philosopher->time_of_day_start);
	if ((now_ms - philosopher->last_time_eaten) > philosopher->time_to_die)
		return (1);
	usleep(1);
	return (0);
}

void	*die(t_philosopher *philosopher)
{
	pthread_mutex_lock(philosopher->print);
	if ((*philosopher->philo_died) == 1)
	{
		pthread_mutex_unlock(philosopher->print);
		return (0);
	}
	printf("%ld %d died\n",now_time(philosopher), philosopher->index);
	*(philosopher->philo_died) = 1;
	philosopher->state_philo = DEAD;
	pthread_mutex_unlock(philosopher->print);
	return (NULL);
}

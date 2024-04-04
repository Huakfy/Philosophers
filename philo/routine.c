/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjourno <mjourno@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 15:18:17 by mjourno           #+#    #+#             */
/*   Updated: 2023/03/29 15:18:13 by mjourno          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

//Checks if anyoone died
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

//Sets last_time_eaten to start_time and handles basic specific cases
static int	init_routine(t_philosopher *philosopher)
{
	pthread_mutex_lock(philosopher->meal);
	if (check_death(philosopher))
	{
		pthread_mutex_unlock(philosopher->meal);
		return (1);
	}
	philosopher->last_time_eaten = now_time(philosopher);
	pthread_mutex_unlock(philosopher->meal);
	if (philosopher->index % 2 == 1)
		usleep(100);
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

//Routine of a philosopher
//Take forks, eat, put down forks, sleep, think and try not to die
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

//Is used in check_end_threads (threads.c)
//Checks if every philosopher ate enough times in which case philo_died is set
//to 1 in order to stop every thread
int	times_eaten(t_philo *philo)
{
	if (philo->nb_times_to_eat != -1)
	{
		pthread_mutex_lock(philo->meal);
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

//Is used in check_end_threads (threads.c)
//Checks if any philosopher is supposed to die of starvation in which case
//philo_died is set to 1 in order to stop every thread and philo died is printed
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

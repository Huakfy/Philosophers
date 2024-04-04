/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eat_sleep_think.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjourno <mjourno@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 16:55:07 by mjourno           #+#    #+#             */
/*   Updated: 2023/03/29 10:16:46 by mjourno          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

//Eat
//If noone died while waiting at mutex we print philo is eating
//We then increase the number of times this philosopher ate and reset last time
//eaten to now
//Then the philosopher sleeps time_to_eat or less (in case he dies while eating)
int	eat(t_philosopher *philosopher)
{
	long	time;

	pthread_mutex_lock(philosopher->print);
	time = now_time(philosopher);
	if (!check_death(philosopher))
		printf("%ld %d is eating\n", time, philosopher->index);
	pthread_mutex_lock(philosopher->meal);
	philosopher->nb_times_eaten++;
	philosopher->last_time_eaten = time;
	pthread_mutex_unlock(philosopher->meal);
	pthread_mutex_unlock(philosopher->print);
	if (philosopher->time_to_eat >= philosopher->time_to_die)
	{
		usleep((philosopher->time_to_die) * 1000);
		return (1);
	}
	while (now_time(philosopher) < (philosopher->last_time_eaten
			+ philosopher->time_to_eat))
		usleep(100);
	return (0);
}

//Sleep
//If noone died while waiting at mutex we print philo is sleeping
//Then the philosopher sleeps time_to_sleep or less
//(in case he dies while sleeping)
int	philo_sleep(t_philosopher *philosopher)
{
	pthread_mutex_lock(philosopher->print);
	if (!check_death(philosopher))
		printf("%ld %d is sleeping\n", now_time(philosopher),
			philosopher->index);
	pthread_mutex_unlock(philosopher->print);
	if (philosopher->time_to_sleep + philosopher->time_to_eat
		>= philosopher->time_to_die)
	{
		usleep((philosopher->time_to_die - philosopher->time_to_eat) * 1000);
		return (1);
	}
	while (now_time(philosopher) < (philosopher->last_time_eaten
			+ philosopher->time_to_eat + philosopher->time_to_sleep))
		usleep(100);
	return (0);
}

//Think
//If noone died while waiting at mutex we print philo is thinking
//We then sleep ((time_to_die - (time_to_eat + time_to_sleep)) / 2) ms so that
//this philosopher doesn't take the forks right after sleeping
int	think(t_philosopher *philosopher)
{
	pthread_mutex_lock(philosopher->print);
	if (!check_death(philosopher))
		printf("%ld %d is thinking\n", now_time(philosopher),
			philosopher->index);
	pthread_mutex_unlock(philosopher->print);
	usleep(((philosopher->time_to_die - (philosopher->time_to_eat
					+ philosopher->time_to_sleep)) / 2) * 1000);
	return (0);
}

//This function is called in times_eaten (routine.c) but is here for norm
//purposes.It returns 1 when all philosophers ate at least nb_times_to_eat times
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

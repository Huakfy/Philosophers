/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjourno <mjourno@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 17:11:24 by mjourno           #+#    #+#             */
/*   Updated: 2023/03/17 14:13:59 by mjourno          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	think_sleep(t_philosopher *philosopher)
{
	long	now_ms;
	long	verify_time;

	now_ms = now_time(philosopher);
	if (((now_ms - philosopher->last_time_eaten) + philosopher->time_to_eat)
		> philosopher->time_to_die)
	{
		verify_time = philosopher->time_to_die - ((now_ms
			- philosopher->last_time_eaten) + philosopher->time_to_eat);
		if (verify_time >= 0)
		{
			usleep(verify_time * 1000);
			return (1);
		}
		verify_time = philosopher->time_to_die
			- (now_ms - philosopher->last_time_eaten);
		if (verify_time >= 0)
		{
			usleep(verify_time * 1000);
			return (1);
		}
		return (1);
	}
	usleep(philosopher->time_to_eat * 1000);
	return (0);
}

static void	take_fork(t_state *fork, t_philosopher *philosopher)
{
	pthread_mutex_lock(philosopher->print);
	if ((*philosopher->philo_died) == 1)
	{
		pthread_mutex_unlock(philosopher->print);
		return ;
	}
	printf("%ld %d has taken a fork\n",now_time(philosopher),
		philosopher->index);
	*fork = USED;
	pthread_mutex_unlock(philosopher->print);
}

int	forks_available(t_philosopher *philosopher)
{
	if (philosopher->index != philosopher->nb_philo
		&& philosopher->forks[philosopher->index - 1] == AVAILABLE
		&& philosopher->forks[philosopher->index] == AVAILABLE)
	{
		pthread_mutex_lock(
			philosopher->toggle_fork[philosopher->index - 1]);
		pthread_mutex_lock(philosopher->toggle_fork[philosopher->index]);
		take_fork(
			&philosopher->forks[philosopher->index - 1], philosopher);
		take_fork(&philosopher->forks[philosopher->index], philosopher);
		return (1);
	}
	else if (philosopher->index == philosopher->nb_philo
		&& philosopher->forks[philosopher->index - 1] == AVAILABLE
		&& philosopher->forks[0] == AVAILABLE)
	{
		pthread_mutex_lock(
			philosopher->toggle_fork[philosopher->index - 1]);
		pthread_mutex_lock(philosopher->toggle_fork[0]);
		take_fork(
			&philosopher->forks[philosopher->index - 1], philosopher);
		take_fork(&philosopher->forks[0], philosopher);
		return (1);
	}
	return (0);
}

void	put_down_forks(t_philosopher *philosopher)
{
	if (philosopher->index != philosopher->nb_philo)
	{
		philosopher->forks[philosopher->index - 1] = AVAILABLE;
		philosopher->forks[philosopher->index] = AVAILABLE;
		pthread_mutex_unlock(philosopher->toggle_fork[philosopher->index - 1]);
		pthread_mutex_unlock(philosopher->toggle_fork[philosopher->index]);
	}
	else
	{
		philosopher->forks[philosopher->index - 1] = AVAILABLE;
		philosopher->forks[0] = AVAILABLE;
		pthread_mutex_unlock(philosopher->toggle_fork[philosopher->index - 1]);
		pthread_mutex_unlock(philosopher->toggle_fork[0]);
	}
}

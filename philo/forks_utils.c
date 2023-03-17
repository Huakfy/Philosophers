/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjourno <mjourno@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 17:11:24 by mjourno           #+#    #+#             */
/*   Updated: 2023/03/17 17:53:39 by mjourno          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

//This function is only called in the think function of the eat_sleep_think.c
//file and is here for norms purposes.
//It's goal is to make the philosopher wait the appropriate amount of time in
//the case where it should die while waiting for available forks.
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

//This function is only called in forks_available right under.
//We first check if a philospher died while waiting for the mutex in which case
//we don't do anything.
//Else we print philosopher has taken a fork and set the fork's enum as USED.
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

//This function's goal is to check the availability of the philosopher's right
//and left hand side's forks. In my code fork[0] is at the left hand side of
//philosopher N°1 which means that fork[0] is also philosopher N°nb_philo's
//right fork which is why this function is divided in two (thus avoiding
//segfault). If the two forks are available we lock each of their mutexes and
//call take_fork.
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

//After eating and waiting time_to_eat the phiosopher can put the forks down.
//We simply set the forks back to available and unlock each of their mutexes.
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

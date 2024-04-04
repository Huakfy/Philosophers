/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjourno <mjourno@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 17:11:24 by mjourno           #+#    #+#             */
/*   Updated: 2023/03/22 16:20:44 by mjourno          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

//This function is only called in forks_available right under.
//It prints philosopher has taken a fork twice after the philosopher succeeded
//to take both forks
static void	take_forks(t_philosopher *philosopher)
{
	pthread_mutex_lock(philosopher->print);
	if (!check_death(philosopher))
	{
		printf("%ld %d has taken a fork\n", now_time(philosopher),
			philosopher->index);
		printf("%ld %d has taken a fork\n", now_time(philosopher),
			philosopher->index);
	}
	pthread_mutex_unlock(philosopher->print);
}

//After eating and waiting time_to_eat the phiosopher can put the forks down.
//We simply unlock each of their mutexes.
void	put_down_forks(t_philosopher *philosopher)
{
	if (philosopher->index % 2 == 0)
	{
		pthread_mutex_unlock(philosopher->fork[philosopher->index
			% philosopher->nb_philo]);
		pthread_mutex_unlock(philosopher->fork[philosopher->index - 1]);
	}
	else
	{
		pthread_mutex_unlock(philosopher->fork[philosopher->index - 1]);
		pthread_mutex_unlock(philosopher->fork[philosopher->index
			% philosopher->nb_philo]);
	}
}

//This function's goal is to check the availability of the philosopher's right
//and left hand side's forks. So we try lock both of the fork's mutexes and
//then check if, while waiting at the mutex, someone died in which case we don't
//do anything and unlock the mutexes. Otherwise we print philosopher has taken
//a fork twice
int	forks_available(t_philosopher *philosopher)
{
	if (philosopher->index % 2 == 0)
	{
		pthread_mutex_lock(philosopher->fork[philosopher->index
			% philosopher->nb_philo]);
		pthread_mutex_lock(philosopher->fork[philosopher->index - 1]);
		if (check_death(philosopher))
		{
			put_down_forks(philosopher);
			return (1);
		}
		take_forks(philosopher);
	}
	else
	{
		pthread_mutex_lock(philosopher->fork[philosopher->index - 1]);
		pthread_mutex_lock(philosopher->fork[philosopher->index
			% philosopher->nb_philo]);
		if (check_death(philosopher))
		{
			put_down_forks(philosopher);
			return (1);
		}
		take_forks(philosopher);
	}
	return (0);
}

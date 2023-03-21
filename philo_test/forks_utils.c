/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjourno <mjourno@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 17:11:24 by mjourno           #+#    #+#             */
/*   Updated: 2023/03/21 18:06:53 by mjourno          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

//This function is only called in forks_available right under.
//Else we print philosopher has taken a fork and set the fork's enum as USED.
static void	take_fork(t_philosopher *philosopher)
{
	pthread_mutex_lock(philosopher->print);
	printf("%ld %d has taken a fork\n",now_time(philosopher),
		philosopher->index);
	pthread_mutex_unlock(philosopher->print);
}

//After eating and waiting time_to_eat the phiosopher can put the forks down.
//We simply set the forks back to available and unlock each of their mutexes.
void	put_down_forks(t_philosopher *philosopher)
{
	if (philosopher->index % 2 == 0)
	{
		pthread_mutex_unlock(philosopher->fork[philosopher->index % philosopher->nb_philo]);
		pthread_mutex_unlock(philosopher->fork[philosopher->index - 1]);
	}
	else
	{
		pthread_mutex_unlock(philosopher->fork[philosopher->index - 1]);
		pthread_mutex_unlock(philosopher->fork[philosopher->index % philosopher->nb_philo]);
	}
}

//This function's goal is to check the availability of the philosopher's right
//and left hand side's forks. In my code fork[0] is at the left hand side of
//philosopher N°1 which means that fork[0] is also philosopher N°nb_philo's
//right fork which is why this function is divided in two (thus avoiding
//segfault). If the two forks are available we lock each of their mutexes and
//call take_fork.
int	forks_available(t_philosopher *philosopher)
{
	if (philosopher->index % 2 == 0)
	{
		pthread_mutex_lock(philosopher->fork[philosopher->index % philosopher->nb_philo]);
		pthread_mutex_lock(philosopher->fork[philosopher->index - 1]);
		if (check_death(philosopher))
		{
			put_down_forks(philosopher);
			return (1);
		}
		take_fork(philosopher);
		take_fork(philosopher);
		return (0);
	}
	else
	{
		pthread_mutex_lock(philosopher->fork[philosopher->index - 1]);
		pthread_mutex_lock(philosopher->fork[philosopher->index % philosopher->nb_philo]);
		if (check_death(philosopher))
		put_down_forks(philosopher);
		{
			put_down_forks(philosopher);
			return (1);
		}
		take_fork(philosopher);
		take_fork(philosopher);
		return (0);
	}
}

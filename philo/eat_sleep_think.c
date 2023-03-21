/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eat_sleep_think.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjourno <mjourno@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 16:55:07 by mjourno           #+#    #+#             */
/*   Updated: 2023/03/21 11:14:43 by mjourno          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

//Each time a philosopher eats, we check if everyone of them ate at least
//nb_times_to_eat times in which case we set philo_died to 1 to stop every
//thread.
static void	verify_nb_times_eaten(t_philosopher *philosopher)
{
	int	i;

	if (philosopher->nb_times_to_eat == -1)
		return ;
	i = 0;
	while (i < philosopher->nb_philo)
	{
		if (philosopher->philosopher[i]->nb_times_eaten
			< philosopher->nb_times_to_eat)
			return ;
		i++;
	}
	//pthread_mutex_lock(philosopher->print);
	*(philosopher->philo_died) = 1;
	//pthread_mutex_unlock(philosopher->print);
	return ;
}

//Eat
//We first check if a philosopher died while waiting at the mutex then we print
//philospher is thinking in case he is still sleeping (this should not happen
//since I added a new condition in the routine).
//We then print philosopher is eating and wait time_to_eat or less (if
//time_to_eat >= time_to_die) in which case the philosopher dies.
int	eat(t_philosopher *philosopher)
{
	pthread_mutex_lock(philosopher->print);
	if ((*philosopher->philo_died) == 1)
	{
		pthread_mutex_unlock(philosopher->print);
		return (0);
	}
	if (philosopher->state_philo == SLEEPING)
		printf("%ld %d is thinking\n", now_time(philosopher),
			philosopher->index);
	philosopher->state_philo = EATING;
	printf("%ld %d is eating\n", now_time(philosopher), philosopher->index);
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

//Sleep
//In my case sleep always happens after eating (if philosopher didn't die)
//We first check if a philosopher died while waiting at the mutex then we print
//philosopher is sleeping and wait time_to_sleep or less (if time_to_eat +
//time_to_sleep >= time_to_die) in which case the philosopher dies.
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
	if (philosopher->state_philo != THINKING)
	{
		pthread_mutex_lock(philosopher->print);
		if ((*philosopher->philo_died) == 1)
		{
			pthread_mutex_unlock(philosopher->print);
			return (0);
		}
		printf("%ld %d is thinking\n",now_time(philosopher),
			philosopher->index);
		pthread_mutex_unlock(philosopher->print);
		philosopher->state_philo = THINKING;
		if (philosopher->nb_philo % 2 != 0)
		{
			if (think_sleep(philosopher))
				return (1);
		}
	}
	if ((now_time(philosopher) - philosopher->last_time_eaten)
		> philosopher->time_to_die)
		return (1);
	return (0);
}

//Die
//We first verify if an other philosopher didn't die at the same time in which
//case we do not print this philospher's death.
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

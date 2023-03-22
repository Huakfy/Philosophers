/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjourno <mjourno@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 12:14:55 by mjourno           #+#    #+#             */
/*   Updated: 2023/03/22 11:13:44 by mjourno          ###   ########.fr       */
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

static void	*start_routine(void	*arg)
{
	t_philosopher	*philosopher;

	philosopher = (t_philosopher *)arg;
	philosopher->last_time_eaten = now_time(philosopher);
	if (philosopher->nb_times_to_eat == 0)
		return (NULL);
	if (philosopher->nb_philo == 1)
	{

		think(philosopher);
		usleep(philosopher->time_to_die * 1000);
		return(NULL);
	}

	while (1)
	{
		if (check_death(philosopher) || forks_available(philosopher))
		{
			return (NULL);
		}
		if (eat(philosopher))
		{
			put_down_forks(philosopher);
			return (NULL);
		}
		put_down_forks(philosopher);
		if (check_death(philosopher) || philo_sleep(philosopher))
		{
			return (NULL);
		}
		if (check_death(philosopher))
			return (NULL);
		think(philosopher);
	}
	return (NULL);
}

//Initiate all values of each philosopher
static int	init_philosopher_values(t_philo *philo, int i)
{
	philo->philosopher[i]->nb_philo = philo->nb_philo;
	philo->philosopher[i]->time_to_die = philo->time_to_die;
	philo->philosopher[i]->time_to_eat = philo->time_to_eat;
	philo->philosopher[i]->time_to_sleep = philo->time_to_sleep;
	philo->philosopher[i]->nb_times_to_eat = philo->nb_times_to_eat;
	philo->philosopher[i]->time_of_day_start = &philo->time_of_day_start;
	philo->philosopher[i]->print = philo->print;
	philo->philosopher[i]->fork = philo->fork;
	philo->philosopher[i]->meal = philo->meal;
	philo->philosopher[i]->death = philo->death;
	philo->philosopher[i]->index = i + 1;
	philo->philosopher[i]->nb_times_eaten = 0;
	philo->philosopher[i]->state_philo = START;
	philo->philosopher[i]->philo_died = &philo->philo_died;
	philo->philosopher[i]->last_time_eaten = 0;
	return (0);
}

//Lock wathever mutex to wait for every thread to be created.
//Create a philosopher structure for each philosopher, initialize their values.
//Create the threads, initialize start time.
//Wait for every threads to be created and start threads.
int	init_threads(t_philo *philo)
{
	int	i;

	i = 0;
	while (i < philo->nb_philo)
	{
		philo->philosopher[i] = malloc(sizeof(t_philosopher));
		if (!philo->philosopher[i])
		{
			free_philo(philo);
			return (write_error(
				"Error\nPhilosopher structure malloc failed\n"));
		}
		if (init_philosopher_values(philo, i))
			return (1);
		i++;
	}
	init_start_time(philo);

	//start even
	i = 1;
	while (i < philo->nb_philo)
	{
		if (pthread_create(&philo->threads[i], NULL, &start_routine,
			philo->philosopher[i]) == -1)
		{
			free_philo(philo);
			return (write_error("Error\nThread creation failed\n"));
		}
		i += 2;
	}

	usleep(100);

	//start odd
	i = 0;
	while (i < philo->nb_philo)
	{
		if (pthread_create(&philo->threads[i], NULL, &start_routine,
			philo->philosopher[i]) == -1)
		{
			free_philo(philo);
			return (write_error("Error\nThread creation failed\n"));
		}
		i += 2;
	}
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjourno <mjourno@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 12:14:55 by mjourno           #+#    #+#             */
/*   Updated: 2023/03/22 15:49:05 by mjourno          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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

int	start_even(t_philo *philo)
{
	int	i;

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
	return (0);
}

int	start_odd(t_philo *philo)
{
	int	i;

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
			return (write_error("Error\nPhilosopher structure malloc failed\n"));
		}
		if (init_philosopher_values(philo, i))
			return (1);
		i++;
	}
	init_start_time(philo);
	start_even(philo);
	usleep(100);
	start_odd(philo);
	return (0);
}

void	check_end_threads(t_philo *philo)
{
	int		i;

	i = 0;
	while (1)
	{
		if (times_eaten(philo, i))
			return ;
		if (time_death(philo, i))
			return ;
		i++;
		if (i >= (philo->nb_philo - 1))
			i = 0;
		usleep (100);
	}
}

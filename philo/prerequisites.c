/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prerequisites.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjourno <mjourno@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 11:56:27 by mjourno           #+#    #+#             */
/*   Updated: 2023/03/15 15:03:09 by mjourno          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

//Create state array to know current state of each fork (available / used)
static int	init_forks(t_philo *philo)
{
	int	i;

	philo->forks = malloc(sizeof(t_state) * philo->nb_philo);
	if (!philo->forks)
	{
		free_philo(philo);
		return (write_error("Error\nMalloc of forks enum array failed\n"));
	}
	i = 0;
	while (i < philo->nb_philo)
	{
		philo->forks[i] = AVAILABLE;
		i++;
	}
	return (0);
}

//Get current epoch time of start & malloc now
static int	init_start_time(t_philo *philo)
{
	philo->now = malloc(sizeof(struct timeval));
	if (!philo->now)
	{
		free_philo(philo);
		return (write_error("Error\nMalloc of now timeval structure failed\n"));
	}
	philo->time_of_day_start = malloc(sizeof(struct timeval));
	if (!philo->time_of_day_start)
	{
		free_philo(philo);
		return (write_error("Error\nMalloc of start timeval structure failed\n"));
	}
	if (gettimeofday(philo->time_of_day_start, NULL) == -1)
	{
		free_philo(philo);
		return (write_error("Error\ngettimeofday returned error\n"));
	}
	return (0);
}

//Create mutexs
static int	init_mutex(t_philo *philo)
{
	int	i;

	philo->print = malloc(sizeof(pthread_mutex_t));
	if (!philo->print)
	{
		free_philo(philo);
		return (write_error("Error\nPrint mutex malloc failed\n"));
	}
	pthread_mutex_init(philo->print, NULL);
	philo->toggle_fork = malloc(sizeof(pthread_mutex_t *) * philo->nb_philo);
	if (!philo->toggle_fork)
	{
		free_philo(philo);
		return (write_error("Error\nToggle_fork mutex malloc failed\n"));
	}
	i = 0;
	while (i < philo->nb_philo)
	{
		philo->toggle_fork[i] = NULL;
		philo->toggle_fork[i] = malloc(sizeof(pthread_mutex_t));
		if (!philo->toggle_fork[i])
		{
			free_philo(philo);
			return (write_error("Error\nToggle_fork mutex malloc failed\n"));
		}
		pthread_mutex_init(philo->toggle_fork[i], NULL);
		i++;
	}
	return (0);
}

//Create a thread for each philosopher
//Create a philosopher structure for each philosopher
static int	init_threads_and_philosopher(t_philo *philo)
{
	philo->threads = malloc(sizeof(pthread_t) * philo->nb_philo);
	if (!philo->threads)
	{
		free_philo(philo);
		return (write_error("Error\nThreads array malloc failed\n"));
	}
	philo->philosopher = malloc(sizeof(t_philosopher *) * philo->nb_philo);
	if (!philo->philosopher)
	{
		free_philo(philo);
		return (write_error("Error\nPhilosopher array malloc failed\n"));
	}
	return (0);
}

//Initialize every prerequisites
int	init_prerequisites(t_philo *philo)
{
	if (init_forks(philo) || init_mutex(philo)
		|| init_threads_and_philosopher(philo) || init_start_time(philo))
		return (1);
	return (0);
}

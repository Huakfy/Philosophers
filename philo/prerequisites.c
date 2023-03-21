/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prerequisites.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjourno <mjourno@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 11:56:27 by mjourno           #+#    #+#             */
/*   Updated: 2023/03/21 11:52:02 by mjourno          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

//Create state array to know current state of each fork (available / used)
//and itializes them to available
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

//Mallocs and intialize every fork's mutex
static int	init_mutex_fork(t_philo *philo)
{
	int	i;

	i = 0;
	while (i < philo->nb_philo)
	{
		philo->fork[i] = NULL;
		philo->fork[i] = malloc(sizeof(pthread_mutex_t));
		if (!philo->fork[i])
		{
			free_philo(philo);
			return (write_error("Error\nfork mutex malloc failed\n"));
		}
		pthread_mutex_init(philo->fork[i], NULL);
		i++;
	}
	return (0);
}

//Create mutexes
static int	init_mutex(t_philo *philo)
{
	philo->print = malloc(sizeof(pthread_mutex_t));
	if (!philo->print)
	{
		free_philo(philo);
		return (write_error("Error\nPrint mutex malloc failed\n"));
	}
	pthread_mutex_init(philo->print, NULL);
	philo->toggle_fork = malloc(sizeof(pthread_mutex_t));
	if (!philo->toggle_fork)
	{
		free_philo(philo);
		return (write_error("Error\nToggle_fork mutex malloc failed\n"));
	}
	pthread_mutex_init(philo->toggle_fork, NULL);
	philo->fork = malloc(sizeof(pthread_mutex_t *) * philo->nb_philo);
	if (!philo->fork)
	{
		free_philo(philo);
		return (write_error("Error\nfork mutex malloc failed\n"));
	}
	return (init_mutex_fork(philo));
}

//Create a thread for each philosopher
//Create a philosopher structure for each philosopher
static int	init_threads_and_philosopher(t_philo *philo)
{
	int	i;

	philo->threads = malloc(sizeof(pthread_t) * philo->nb_philo);
	if (!philo->threads)
	{
		free_philo(philo);
		return (write_error("Error\nThreads array malloc failed\n"));
	}
	i = -1;
	while (++i < philo->nb_philo)
		philo->threads[i] = 0;
	philo->philosopher = malloc(sizeof(t_philosopher *) * philo->nb_philo);
	if (!philo->philosopher)
	{
		free_philo(philo);
		return (write_error("Error\nPhilosopher array malloc failed\n"));
	}
	i = -1;
	while (++i < philo->nb_philo)
		philo->philosopher[i] = NULL;
	return (0);
}

//Initialize every prerequisites
int	init_prerequisites(t_philo *philo)
{
	if (init_forks(philo) || init_mutex(philo)
		|| init_threads_and_philosopher(philo))
		return (1);
	return (0);
}

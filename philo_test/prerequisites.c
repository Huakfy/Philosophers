/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prerequisites.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjourno <mjourno@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 11:56:27 by mjourno           #+#    #+#             */
/*   Updated: 2023/03/22 15:02:25 by mjourno          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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

static int	init_mutex2(t_philo *philo)
{
	philo->meal = malloc(sizeof(pthread_mutex_t));
	if (!philo->meal)
	{
		free_philo(philo);
		return (write_error("Error\nMeal mutex malloc failed\n"));
	}
	pthread_mutex_init(philo->meal, NULL);
	philo->death = malloc(sizeof(pthread_mutex_t));
	if (!philo->death)
	{
		free_philo(philo);
		return (write_error("Error\nDeath mutex malloc failed\n"));
	}
	pthread_mutex_init(philo->death, NULL);
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
	if (init_mutex2(philo))
		return (1);
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
	if (init_mutex(philo)
		|| init_threads_and_philosopher(philo))
		return (1);
	return (0);
}

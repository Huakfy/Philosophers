/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjourno <mjourno@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 11:32:06 by mjourno           #+#    #+#             */
/*   Updated: 2023/03/22 16:21:46 by mjourno          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

//We first wait for every thread to end before freeing threads pointer
static void	free_threads(t_philo *philo)
{
	int	i;

	i = 0;
	if (philo->threads)
	{
		while (i < philo->nb_philo && philo->threads && philo->threads[i])
		{
			pthread_join(philo->threads[i], NULL);
			i++;
		}
		free(philo->threads);
	}
}

//For norm purposes we need a secon function to free our mutexes.
static void	free_mutexes2(t_philo *philo)
{
	if (philo->meal)
	{
		pthread_mutex_destroy(philo->meal);
		free(philo->meal);
	}
	if (philo->death)
	{
		pthread_mutex_destroy(philo->death);
		free(philo->death);
	}
}

//We destroy and free every mutex as well as fork pointer
static void	free_mutexes(t_philo *philo)
{
	int	i;

	if (philo->print)
	{
		pthread_mutex_destroy(philo->print);
		free(philo->print);
	}
	if (philo->fork)
	{
		i = 0;
		while (i < philo->nb_philo && philo->fork[i])
		{
			pthread_mutex_destroy(philo->fork[i]);
			free(philo->fork[i]);
			i++;
		}
		free(philo->fork);
	}
	free_mutexes2(philo);
}

//This function's goal is to be called at any time in the program (after
//philo has been malloced) and to free anything that has been malloced
void	free_philo(t_philo *philo)
{
	int	i;

	free_threads(philo);
	i = 0;
	while (i < philo->nb_philo && philo->philosopher && philo->philosopher[i])
	{
		free(philo->philosopher[i]);
		i++;
	}
	if (philo->philosopher)
		free(philo->philosopher);
	free_mutexes(philo);
	free(philo);
}

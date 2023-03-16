/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjourno <mjourno@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 11:32:06 by mjourno           #+#    #+#             */
/*   Updated: 2023/03/16 16:05:12 by mjourno          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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

static void	free_mutexes(t_philo *philo)
{
	int	i;

	if (philo->print)
	{
		pthread_mutex_destroy(philo->print);
		free(philo->print);
	}
	if (philo->toggle_fork)
	{
		i = 0;
		while (i < philo->nb_philo && philo->toggle_fork[i])
		{
			pthread_mutex_destroy(philo->toggle_fork[i]);
			free(philo->toggle_fork[i]);
			i++;
		}
		free(philo->toggle_fork);
	}
}

void	free_philo(t_philo *philo)
{
	int	i;

	free_threads(philo);
	if (philo->forks)
		free(philo->forks);
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

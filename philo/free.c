/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjourno <mjourno@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 11:32:06 by mjourno           #+#    #+#             */
/*   Updated: 2023/03/13 16:41:56 by mjourno          ###   ########.fr       */
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
	if (philo->time_of_day_start)
		free(philo->time_of_day_start);
	if (philo->now)
		free(philo->now);
	if (philo->print)
	{
		pthread_mutex_destroy(philo->print);
		free(philo->print);
	}
	if (philo->toggle_fork)
	{
		pthread_mutex_destroy(philo->toggle_fork);
		free(philo->toggle_fork);
	}
	free(philo);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjourno <mjourno@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 13:23:02 by mjourno           #+#    #+#             */
/*   Updated: 2023/03/01 13:38:14 by mjourno          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*start_routine(void	*arg)
{
	t_philo	*philo;

	philo = arg;
	printf("%d\n", philo->nb_philo);
	return (NULL);
}

int	main(int argc, char **argv)
{
	t_philo	*philo;

	philo = malloc(sizeof(t_philo));
	if (!philo)
		return (write_error("Error\nMalloc of philo structure failed\n"));
	if (parsing(argc, argv, philo))
		return (1);
	if (special_cases(philo))
		return (0);

	//Get current epoch time of start
	philo->time_of_day_start = malloc(sizeof(struct timeval));
	if (!philo->time_of_day_start)
		return (write_error("Error\nMalloc of timeval structure failed\n"));
	if (gettimeofday(philo->time_of_day_start, NULL) == -1)
	{
		free(philo->time_of_day_start);
		free(philo);
		return (write_error("Error\ngettimeofday returned error\n"));
	}

	//Create a thread for each philosopher
	philo->threads = malloc(sizeof(pthread_t *) * (philo->nb_philo + 1));
	if (!philo->threads)
	{
		free(philo->time_of_day_start);
		free(philo);
		return (write_error("Error\nThreads array malloc failed\n"));
	}
	philo->threads[philo->nb_philo] = NULL;
	int	i = 0;

	while (i < philo->nb_philo)
	{
		if (pthread_create(philo->threads[i], NULL, &start_routine, philo) == -1)
		{
			free(philo->time_of_day_start);
			free(philo->threads);
			free(philo);
			return (write_error("Error\nThread creation failed\n"));
		}
		i++;
	}

	free(philo->time_of_day_start);
	free(philo->threads);
	free(philo);
	return (0);
}


////exemple utilisation time
//usleep(1000);
//struct timeval	*now;
//now = malloc(sizeof(struct timeval));
//gettimeofday(now, NULL);
//printf("ms:%ld\n",((now->tv_sec - philo->time_of_day_start->tv_sec) * 1000)
//	+ ((now->tv_usec - philo->time_of_day_start->tv_usec) / 1000));
//free(now);
//free(philo->time_of_day_start);

//une action a la fois
//donner un nombre de 1 a n
//assis a cote de maniere circulaire
//pas de communication entre les philospher (entre les threads?)
//eat->sleep->think
//eat: utilisation de 2 fourchettes
//Any state change of a philosopher must be formatted as follows:
// timestamp_in_ms X has taken a fork
// timestamp_in_ms X is eating
// timestamp_in_ms X is sleeping
// timestamp_in_ms X is thinking
// timestamp_in_ms X died

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjourno <mjourno@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 13:23:02 by mjourno           #+#    #+#             */
/*   Updated: 2023/03/13 15:52:47 by mjourno          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
	if (init_prerequisites(philo))
		return (2);
	if (init_threads(philo))
		return (3);

	//Free all
	free_philo(philo);
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

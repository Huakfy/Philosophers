/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjourno <mjourno@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 13:23:02 by mjourno           #+#    #+#             */
/*   Updated: 2023/03/10 11:58:47 by mjourno          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//Revoir l'initialisation des valeurs pour free facilement + essayer chaque erreur

#include "philo.h"

void	*start_routine(void	*arg)
{
	t_philosopher	*philosopher;

	philosopher = (t_philosopher *)arg;
	printf("philosopher N°%d\n", philosopher->index);
	//while (1)
	//{

	//}
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



	int	i;

	//Create state array to know current state of each fork (available / used)
	philo->forks = NULL;
	philo->forks = malloc(sizeof(t_state) * philo->nb_philo);
	if (!philo->forks)
	{
		free(philo);
		return (write_error("Error\nMalloc of forks enumeration array failed\n"));
	}
	i = 0;
	while (i < philo->nb_philo)
	{
		philo->forks[i] = AVAILABLE;
		i++;
	}

	//Get current epoch time of start
	philo->time_of_day_start = malloc(sizeof(struct timeval));
	if (!philo->time_of_day_start)
		return (write_error("Error\nMalloc of timeval structure failed\n"));
	if (gettimeofday(philo->time_of_day_start, NULL) == -1)
	{
		free(philo->time_of_day_start);
		free(philo->forks);
		free(philo);
		return (write_error("Error\ngettimeofday returned error\n"));
	}

	////Create mutexs
	//philo->test = malloc(sizeof(pthread_mutex_t));
	//if (!philo->test)
	//{
	//	free(philo->time_of_day_start);
	//	free(philo);
	//	i = 0;
	//	while (i < philo->nb_philo)
	//	{
	//		pthread_join(philo->threads[i], NULL);
	//		i++;
	//	}
	//	free(philo->threads);
	//	return (write_error("Error\nMutex malloc failed\n"));
	//}
	//pthread_mutex_init(philo->test, NULL);

	//Create a thread for each philosopher
	philo->threads = malloc(sizeof(pthread_t) * philo->nb_philo);
	if (!philo->threads)
	{
		free(philo->time_of_day_start);
		free(philo->forks);
		free(philo);
		return (write_error("Error\nThreads array malloc failed\n"));
	}

	//Create a philosopher structure for each philosopher
	philo->philosopher = malloc(sizeof(t_philosopher *) * philo->nb_philo);
	if (!philo->philosopher)
	{
		free(philo->time_of_day_start);
		free(philo->threads);
		free(philo->forks);
		free(philo);
		return (write_error("Error\nPhilosopher array malloc failed\n"));
	}

	i = 0;
	while (i < philo->nb_philo)
	{
		//Create each philosopher struct
		philo->philosopher[i] = malloc(sizeof(t_philo));
		if (!philo->philosopher[i])
		{
			free(philo->time_of_day_start);
			free(philo->threads);
			i = 0;
			while(i < philo->nb_philo && philo->philosopher[i])
			{
				free(philo->philosopher[i]);
				i++;
			}
			free(philo->forks);
			free(philo);
			return (write_error("Error\nPhilosopher structure malloc failed\n"));
		}
		//Initiate all values
		philo->philosopher[i]->nb_philo = philo->nb_philo;
		philo->philosopher[i]->time_to_die = philo->time_to_die;
		philo->philosopher[i]->time_to_eat = philo->time_to_eat;
		philo->philosopher[i]->time_to_sleep = philo->time_to_sleep;
		philo->philosopher[i]->nb_times_to_eat = philo->nb_times_to_eat;
		philo->philosopher[i]->time_of_day_start = philo->time_of_day_start;
		philo->philosopher[i]->index = i + 1;
		//Initiate each thread
		if (pthread_create(&philo->threads[i], NULL, &start_routine, philo->philosopher[i]) == -1)
		{
			free(philo->threads);
			free(philo->time_of_day_start);
			free(philo->forks);
			free(philo);
			return (write_error("Error\nThread creation failed\n"));
		}
		i++;
	}

	//Free all
	free(philo->forks);

	i = 0;
	while (i < philo->nb_philo)
	{
		pthread_join(philo->threads[i], NULL);
		i++;
	}
	free(philo->threads);

	i = 0;
	while (i < philo->nb_philo)
	{
		free(philo->philosopher[i]);
		i++;
	}
	free(philo->philosopher);

	//free(philo->test);

	free(philo->time_of_day_start);
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

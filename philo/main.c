/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjourno <mjourno@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 13:23:02 by mjourno           #+#    #+#             */
/*   Updated: 2023/02/28 13:11:30 by mjourno          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	not_numeric_chars_only(char **argv)
{
	int	i;
	int	j;

	i = 1;
	while (argv[i])
	{
		j = 0;
		while (argv[i][j])
		{
			if (!('0' <= argv[i][j] && argv[i][j] <= '9'))
				return (1);
			j++;
		}
		if (j == 0)
			return (1);
		i++;
	}
	return (0);
}

static int	ft_atoi(const char *str)
{
	long long int	result;
	int				i;

	i = 0;
	result = 0;
	while ('0' <= str[i] && str[i] <= '9')
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	if (i > 10 || result > 2147483647 || result < 0)
		return (-1);
	return ((int)result);
}

static int	init_values(t_philo	*philo, char **argv)
{
	philo->nb_philo = -1;
	philo->time_to_die = -1;
	philo->time_to_eat = -1;
	philo->time_to_sleep = -1;
	philo->nb_times_to_eat = -1;
	philo->nb_philo = ft_atoi(argv[1]);
	philo->time_to_die = ft_atoi(argv[2]);
	philo->time_to_eat = ft_atoi(argv[3]);
	philo->time_to_sleep = ft_atoi(argv[4]);
	if (philo->nb_philo == -1 || philo->time_to_die == -1
		|| philo->time_to_eat == -1 || philo->time_to_sleep == -1)
		return (1);
	if (argv[5])
	{
		philo->nb_times_to_eat = ft_atoi(argv[5]);
		if (philo->nb_times_to_eat == -1)
			return (1);
	}
	philo->time_of_day_start = NULL;
	return (0);
}

int	main(int argc, char **argv)
{
	t_philo	*philo;

	// parsing
	if (argc < 5 || argc > 6)
		return (write(2, "Error\nMust have 4 or 5 arguments\n", 33));
	if (not_numeric_chars_only(argv))
		return (write(2, "Error\nNon numeric / empty argument\n", 35));
	philo = malloc(sizeof(t_philo));
	if (!philo)
		return (write(2, "Error\nMalloc of philo structure failed\n", 39));
	if (init_values(philo, argv))
	{
		free(philo);
		return (write(2, "Error\nValue bigger than int\n", 28));
	}
	if (philo->nb_philo == 1)
	{
		free(philo);
		return (write(2, "Error\nA philosopher need two forks to eat\n", 42));
	}
	if (philo->nb_times_to_eat == 0)
	{
		free(philo);
		return (0);
	}

	//time
	philo->time_of_day_start = malloc(sizeof(struct timeval));
	if (!philo->time_of_day_start)
		return (write(2, "Error\nMalloc of timeval structure failed\n", 41));
	if (gettimeofday(philo->time_of_day_start, NULL) == -1)
	{
		free(philo->time_of_day_start);
		free(philo);
		return (write(2, "Error\ngettimeofday returned error\n", 34));
	}
	//exemple utilisation time
	usleep(1000);
	struct timeval	*now;
	now = malloc(sizeof(struct timeval));
	gettimeofday(now, NULL);
	printf("ms:%ld\n",((now->tv_sec - philo->time_of_day_start->tv_sec) * 1000)
		+ ((now->tv_usec - philo->time_of_day_start->tv_usec) / 1000));
	free(now);

	free(philo->time_of_day_start);
	free(philo);
	return (0);
}

//autant de forks que de philosopher
//une action a la fois
//donner un nombre de 1 a n
//assis a cote de maniere circulaire
//pas de communication entre les philospher (entre les threads?)
//eat->sleep->think
//eat: utilisation de 2 forks
//Any state change of a philosopher must be formatted as follows:
// timestamp_in_ms X has taken a fork
// timestamp_in_ms X is eating
// timestamp_in_ms X is sleeping
// timestamp_in_ms X is thinking
// timestamp_in_ms X died

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjourno <mjourno@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/01 11:31:35 by mjourno           #+#    #+#             */
/*   Updated: 2023/03/16 15:41:05 by mjourno          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

//Verify if char other than 0-9
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

//Simplified atoi
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

//Init values
static int	init_values(t_philo	*philo, char **argv)
{
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
	philo->time_of_day_start = 0;
	philo->threads = NULL;
	philo->philosopher = NULL;
	philo->forks = NULL;
	philo->print = NULL;
	philo->toggle_fork = NULL;
	philo->philo_died = 0;
	return (0);
}

//Parsing
int	parsing(int argc, char **argv, t_philo *philo)
{
	if (argc < 5 || argc > 6)
	{
		free(philo);
		return (write_error("Error\nMust have 4 or 5 arguments\n"));
	}
	if (not_numeric_chars_only(argv))
	{
		free(philo);
		return (write_error("Error\nNon numeric / empty argument\n"));
	}
	if (init_values(philo, argv))
	{
		free(philo);
		return (write_error("Error\nValue bigger than int\n"));
	}
	if (philo->nb_philo == 0)
	{
		free(philo);
		return (write_error("Error\nNo philosopher\n"));
	}
	return (0);
}

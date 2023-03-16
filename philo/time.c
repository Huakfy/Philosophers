/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjourno <mjourno@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 17:13:54 by mjourno           #+#    #+#             */
/*   Updated: 2023/03/16 17:18:07 by mjourno          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

//Get current epoch time of start
int	init_start_time(t_philo *philo)
{
	struct timeval	start;

	if (gettimeofday(&start, NULL) == -1)
	{
		free_philo(philo);
		return (write_error("Error\ngettimeofday returned error\n"));
	}
	philo->time_of_day_start = (start.tv_sec * 1000) + (start.tv_usec / 1000);
	return (0);
}

long	now_time(t_philosopher *philosopher)
{
	struct timeval	now;

	gettimeofday(&now, NULL);
	return (((now.tv_sec * 1000) + (now.tv_usec / 1000))
		- *(philosopher->time_of_day_start));
}

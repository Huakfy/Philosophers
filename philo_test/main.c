/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjourno <mjourno@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 13:23:02 by mjourno           #+#    #+#             */
/*   Updated: 2023/03/21 17:34:03 by mjourno          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

//Parsing parses every argument, checks for error and initializes philo's
//values to arg or NULL.
//Init_prerequisites mallocs every value of philo.
//Init_threads sets the right value for each element of each philosopher and
//start the threads.
//Finally free philo frees everything that needs to.
int	main(int argc, char **argv)
{
	t_philo	*philo;

	philo = malloc(sizeof(t_philo));
	if (!philo)
		return (write_error("Error\nMalloc of philo structure failed\n"));
	if (parsing(argc, argv, philo))
		return (1);
	if (init_prerequisites(philo))
		return (2);
	if (init_threads(philo))
		return (3);
	check_end_threads(philo);
	free_philo(philo);
	return (0);
}

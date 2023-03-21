/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjourno <mjourno@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 12:14:55 by mjourno           #+#    #+#             */
/*   Updated: 2023/03/21 11:10:19 by mjourno          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

//Only called in start_routine but is here because norm.
//Waits for every thread to be created before starting (mutex).
//Initialize last_time_eaten to the current time.
//Checks if nb_times_to_eat == 0 in which case the thread can already be closed
static int	norm_wait(t_philosopher	*philosopher)
{
	pthread_mutex_lock(philosopher->print);
	pthread_mutex_unlock(philosopher->print);
	philosopher->last_time_eaten = now_time(philosopher);
	if (philosopher->index % 2 == 0)
		usleep(100);
	if (philosopher->nb_times_to_eat == 0)
		return (1);
	return (0);
}

//Only called in start_routine but is here because norm.
//If forks are available eat and put down the forks
//before sleeping.
static int	norm_eat(t_philosopher	*philosopher)
{
	pthread_mutex_lock(philosopher->print);
	if (*(philosopher->philo_died) == 1)
	{
		pthread_mutex_unlock(philosopher->print);
		return (1);
	}
	pthread_mutex_unlock(philosopher->print);
	if (eat(philosopher))
	{
		put_down_forks(philosopher);
		if (*(philosopher->philo_died) == 0)
			die(philosopher);
		return (1);
	}
	put_down_forks(philosopher);
	pthread_mutex_lock(philosopher->print);
	if (*(philosopher->philo_died) == 1)
	{
		pthread_mutex_unlock(philosopher->print);
		return (1);
	}
	pthread_mutex_unlock(philosopher->print);
	if (philo_sleep(philosopher))
	{
		die(philosopher);
		return (1);
	}
	return (0);
}

//Wait for every thread to be created before starting.
//Starts the while true loop. Check everytime for a philosopher's death.
//Think while forks aren't available, else eat and sleep.
static void	*start_routine(void	*arg)
{
	t_philosopher	*philosopher;

	philosopher = (t_philosopher *)arg;
	if (norm_wait(philosopher))
		return (NULL);
	while (1)
	{
		pthread_mutex_lock(philosopher->print);
		if (*(philosopher->philo_died) == 1)
		{
			pthread_mutex_unlock(philosopher->print);
			return (NULL);
		}
		pthread_mutex_unlock(philosopher->print);
		if ((now_time(philosopher) - philosopher->last_time_eaten)
			>= philosopher->time_to_die)
			return (die(philosopher));
		if (philosopher->nb_philo > 1 && (philosopher->state_philo == THINKING
			|| philosopher->state_philo == START)
			&& forks_available(philosopher))
		{
			if (norm_eat(philosopher))
				return (NULL);
		}
		else if (think(philosopher))
			return (die(philosopher));
	}
	return (NULL);
}

//Initiate all values of each philosopher
static int	init_philosopher_values(t_philo *philo, int i)
{
	philo->philosopher[i]->nb_philo = philo->nb_philo;
	philo->philosopher[i]->time_to_die = philo->time_to_die;
	philo->philosopher[i]->time_to_eat = philo->time_to_eat;
	philo->philosopher[i]->time_to_sleep = philo->time_to_sleep;
	philo->philosopher[i]->nb_times_to_eat = philo->nb_times_to_eat;
	philo->philosopher[i]->forks = philo->forks;
	philo->philosopher[i]->time_of_day_start = &philo->time_of_day_start;
	philo->philosopher[i]->print = philo->print;
	philo->philosopher[i]->fork = philo->fork;
	philo->philosopher[i]->toggle_fork = philo->toggle_fork;
	philo->philosopher[i]->index = i + 1;
	philo->philosopher[i]->nb_times_eaten = 0;
	philo->philosopher[i]->state_philo = START;
	philo->philosopher[i]->philo_died = &philo->philo_died;
	philo->philosopher[i]->philosopher = philo->philosopher;
	return (0);
}

//Lock wathever mutex to wait for every thread to be created.
//Create a philosopher structure for each philosopher, initialize their values.
//Create the threads, initialize start time.
//Wait for evzey threads to be created and start threads.
int	init_threads(t_philo *philo)
{
	int	i;

	pthread_mutex_lock(philo->print);
	i = 0;
	while (i < philo->nb_philo)
	{
		philo->philosopher[i] = malloc(sizeof(t_philosopher));
		if (!philo->philosopher[i])
		{
			free_philo(philo);
			return (write_error(
				"Error\nPhilosopher structure malloc failed\n"));
		}
		if (init_philosopher_values(philo, i))
			return (1);
		if (pthread_create(&philo->threads[i], NULL, &start_routine,
			philo->philosopher[i]) == -1)
		{
			free_philo(philo);
			return (write_error("Error\nThread creation failed\n"));
		}
		i++;
	}
	init_start_time(philo);
	pthread_mutex_unlock(philo->print);
	return (0);
}

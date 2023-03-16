/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mjourno <mjourno@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/13 12:14:55 by mjourno           #+#    #+#             */
/*   Updated: 2023/03/16 16:05:32 by mjourno          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static long	now_time(t_philosopher *philosopher)
{
	struct timeval	now;

	gettimeofday(&now, NULL);
	return (((now.tv_sec * 1000) + (now.tv_usec / 1000))
		- *(philosopher->time_of_day_start));
}

static void	*die(t_philosopher *philosopher)
{
	pthread_mutex_lock(philosopher->print);
	if ((*philosopher->philo_died) == 1)
	{
		pthread_mutex_unlock(philosopher->print);
		return (0);
	}
	printf("%ld %d died\n",now_time(philosopher), philosopher->index);
	*(philosopher->philo_died) = 1;
	philosopher->state_philo = DEAD;
	pthread_mutex_unlock(philosopher->print);
	return (NULL);
}

static void	take_fork(t_state *fork, t_philosopher *philosopher)
{
	pthread_mutex_lock(philosopher->print);
	if ((*philosopher->philo_died) == 1)
	{
		pthread_mutex_unlock(philosopher->print);
		return ;
	}
	printf("%ld %d has taken a fork\n",now_time(philosopher), philosopher->index);
	*fork = USED;
	pthread_mutex_unlock(philosopher->print);
}

static void	verify_nb_times_eaten(t_philosopher *philosopher)
{
	int	i;

	if (philosopher->nb_times_to_eat == -1)
		return ;
	i = 0;
	while (i < philosopher->nb_philo)
	{
		if (philosopher->philosopher[i]->nb_times_eaten < philosopher->nb_times_to_eat)
			return ;
		i++;
	}
	*(philosopher->philo_died) = 1;
	return ;
}

static int	eat(t_philosopher *philosopher)
{
	pthread_mutex_lock(philosopher->print);
	if ((*philosopher->philo_died) == 1)
	{
		pthread_mutex_unlock(philosopher->print);
		return (0);
	}
	if (philosopher->state_philo == SLEEPING)
		printf("%ld %d is thinking\n",now_time(philosopher), philosopher->index);
	philosopher->state_philo = EATING;
	printf("%ld %d is eating\n",now_time(philosopher), philosopher->index);
	philosopher->nb_times_eaten++;
	verify_nb_times_eaten(philosopher);
	pthread_mutex_unlock(philosopher->print);

	philosopher->last_time_eaten = now_time(philosopher);
	//will the philosopher die while eating ?
	if (philosopher->time_to_eat >= philosopher->time_to_die)
	{
		usleep((philosopher->time_to_die) * 1000);
		return (1);
	}
	usleep(philosopher->time_to_eat * 1000);
	return (0);
}

static int	forks_available(t_philosopher *philosopher)
{
	if (philosopher->index != philosopher->nb_philo)
	{
		if (philosopher->forks[philosopher->index - 1] == AVAILABLE
			&& philosopher->forks[philosopher->index] == AVAILABLE)
		{
			pthread_mutex_lock(philosopher->toggle_fork[philosopher->index - 1]);
			take_fork(&philosopher->forks[philosopher->index - 1], philosopher);
			pthread_mutex_lock(philosopher->toggle_fork[philosopher->index]);
			take_fork(&philosopher->forks[philosopher->index], philosopher);
			return (1);
		}
	}
	else
	{
		if (philosopher->forks[philosopher->index - 1] == AVAILABLE
			&& philosopher->forks[0] == AVAILABLE)
		{
			pthread_mutex_lock(philosopher->toggle_fork[philosopher->index - 1]);
			take_fork(&philosopher->forks[philosopher->index - 1], philosopher);
			pthread_mutex_lock(philosopher->toggle_fork[0]);
			take_fork(&philosopher->forks[0], philosopher);
			return (1);
		}
	}
	return (0);
}

static void	put_down_forks(t_philosopher *philosopher)
{
	if (philosopher->index != philosopher->nb_philo)
	{
		philosopher->forks[philosopher->index - 1] = AVAILABLE;
		philosopher->forks[philosopher->index] = AVAILABLE;
		pthread_mutex_unlock(philosopher->toggle_fork[philosopher->index - 1]);
		pthread_mutex_unlock(philosopher->toggle_fork[philosopher->index]);
	}
	else
	{
		philosopher->forks[philosopher->index - 1] = AVAILABLE;
		philosopher->forks[0] = AVAILABLE;
		pthread_mutex_unlock(philosopher->toggle_fork[philosopher->index - 1]);
		pthread_mutex_unlock(philosopher->toggle_fork[0]);
	}
}

static int	philo_sleep(t_philosopher *philosopher)
{
	philosopher->state_philo = SLEEPING;
	pthread_mutex_lock(philosopher->print);
	if ((*philosopher->philo_died) == 1)
	{
		pthread_mutex_unlock(philosopher->print);
		return (0);
	}
	printf("%ld %d is sleeping\n",now_time(philosopher), philosopher->index);
	pthread_mutex_unlock(philosopher->print);
	//will the philosopher die while sleeping ?
	if (philosopher->time_to_sleep + philosopher->time_to_eat >= philosopher->time_to_die)
	{
		usleep((philosopher->time_to_die - philosopher->time_to_eat) * 1000);
		return (1);
	}
	usleep(philosopher->time_to_sleep * 1000);
	return (0);
}

static int	think(t_philosopher *philosopher)
{
	int				time_to_think;
	struct timeval	now;
	long			now_ms;

	//time_to_think = philosopher->time_to_eat;
	time_to_think = 1;
	if (philosopher->state_philo != THINKING)
	{
		pthread_mutex_lock(philosopher->print);
		if ((*philosopher->philo_died) == 1)
		{
			pthread_mutex_unlock(philosopher->print);
			return (0);
		}
		printf("%ld %d is thinking\n",now_time(philosopher), philosopher->index);
		pthread_mutex_unlock(philosopher->print);
	}
	gettimeofday(&now, NULL);
	now_ms = ((now.tv_sec * 1000) + (now.tv_usec / 1000)) - *(philosopher->time_of_day_start);
	if ((now_ms - philosopher->last_time_eaten) + time_to_think > philosopher->time_to_die)
	{
		usleep(philosopher->time_to_die - philosopher->last_time_eaten);
		return (1);
	}
	//usleep(time_to_think * 1000);
	usleep(time_to_think);
	philosopher->state_philo = THINKING;
	return (0);
}

static void	*start_routine(void	*arg)
{
	t_philosopher	*philosopher;

	philosopher = (t_philosopher *)arg;
	//init last_time_eaten to start time
	philosopher->last_time_eaten = now_time(philosopher);
	//wait for all threads to be created
	pthread_mutex_lock(philosopher->print);
	pthread_mutex_unlock(philosopher->print);
	if (philosopher->nb_times_to_eat == 0)
		return (NULL);
	while (1)
	{
		//If 2 forks are available take them
		if (*(philosopher->philo_died) == 1)
			return (NULL);
		if (philosopher->nb_philo > 1 && (philosopher->state_philo == THINKING || philosopher->state_philo == START) && forks_available(philosopher))
		{
			//eat
			if (*(philosopher->philo_died) == 1)
				return (NULL);
			if (eat(philosopher))
			{
				put_down_forks(philosopher);
				if (*(philosopher->philo_died) == 0)
					die(philosopher);
				return (NULL);
			}
			put_down_forks(philosopher);
			//verify if dead
			if (*(philosopher->philo_died) == 1)
				return (NULL);
			//sleep
			if (philo_sleep(philosopher))
				return (die(philosopher));
		}
		else
		{
			if (*(philosopher->philo_died) == 1)
				return (NULL);
			//think
			if (think(philosopher))
				return (die(philosopher));
		}
	}
	return (NULL);
}

//Initiate all values
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
	philo->philosopher[i]->toggle_fork = philo->toggle_fork;
	philo->philosopher[i]->index = i + 1;
	philo->philosopher[i]->nb_times_eaten = 0;
	philo->philosopher[i]->state_philo = START;
	philo->philosopher[i]->philo_died = &philo->philo_died;
	philo->philosopher[i]->philosopher = philo->philosopher;
	return (0);
}

//Lock wathever mutex to wait for every thread to be created
//Create a philosopher structure for each philosopher, initialize their values
//Create the threads
//Initialize start time
//Wait for evzey threads to be created and start threads
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
			return (write_error("Error\nPhilosopher structure malloc failed\n"));
		}
		if (init_philosopher_values(philo, i))
			return (1);
		if (pthread_create(&philo->threads[i], NULL, &start_routine, philo->philosopher[i]) == -1)
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

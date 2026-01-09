/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inits.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maria-ol <maria-ol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/07 13:45:44 by mona              #+#    #+#             */
/*   Updated: 2026/01/09 16:05:22 by maria-ol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/**
 * @brief Initialize the shared data structure.
 *
 * This function parses command-line arguments and initializes the
 * shared data structure with simulation parameters. It sets the
 * number of philosophers, timing values (time_to_die, time_to_eat,
 * time_to_sleep), and optionally the minimum number of meals each
 * philosopher must eat. It also initializes control flags and sets
 * pointer fields to NULL.
 *
 * @param data Pointer to the data structure to be initialized.
 * @param ac Number of command-line arguments.
 * @param av Array of command-line argument strings.
 * @return Always returns 0.
 */
int	init_data(t_data *data, int ac, char **av)
{
	data->num_philos = ft_atoi(av[1]);
	data->time_to_die = ft_atoi(av[2]);
	data->time_to_eat = ft_atoi(av[3]);
	data->time_to_sleep = ft_atoi(av[4]);
	if (ac == 6)
		data->num_must_eat = ft_atoi(av[5]);
	else
		data->num_must_eat = -1;
	data->someone_died = false;
	data->all_ate_enough = false;
	data->philos = NULL;
	data->forks = NULL;
	return (0);
}

/**
 * @brief Initialize all mutexes for the simulation.
 *
 * This function creates and initializes all mutexes required for the
 * simulation. It initializes three global mutexes (print_mutex,
 * death_mutex, meal_mutex) and allocates an array of fork mutexes,
 * one for each philosopher. Each fork mutex is then initialized.
 * Returns an error code if any initialization fails.
 *
 * @param data Pointer to the shared data structure where mutexes
 *             will be initialized.
 * @return 0 on success, 1 on failure.
 */
int	init_mutexes(t_data *data)
{
	int	i;

	if (pthread_mutex_init(&data->print_mutex, NULL))
		return (handle_error(ERR_INIT_GMUTEX));
	if (pthread_mutex_init(&data->death_mutex, NULL))
		return (handle_error(ERR_INIT_GMUTEX));
	if (pthread_mutex_init(&data->meal_mutex, NULL))
		return (handle_error(ERR_INIT_GMUTEX));
	data->forks = malloc(sizeof(pthread_mutex_t) * data->num_philos);
	if (!data->forks)
		return (handle_error(ERR_ALOC));
	i = 0;
	while (i < data->num_philos)
	{
		if (pthread_mutex_init(&data->forks[i], NULL))
			return (handle_error(ERR_INIT_FMUTEX));
		i++;
	}
	return (0);
}

/**
 * @brief Initialize all philosopher structures.
 *
 * This function allocates memory for the philosophers array and
 * initializes each philosopher structure. It assigns a unique ID,
 * sets initial meal count to 0, assigns left and right fork pointers
 * using circular indexing, and links each philosopher to the shared
 * data structure. The right fork uses modulo arithmetic to wrap
 * around for the last philosopher.
 *
 * @param data Pointer to the shared data structure containing
 *             philosopher array to be initialized.
 * @return 0 on success, 1 on failure.
 */
int	init_philos(t_data *data)
{
	int	i;

	data->philos = malloc(sizeof(t_philo) * data->num_philos);
	if (!data->philos)
		return (handle_error(ERR_ALOC));
	i = 0;
	while (i < data->num_philos)
	{
		data->philos[i].id = i + 1;
		data->philos[i].meals_eaten = 0;
		data->philos[i].last_meal_time = 0;
		data->philos[i].left_fork = &data->forks[i];
		data->philos[i].right_fork = &data->forks[(i + 1) % data->num_philos];
		data->philos[i].data = data;
		i++;
	}
	return (0);
}

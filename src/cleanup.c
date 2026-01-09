/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maria-ol <maria-ol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/07 13:47:10 by mona              #+#    #+#             */
/*   Updated: 2026/01/09 15:51:57 by maria-ol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/**
 * @brief Destroy all mutexes used in the simulation.
 *
 * This function destroys all mutexes that were initialized during the
 * simulation setup. It destroys the global mutexes (print_mutex,
 * death_mutex, meal_mutex) and all fork mutexes. The fork mutexes
 * are only destroyed if the forks array was successfully allocated.
 *
 * @param data Pointer to the shared data structure containing all
 *             mutexes to be destroyed.
 */
void	destroy_mutexes(t_data *data)
{
	int	i;

	pthread_mutex_destroy(&data->print_mutex);
	pthread_mutex_destroy(&data->death_mutex);
	pthread_mutex_destroy(&data->meal_mutex);
	if (data->forks)
	{
		i = 0;
		while (i < data->num_philos)
		{
			pthread_mutex_destroy(&data->forks[i]);
			i++;
		}
	}
}

/**
 * @brief Clean up all resources allocated during the simulation.
 *
 * This function performs a complete cleanup of all resources that were
 * allocated during the simulation. It destroys all mutexes and frees
 * all dynamically allocated memory (forks and philosophers arrays).
 * It should be called before the program exits to prevent memory
 * leaks and ensure proper resource deallocation.
 *
 * @param data Pointer to the shared data structure containing all
 *             resources to be freed.
 */
void	cleanup(t_data *data)
{
	destroy_mutexes(data);
	if (data->forks)
		free(data->forks);
	if (data->philos)
		free(data->philos);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maria-ol <maria-ol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 16:38:34 by maria-ol          #+#    #+#             */
/*   Updated: 2026/01/13 14:30:46 by maria-ol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/**
 * @brief Handle the special case of a single philosopher.
 *
 * When there is only one philosopher, they can only take one fork
 * and will inevitably die. This function prints the fork taken
 * message and waits for the time_to_die duration.
 *
 * @param philo Pointer to the philosopher structure.
 * @return Always returns NULL.
 */
static void	*one_philo_routine(t_philo *philo)
{
	print_status(philo, "has taken a fork");
	precise_sleep(philo->data->time_to_die, philo->data);
	return (NULL);
}

/**
 * @brief Main routine executed by each philosopher thread.
 *
 * This function implements the main lifecycle of a philosopher.
 * It handles the special case of a single philosopher, introduces
 * a small delay for even-numbered philosophers to reduce initial
 * contention, and then enters an infinite loop where the philosopher
 * repeatedly eats, sleeps, and thinks until someone dies. The loop
 * checks the death flag before each cycle to exit gracefully.
 *
 * @param arg Pointer to the philosopher structure cast as void*.
 * @return Always returns NULL when the routine finishes.
 */
void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->data->num_philos == 1)
		return (one_philo_routine(philo));
	if (philo->id % 2 == 0)
		usleep(1000);
	while (1)
	{
		pthread_mutex_lock(&philo->data->death_mutex);
		if (philo->data->someone_died == true
			|| philo->data->all_ate_enough == true)
		{
			pthread_mutex_unlock(&philo->data->death_mutex);
			break ;
		}
		pthread_mutex_unlock(&philo->data->death_mutex);
		philo_eat(philo);
		philo_sleep(philo);
		philo_think(philo);
	}
	return (NULL);
}

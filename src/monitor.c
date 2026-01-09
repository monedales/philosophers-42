/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maria-ol <maria-ol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 15:46:13 by maria-ol          #+#    #+#             */
/*   Updated: 2026/01/09 15:46:54 by maria-ol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/**
 * @brief Print philosopher status in a thread-safe manner.
 *
 * This function prints the current status of a philosopher with a
 * timestamp relative to the simulation start time. It ensures
 * thread-safety by using mutexes to protect both the death check
 * and the printing operation. If someone has already died, the
 * function returns without printing to avoid output after death.
 *
 * @param philo Pointer to the philosopher structure whose status is
 *              being printed.
 * @param status String describing the philosopher's current action
 *               (e.g., "is eating", "is sleeping", "is thinking",
 *               "has taken a fork", "died").
 */
void	print_status(t_philo *philo, char *status)
{
	long	timestamp;

	pthread_mutex_lock(&philo->data->death_mutex);
	if (philo->data->someone_died)
	{
		pthread_mutex_unlock(&philo->data->death_mutex);
		return ;
	}
	pthread_mutex_unlock(&philo->data->death_mutex);
	pthread_mutex_lock(&philo->data->print_mutex);
	timestamp = get_time() - philo->data->start_time;
	printf("%ld %d %s\n", timestamp, philo->id, status);
	pthread_mutex_unlock(&philo->data->print_mutex);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mona <mona@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 16:40:28 by maria-ol          #+#    #+#             */
/*   Updated: 2026/01/11 18:28:38 by mona             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/**
 * @brief Take forks in the correct order to prevent deadlock.
 *
 * Philosophers with even IDs take right fork first, odd IDs take
 * left fork first. A small delay after taking forks helps prevent
 * starvation in edge cases.
 *
 * @param philo Pointer to the philosopher structure.
 */
static void	take_forks(t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(philo->right_fork);
		print_status(philo, "has taken a fork");
		pthread_mutex_lock(philo->left_fork);
		print_status(philo, "has taken a fork");
		usleep(100);
	}
	else
	{
		pthread_mutex_lock(philo->left_fork);
		print_status(philo, "has taken a fork");
		pthread_mutex_lock(philo->right_fork);
		print_status(philo, "has taken a fork");
		usleep(1);
	}
}

/**
 * @brief Release forks in reverse order.
 *
 * @param philo Pointer to the philosopher structure.
 */
static void	release_forks(t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
	}
	else
	{
		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(philo->left_fork);
	}
}

/**
 * @brief Philosopher eating action.
 *
 * This function implements the eating action for a philosopher.
 * The philosopher attempts to pick up the left and right forks
 * (mutexes) in a specific order based on their ID to prevent
 * deadlock. After acquiring both forks, the philosopher updates
 * their last meal time and increments their meal count. The
 * philosopher then sleeps for the duration of eating before
 * releasing the forks.
 *
 * @param philo Pointer to the philosopher structure performing
 *              the eating action.
 */
void	philo_eat(t_philo *philo)
{
	take_forks(philo);
	print_status(philo, "is eating");
	pthread_mutex_lock(&philo->data->meal_mutex);
	philo->last_meal_time = get_time();
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->data->meal_mutex);
	precise_sleep(philo->data->time_to_eat, philo->data);
	release_forks(philo);
}

/**
 * @brief Philosopher sleeping action.
 *
 * This function implements the sleeping action for a philosopher.
 * The philosopher prints their sleeping status and then sleeps for
 * the specified duration (time_to_sleep). The precise_sleep function
 * is used to allow early termination if a philosopher dies.
 *
 * @param philo Pointer to the philosopher structure performing
 *              the sleeping action.
 */
void	philo_sleep(t_philo *philo)
{
	print_status(philo, "is sleeping");
	precise_sleep(philo->data->time_to_sleep, philo->data);
}

/**
 * @brief Philosopher thinking action.
 *
 * This function implements the thinking action for a philosopher.
 * For odd numbers of philosophers, a small thinking delay is added
 * to prevent starvation.
 *
 * @param philo Pointer to the philosopher structure performing
 *              the thinking action.
 */
void	philo_think(t_philo *philo)
{
	long	think_time;

	print_status(philo, "is thinking");
	if (philo->data->num_philos % 2 != 0)
	{
		think_time = philo->data->time_to_eat * 2 - philo->data->time_to_sleep;
		if (think_time < 0)
			think_time = 0;
		if (think_time > 600)
			think_time = 200;
		if (think_time > 0)
			usleep(think_time * 1000);
	}
}

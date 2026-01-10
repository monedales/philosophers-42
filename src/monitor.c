/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maria-ol <maria-ol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 15:46:13 by maria-ol          #+#    #+#             */
/*   Updated: 2026/01/09 20:44:16 by maria-ol         ###   ########.fr       */
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

/**
 * @brief Check if any philosopher has died from starvation.
 *
 * This function iterates through all philosophers and checks if any
 * has exceeded the time_to_die limit since their last meal. If a
 * philosopher is found to have died, it sets the someone_died flag,
 * prints the death message with proper timestamp, and returns true.
 * Access to shared data is protected by appropriate mutexes.
 *
 * @param data Pointer to the shared data structure.
 * @return true if a philosopher has died, false otherwise.
 */
bool	check_death(t_data *data)
{
	long	last_meal;
	long	current_time;
	long	time_since_meal;
	int		i;

	i = 0;
	while (i < data->num_philos)
	{
		current_time = get_time();
		pthread_mutex_lock(&data->meal_mutex);
		last_meal = data->philos[i].last_meal_time;
		pthread_mutex_unlock(&data->meal_mutex);
		time_since_meal = current_time - last_meal;
		if (time_since_meal >= data->time_to_die)
		{
			pthread_mutex_lock(&data->death_mutex);
			data->someone_died = true;
			pthread_mutex_unlock(&data->death_mutex);
			pthread_mutex_lock(&data->print_mutex);
			printf("%ld %d died\n", current_time - data->start_time,
				data->philos[i].id);
			pthread_mutex_unlock(&data->print_mutex);
			return (true);
		}
		i++;
	}
	return (false);
}

/**
 * @brief Check if all philosophers have eaten enough times.
 *
 * This function verifies if all philosophers have reached the
 * required number of meals (num_must_eat). If num_must_eat is -1,
 * the function returns false immediately as there is no meal limit.
 * If all philosophers have eaten the required amount, it sets the
 * all_ate_enough flag and returns true.
 *
 * @param data Pointer to the shared data structure.
 * @return true if all philosophers ate enough, false otherwise.
 */
bool	check_all_ate(t_data *data)
{
	int		i;
	int		count;
	int		meals;

	if (data->num_must_eat == -1)
		return (false);
	i = 0;
	count = 0;
	while (i < data->num_philos)
	{
		pthread_mutex_lock(&data->meal_mutex);
		meals = data->philos[i].meals_eaten;
		pthread_mutex_unlock(&data->meal_mutex);
		if (meals >= data->num_must_eat)
			count++;
		i++;
	}
	if (count == data->num_philos)
	{
		pthread_mutex_lock(&data->death_mutex);
		data->all_ate_enough = true;
		pthread_mutex_unlock(&data->death_mutex);
		return (true);
	}
	return (false);
}

/**
 * @brief Monitor routine to check for death and completion.
 *
 * This function runs in a separate thread and continuously monitors
 * the simulation state. It checks if any philosopher has died from
 * starvation or if all philosophers have eaten the required number
 * of meals. The loop exits when either condition is met. Small
 * delays are introduced to reduce CPU usage during monitoring.
 *
 * @param arg Pointer to the shared data structure cast as void*.
 * @return Always returns NULL when monitoring ends.
 */
void	*monitor_routine(void *arg)
{
	t_data	*data;

	data = (t_data *)arg;
	usleep(1000);
	while (1)
	{
		if (check_death(data) == true)
			break ;
		if (check_all_ate(data) == true)
			break ;
		usleep(1000);
	}
	return (NULL);
}

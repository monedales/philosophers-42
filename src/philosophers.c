/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mona <mona@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/07 13:45:24 by mona              #+#    #+#             */
/*   Updated: 2026/01/11 18:33:48 by mona             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/**
 * @brief Validate command-line arguments.
 *
 * This function checks if the correct number of arguments was provided
 * (between 5 and 6) and ensures all numeric arguments are positive
 * integers. The expected format is: ./philo number_of_philosophers
 * time_to_die time_to_eat time_to_sleep [number_of_times_each_
 * philosopher_must_eat].
 *
 * @param argc Number of command-line arguments.
 * @param argv Array of command-line argument strings.
 * @return 0 on success, 1 on failure.
 */
static int	validate_args(int argc, char **argv)
{
	int	i;

	if (argc < 5 || argc > 6)
		return (handle_error(ERR_ARGS));
	i = 1;
	while (i < argc)
	{
		if (ft_atoi(argv[i]) <= 0)
			return (handle_error(ERR_POSITIVE));
		i++;
	}
	return (0);
}

/**
 * @brief Start the philosophers simulation.
 *
 * This function initializes the simulation start time, creates a
 * thread for each philosopher, and creates a monitor thread to check
 * for death or completion conditions. It then waits for all threads
 * to finish before returning. Each philosopher's last_meal_time is
 * initialized to the simulation start time.
 *
 * @param data Pointer to the shared data structure containing all
 *             simulation parameters and philosopher information.
 * @return 0 on success, 1 on failure.
 */
static int	start_simulation(t_data *data)
{
	int			i;
	pthread_t	monitor;

	data->start_time = get_time();
	pthread_mutex_lock(&data->meal_mutex);
	i = 0;
	while (i < data->num_philos)
	{
		data->philos[i].last_meal_time = data->start_time;
		i++;
	}
	pthread_mutex_unlock(&data->meal_mutex);
	i = 0;
	while (i < data->num_philos)
	{
		if (pthread_create(&data->philos[i].thread, NULL,
				philo_routine, &data->philos[i]))
			return (handle_error(ERR_PHILO_THREAD));
		i++;
	}
	if (pthread_create(&monitor, NULL, monitor_routine, data))
		return (handle_error(ERR_MONIT_THREAD));
	i = 0;
	while (i < data->num_philos)
	{
		pthread_join(data->philos[i].thread, NULL);
		i++;
	}
	pthread_join(monitor, NULL);
	return (0);
}

/**
 * @brief Main coordinator function for the philosophers simulation.
 *
 * This function implements the main flow of the philosophers
 * simulation. It validates arguments, initializes the shared data
 * structure, mutexes, and philosophers, then starts the simulation.
 * All resources are properly cleaned up before returning, regardless
 * of success or failure.
 *
 * @param argc Number of command-line arguments.
 * @param argv Array of command-line argument strings.
 * @return 0 on success, 1 on failure.
 */
int	main(int argc, char **argv)
{
	t_data	data;

	if (validate_args(argc, argv))
		return (1);
	if (init_data(&data, argc, argv))
		return (1);
	if (init_mutexes(&data))
	{
		cleanup(&data);
		return (1);
	}
	if (init_philos(&data))
	{
		cleanup(&data);
		return (1);
	}
	if (start_simulation(&data))
	{
		cleanup(&data);
		return (1);
	}
	cleanup(&data);
	return (0);
}

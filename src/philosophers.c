/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mona <mona@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/07 13:45:24 by mona              #+#    #+#             */
/*   Updated: 2026/01/07 15:06:39 by mona             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static int	validate_args(int ac, char **av)
{
	int	i;

	if (ac < 5 || ac > 6)
		return (handle_error(ERR_ARGS));
	i = 1;
	while (i < ac)
	{
		if (ft_atoi(av[i]) <= 0)
			return (handle_error(ERR_POSITIVE));
		i++;
	}
	return (0);
}

static int	start_simulation(t_data *data)
{
	int			i;
	pthread_t	monitor;

	i = 0;
	data->start_time = get_time();
	while (i < data->num_philos)
	{
		data->philos[i].last_meal_time = data->start_time;
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

int	main(int ac, char **av)
{
	t_data	data;

	if (validate_args(ac, av))
		return (1);
	if (init_data(&data, ac, av))
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

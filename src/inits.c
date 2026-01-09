/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inits.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mona <mona@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/07 13:45:44 by mona              #+#    #+#             */
/*   Updated: 2026/01/07 15:06:37 by mona             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

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

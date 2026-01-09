/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maria-ol <maria-ol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 15:42:14 by maria-ol          #+#    #+#             */
/*   Updated: 2026/01/09 16:08:06 by maria-ol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>
# include <stdbool.h>

typedef enum e_error
{
	ERR_ARGS = 1,
	ERR_POSITIVE,
	ERR_INIT_GMUTEX,
	ERR_INIT_FMUTEX,
	ERR_ALOC,
	ERR_NOEAT,
	ERR_PHILO_THREAD,
	ERR_MONIT_THREAD,
	ERR_NO_LONG
}				t_error;

typedef struct s_data	t_data;

typedef struct s_philo
{
	int				id;
	int				meals_eaten;
	long			last_meal_time;
	pthread_t		thread;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	t_data			*data;
}	t_philo;

typedef struct s_data
{
	int				num_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				num_must_eat;
	long			start_time;
	bool			someone_died;
	bool			all_ate_enough;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	death_mutex;
	pthread_mutex_t	meal_mutex;
	pthread_mutex_t	*forks;
	t_philo			*philos;
}	t_data;

// Error handling
int		handle_error(t_error error);

// Initialization
int		init_data(t_data *data, int ac, char **av);
int		init_mutexes(t_data *data);
int		init_philos(t_data *data);

// Utils
long	get_time(void);
void	precise_sleep(long milliseconds, t_data *data);
int		ft_atoi(const char *str);
void	print_status(t_philo *philo, char *status);

// Routine
// void	*philo_routine(void *arg);

// Actions
// void	philo_eat(t_philo *philo);
// void	philo_sleep(t_philo *philo);
// void	philo_think(t_philo *philo);

// Monitor
// void	*monitor_routine(void *arg);
// bool	check_death(t_data *data);
// bool	check_all_ate(t_data *data);

// Cleanup
void	cleanup(t_data *data);
void	destroy_mutexes(t_data *data);

#endif
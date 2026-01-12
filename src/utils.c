/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mona <mona@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/07 13:47:13 by mona              #+#    #+#             */
/*   Updated: 2026/01/12 19:11:18 by mona             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/**
 * @brief Get current time in milliseconds.
 *
 * This function retrieves the current time using gettimeofday and
 * converts it to milliseconds. The returned value represents the
 * number of milliseconds since the Unix epoch (January 1, 1970).
 *
 * @return Current time in milliseconds as a long integer.
 */
long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000 + tv.tv_usec / 1000));
}

/**
 * @brief Precise sleep with death check.
 *
 * This function implements a precise sleep mechanism that continuously
 * checks if any philosopher has died during the sleep period. It uses
 * small intervals of usleep (500 microseconds) to maintain
 * responsiveness while sleeping for the specified duration. The
 * function will exit early if someone_died flag is set to true.
 *
 * @param milliseconds The duration to sleep in milliseconds.
 * @param data Pointer to the shared data structure containing
 *             death_mutex and someone_died flag.
 */
void	precise_sleep(long milliseconds, t_data *data)
{
	long	start;
	long	current;

	start = get_time();
	while (1)
	{
		pthread_mutex_lock(&data->death_mutex);
		if (data->someone_died)
		{
			pthread_mutex_unlock(&data->death_mutex);
			break ;
		}
		pthread_mutex_unlock(&data->death_mutex);
		current = get_time();
		if (current - start >= milliseconds)
			break ;
		usleep(500);
	}
}

/**
 * @brief Handle and display error messages.
 *
 * This function takes an error code from the t_error enum and displays
 * the corresponding error message to the standard output. It uses a
 * static array of error messages indexed by the error code. The
 * function always returns 1 to indicate an error occurred.
 *
 * @param error The error code from the t_error enum indicating the
 *              type of error.
 * @return Always returns 1 to indicate an error condition.
 */
int	handle_error(t_error error)
{
	static const char	*messages[] = {
		NULL,
		"Error\nInvalid number of arguments\n",
		"Error\nArgument must be numeric only\n",
		"Error\nArgument must be a positive integer\n",
		"Error\nValue exceeds maximum (2147483647)\n",
		"Error\nOne philosopher cannot eat (needs at least 2)\n",
		"Error\nFailed to initialize global mutex\n",
		"Error\nFailed to initialize fork mutex\n",
		"Error\nMemory allocation failed\n",
		"Error\nFailed to create philosopher thread\n",
		"Error\nFailed to create monitor thread\n"
	};

	if (error > 0 && error < (int)(sizeof(messages) / sizeof(messages[0])))
	{
		printf("%s", messages[error]);
	}
	return (1);
}

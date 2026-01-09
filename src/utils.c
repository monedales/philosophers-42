/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maria-ol <maria-ol@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/07 13:47:13 by mona              #+#    #+#             */
/*   Updated: 2026/01/09 15:46:59 by maria-ol         ###   ########.fr       */
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
		"Error\ninvalid arguments\n",
		"Error\nit must be a positive int number!\n",
		"Error\nfailed to init global mutex\n",
		"Error\nfail to init fork mutex\n",
		"Error\nalocation with malloc failed\n",
		"Error\nfailed to feed philo\n",
		"Error\nwhile creating philo thread\n",
		"Error\nwhile creating monitor thread\n",
		"Error\nThe value cannot be higher than 2147483647"
	};

	if (error > 0 && error < (int)(sizeof(messages) / sizeof(messages[0])))
	{
		printf("%s", messages[error]);
	}
	return (1);
}

/**
 * @brief Check if a character is a whitespace.
 *
 * This function returns 1 if the given character is a whitespace
 * character (space, tab, newline, vertical tab, form feed, or
 * carriage return). Otherwise, it returns 0.
 *
 * @param chr The character to check.
 * @return 1 if the character is a whitespace, 0 otherwise.
 */
static int	ft_isspace(int chr)
{
	if ((chr >= 9 && chr <= 13) || chr == 32)
		return (1);
	return (0);
}

/**
 * @brief Convert a string to a integer
 *
 * This function converts the initial portion of the string pointed to
 * by 'str' to int. The beggining of 'str' may contain whitespace
 * characters followed by a single optional '+' or '-' sign. After a
 * digit is found the conversion continues until a non-digit character
 * is encountered.
 *
 * @param str Pointer to a string containing a representation of a
 *            whole number.
 * @return An int value containing the converted 'str' or 0 on error.
 */
int	ft_atoi(const char *str)
{
	int	count;
	int	result;
	int	sign;

	count = 0;
	result = 0;
	sign = 1;
	while (ft_isspace(str[count]) == 1)
		count++ ;
	if (str[count] == '-' || str[count] == '+')
	{
		if (str[count] == '-')
			sign = -sign;
		count++;
	}
	while (str[count] && str[count] >= 48 && str[count] <= 57)
	{
		result = result * 10 + (str[count] - 48);
		count++;
	}
	return (result * sign);
}

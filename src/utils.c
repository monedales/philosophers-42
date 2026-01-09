/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mona <mona@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/07 13:47:13 by mona              #+#    #+#             */
/*   Updated: 2026/01/07 15:10:41 by mona             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000 + tv.tv_usec / 1000));
}

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
 * This function returns 1 if the given character is a whitespace character 
 * (space, tab, newline, vertical tab, form feed, or carriage return).
 * Otherwise, it returns 0.
 *
 * @param chr The character to check.
 * @return 1 if the character is a whitespace, 0 otherwise.
 */
int	ft_isspace(int chr)
{
	if ((chr >= 9 && chr <= 13) || chr == 32)
		return (1);
	return (0);
}

/**
 * @brief Convert a string to a integer
 *
 * This function converts the initial portion of the string pointed to by 'str' 
 * to int. The beggining of 'str' may contain whitespace characters followed by
 * a single optional '+' or '-' sign. After a digit is found the conversion 
 * continues until a non-digit character is encountered.
 * 
 *
 * @param str Pointer to a string containing a representation of a whole number.
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


/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mona <mona@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/12 19:07:08 by mona              #+#    #+#             */
/*   Updated: 2026/01/12 19:17:12 by mona             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/**
 * @brief Check if a string contains only digits.
 *
 * This function validates if the given string contains only numeric
 * digits (0-9), with optional leading whitespace and a single '+' sign.
 * This ensures that conversions with ft_atoi are safe and don't accept
 * partial conversions like "200A".
 *
 * @param str The string to validate.
 * @return 1 if the string is a valid positive number, 0 otherwise.
 */
int	is_valid_number(const char *str)
{
	int	i;

	i = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '+')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
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
 * @brief Convert a string to a long integer
 *
 * This function converts the initial portion of the string pointed to
 * by 'str' to long. Used for overflow detection before converting to int.
 *
 * @param str Pointer to a string containing a representation of a number.
 * @return A long value containing the converted 'str' or 0 on error.
 */
long	ft_atol(const char *str)
{
	int		count;
	long	result;
	int		sign;

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

/**
 * @brief Convert a string to a integer
 *
 * This function converts the initial portion of the string pointed to
 * by 'str' to int by calling ft_atol and casting the result.
 *
 * @param str Pointer to a string containing a representation of a
 *            whole number.
 * @return An int value containing the converted 'str' or 0 on error.
 */
int	ft_atoi(const char *str)
{
	return ((int)ft_atol(str));
}

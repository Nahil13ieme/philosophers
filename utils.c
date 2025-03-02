/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbenhami <nbenhami@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 13:18:17 by nbenhami          #+#    #+#             */
/*   Updated: 2025/03/02 18:24:00 by nbenhami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* Returns current time in milliseconds */
long	get_time_in_ms(void)
{
	struct timeval	tv;
	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000L + tv.tv_usec / 1000);
}

/* Sleeps approximately ms milliseconds */
void	sleep_ms(long ms)
{
	usleep(ms * 1000);
}

int	ft_atoi(char *str)
{
	int	result;
	int	sign;

	result = 0;
	sign = 1;
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		result = result * 10 + (*str - '0');
		str++;
	}
	return (result * sign);
}

void	print_message(t_philo *philo, char *message)
{
	pthread_mutex_lock(&philo->sim->print_mutex);
	printf("%ld %d : %s\n", get_time_in_ms() - philo->sim->start_time , philo->id, message);
	pthread_mutex_unlock(&philo->sim->print_mutex);
}

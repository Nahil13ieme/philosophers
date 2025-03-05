/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbenhami <nbenhami@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 13:18:17 by nbenhami          #+#    #+#             */
/*   Updated: 2025/03/05 17:58:44 by nbenhami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* Returns current time in milliseconds */
long	get_time_in_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000L) + (tv.tv_usec / 1000));
}

/* Sleeps approximately ms milliseconds */
void	sleep_ms(long ms)
{
	long start = get_time_in_ms();
    while (get_time_in_ms() - start < ms)
        usleep(100);
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

void	print_message(t_philo *philo, char *message, int color)
{
	if (check_sim_stop(philo->sim))
		return ;
	else
	{
		pthread_mutex_lock(&philo->sim->print_mutex);
		printf("\033[%dm%ld %d %s\033[0m\n",
			color,
			get_time_in_ms() - philo->sim->start_time,
			philo->id,
			message);
		pthread_mutex_unlock(&philo->sim->print_mutex);
	}
}

void	free_sim(t_simulation *sim)
{
	int	i;

	i = 0;
	while (i < sim->nbr_of_philos)
	{
		pthread_mutex_destroy(&sim->forks[i]);
		i++;
	}
	free(sim->forks);
	pthread_mutex_destroy(&sim->print_mutex);
	pthread_mutex_destroy(&sim->meal_check_mutex);
	pthread_mutex_destroy(&sim->stop_mutex);
	free(sim->philos);
}

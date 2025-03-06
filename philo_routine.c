/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbenhami <nbenhami@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 18:14:27 by nbenhami          #+#    #+#             */
/*   Updated: 2025/03/06 12:46:41 by nbenhami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	take_forks(t_philo *philo)
{
	pthread_mutex_t	*first;
	pthread_mutex_t	*second;

	if (philo->left_fork < philo->right_fork)
	{
		first = philo->left_fork;
		second = philo->right_fork;
	}
	else
	{
		first = philo->right_fork;
		second = philo->left_fork;
	}
	pthread_mutex_lock(first);
	print_message(philo, "has taken a fork", MAGENTA);
	usleep(100);
	pthread_mutex_lock(second);
	print_message(philo, "has taken a fork", MAGENTA);
}

void	eat(t_philo *philo)
{
	print_message(philo, "is eating", GREEN);
	pthread_mutex_lock(&philo->sim->meal_check_mutex);
	philo->last_eat = get_time_in_ms();
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->sim->meal_check_mutex);
	sleep_ms(philo->sim->time_to_eat);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}

void	philo_sleep(t_philo *philo)
{
	print_message(philo, "is sleeping", YELLOW);
	sleep_ms(philo->sim->time_to_sleep);
}

void	think(t_philo *philo)
{
	print_message(philo, "is thinking", BLUE);
}

int	check_sim_stop(t_simulation *sim)
{
	int	stop;

	pthread_mutex_lock(&sim->stop_mutex);
	stop = sim->stop;
	pthread_mutex_unlock(&sim->stop_mutex);
	return (stop);
}

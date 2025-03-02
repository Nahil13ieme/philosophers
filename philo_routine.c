/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbenhami <nbenhami@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 18:14:27 by nbenhami          #+#    #+#             */
/*   Updated: 2025/03/02 18:14:33 by nbenhami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	take_forks(t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(philo->right_fork);
		print_message(philo, "has taken a fork");
		pthread_mutex_lock(philo->left_fork);
		print_message(philo, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(philo->left_fork);
		print_message(philo, "has taken a fork");
		if (philo->sim->nbr_of_philos == 1)
			return ;
		pthread_mutex_lock(philo->right_fork);
		print_message(philo, "has taken a fork");
	}
}

void	eat(t_philo *philo)
{
	print_message(philo, "is eating");
	sleep_ms(philo->sim->time_to_eat);
	pthread_mutex_lock(&philo->sim->meal_check_mutex);
	philo->last_eat = get_time_in_ms();
	pthread_mutex_unlock(&philo->sim->meal_check_mutex);
	if (philo->sim->nbr_to_eat != -1)
		philo->meals_eaten++;
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}

void	philo_sleep(t_philo *philo)
{
	print_message(philo, "is sleeping");
	sleep_ms(philo->sim->time_to_sleep);
}

void	think(t_philo *philo)
{
	print_message(philo, "is thinking");
}
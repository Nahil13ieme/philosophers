/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbenhami <nbenhami@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 18:14:27 by nbenhami          #+#    #+#             */
/*   Updated: 2025/03/03 14:32:06 by nbenhami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	take_forks(t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(philo->right_fork);
		print_message(philo, "has taken a fork", MAGENTA);
		pthread_mutex_lock(philo->left_fork);
		print_message(philo, "has taken a fork", MAGENTA);
	}
	else
	{
		pthread_mutex_lock(philo->left_fork);
		print_message(philo, "has taken a fork", MAGENTA);
		if (philo->sim->nbr_of_philos == 1)
		{
			pthread_mutex_unlock(philo->left_fork);
			return ;
		}
		pthread_mutex_lock(philo->right_fork);
		print_message(philo, "has taken a fork", MAGENTA);
	}
}

void	eat(t_philo *philo)
{
	print_message(philo, "is eating", GREEN);
	pthread_mutex_lock(&philo->sim->meal_check_mutex);
	philo->last_eat = get_time_in_ms();
	pthread_mutex_unlock(&philo->sim->meal_check_mutex);
	sleep_ms(philo->sim->time_to_eat);
	if (philo->sim->nbr_to_eat != -1)
		philo->meals_eaten++;
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

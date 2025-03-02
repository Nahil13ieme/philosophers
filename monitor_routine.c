/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_routine.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbenhami <nbenhami@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 20:13:44 by nbenhami          #+#    #+#             */
/*   Updated: 2025/03/02 20:39:55 by nbenhami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*-------------------- PHILOS ROUTINES --------------------*/
void	check_philo_routine(t_philo *philo)
{
	while (!check_sim_stop(philo->sim))
	{
		sleep_ms(1);
		take_forks(philo);
		if (check_sim_stop(philo->sim))
		{
			pthread_mutex_unlock(philo->left_fork);
			pthread_mutex_unlock(philo->right_fork);
			break ;
		}
		eat(philo);
		if (check_sim_stop(philo->sim))
			break ;
		philo_sleep(philo);
		if (check_sim_stop(philo->sim))
			break ;
		think(philo);
	}
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	pthread_mutex_lock(&philo->sim->meal_check_mutex);
	philo->last_eat = get_time_in_ms();
	pthread_mutex_unlock(&philo->sim->meal_check_mutex);
	if (philo->sim->nbr_of_philos == 1)
	{
		think(philo);
		take_forks(philo);
		pthread_mutex_unlock(philo->left_fork);
		while (!check_sim_stop(philo->sim))
			usleep(100);
	}
	else
		check_philo_routine(philo);
	return (NULL);
}
/*-------------------- MONITOR ROUTINES -------------------*/
int	check_philo(t_simulation *sim, int *all_eaten)
{
	int	i;

	i = 0;
	while (i < sim->nbr_of_philos)
	{
		pthread_mutex_lock(&sim->meal_check_mutex);
		if (get_time_in_ms() - sim->philos[i].last_eat > sim->time_to_die)
		{
			pthread_mutex_lock(&sim->stop_mutex);
			sim->stop = 1;
			pthread_mutex_unlock(&sim->stop_mutex);
			pthread_mutex_unlock(&sim->meal_check_mutex);
			pthread_mutex_lock(&sim->print_mutex);
			print_message(&sim->philos[i], "died", RED);
			pthread_mutex_unlock(&sim->print_mutex);
			(*all_eaten) = -1;
			return (1);
		}
		if (sim->nbr_to_eat != -1
				&& sim->philos[i].meals_eaten >= sim->nbr_to_eat)
			(*all_eaten)++;
		pthread_mutex_unlock(&sim->meal_check_mutex);
		i++;
	}
	return (0);
}

void	*monitor_routine(void *arg)
{
	t_simulation	*sim;
	int				all_eaten;

	sim = (t_simulation *)arg;
	while (!sim->stop)
	{
		all_eaten = 0;
		if (check_philo(sim, &all_eaten))
			break ;
		if (all_eaten == sim->nbr_of_philos)
		{
			pthread_mutex_lock(&sim->stop_mutex);
			sim->stop = 1;
			pthread_mutex_unlock(&sim->stop_mutex);
			pthread_mutex_lock(&sim->print_mutex);
			printf("\033[33m All spaghettis have been eaten !\033[0m\n");
			pthread_mutex_unlock(&sim->print_mutex);
			return (NULL);
		}
	}
	return (NULL);
}
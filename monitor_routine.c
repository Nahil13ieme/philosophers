/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_routine.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbenhami <nbenhami@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 20:13:44 by nbenhami          #+#    #+#             */
/*   Updated: 2025/03/05 20:25:43 by nbenhami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*-------------------- PHILOS ROUTINES --------------------*/
void	check_philo_routine(t_philo *philo)
{
	while (!check_sim_stop(philo->sim))
	{
		print_message(philo, "is thinking", BLUE);
		take_forks(philo);
		eat(philo);
		if (check_sim_stop(philo->sim))
			break ;
		philo_sleep(philo);
		if (check_sim_stop(philo->sim))
			break ;
	}
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	pthread_mutex_lock(&philo->sim->meal_check_mutex);
	philo->last_eat = get_time_in_ms();
	pthread_mutex_unlock(&philo->sim->meal_check_mutex);
	if (philo->id % 2 == 1)
		sleep_ms(1);
	if (philo->sim->nbr_of_philos == 1)
	{
		think(philo);
		pthread_mutex_lock(philo->left_fork);
		print_message(philo, "has taken a fork", CYAN);
		pthread_mutex_unlock(philo->left_fork);
		while (!check_sim_stop(philo->sim))
		{
			usleep(10);
		}
	}
	else
		check_philo_routine(philo);
	return (NULL);
}

/*-------------------- MONITOR ROUTINES -------------------*/

static int	check_last_meal(t_simulation *sim, int index)
{
	long	last_eat;

	pthread_mutex_lock(&sim->meal_check_mutex);
	last_eat = get_time_in_ms() - sim->philos[index].last_eat;
	pthread_mutex_unlock(&sim->meal_check_mutex);
	if (last_eat >= sim->time_to_die + 1)
	{
		print_message(&sim->philos[index], "died", RED);
		pthread_mutex_lock(&sim->stop_mutex);
		sim->stop = 1;
		pthread_mutex_unlock(&sim->stop_mutex);
		return (0);
	}
	return (1);
}

int	check_philo(t_simulation *sim, int *all_eaten)
{
	int		i;

	i = 0;
	while (i < sim->nbr_of_philos)
	{
		if (!check_last_meal(sim, i))
			return (1);
		pthread_mutex_lock(&sim->meal_check_mutex);
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
	sleep_ms(sim->time_to_die / 2);
	while (!sim->stop)
	{
		all_eaten = 0;
		if (check_philo(sim, &all_eaten))
			break ;
		if (all_eaten >= sim->nbr_of_philos)
		{
			pthread_mutex_lock(&sim->stop_mutex);
			sim->stop = 1;
			pthread_mutex_unlock(&sim->stop_mutex);
			return (NULL);
		}
		usleep(500);
	}
	return (NULL);
}

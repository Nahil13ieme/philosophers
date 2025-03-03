/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_routine.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbenhami <nbenhami@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 20:13:44 by nbenhami          #+#    #+#             */
/*   Updated: 2025/03/03 17:50:20 by nbenhami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*-------------------- PHILOS ROUTINES --------------------*/
void	check_philo_routine(t_philo *philo)
{
	while (!check_sim_stop(philo->sim))
	{
		usleep(500);
		think(philo);
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
	if (philo->id % 2 == 1)
		usleep(50);
	if (philo->sim->nbr_of_philos == 1)
	{
		pthread_mutex_lock(&philo->sim->meal_check_mutex);
		philo->last_eat = get_time_in_ms();
		pthread_mutex_unlock(&philo->sim->meal_check_mutex);
		think(philo);
		take_forks(philo);
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
	int		is_initialized;

	pthread_mutex_lock(&sim->meal_check_mutex);
	last_eat = get_time_in_ms() - sim->philos[index].last_eat;
	is_initialized = sim->philos[index].last_eat > 0;
	pthread_mutex_unlock(&sim->meal_check_mutex);
	if (is_initialized && last_eat > sim->time_to_die)
	{
		print_message(&sim->philos[index], "died", RED);
		pthread_mutex_lock(&sim->stop_mutex);
		sim->stop = 1;
		pthread_mutex_unlock(&sim->stop_mutex);
		printf("TIMESPTAN = %ld \n", last_eat);
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
			return (0);
		if (sim->nbr_to_eat != -1
			&& sim->philos[i].meals_eaten >= sim->nbr_to_eat)
			(*all_eaten)++;
		i++;
	}
	return (0);
}

void	*monitor_routine(void *arg)
{
	t_simulation	*sim;
	int				all_eaten;

	sim = (t_simulation *)arg;
	usleep(sim->nbr_of_philos * 10);
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

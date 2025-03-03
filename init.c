/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbenhami <nbenhami@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 13:18:38 by nbenhami          #+#    #+#             */
/*   Updated: 2025/03/03 13:51:26 by nbenhami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	init_mutex(t_simulation *sim)
{
	int	i;

	i = 0;
	while (i < sim->nbr_of_philos)
	{
		pthread_mutex_init(&sim->forks[i], NULL);
		i++;
	}
	pthread_mutex_init(&sim->print_mutex, NULL);
	pthread_mutex_init(&sim->meal_check_mutex, NULL);
	pthread_mutex_init(&sim->stop_mutex, NULL);
}

void	init_philo(t_simulation *sim)
{
	int	i;

	i = 0;
	while (i < sim->nbr_of_philos)
	{
		sim->philos[i].id = i + 1;
		sim->philos[i].meals_eaten = 0;
		sim->philos[i].nbr_to_eat = sim->nbr_to_eat;
		sim->philos[i].sim = sim;
		sim->philos[i].left_fork = &sim->forks[i];
		sim->philos[i].last_eat = 0;
		if (sim->nbr_of_philos == 1)
			sim->philos[i].right_fork = NULL;
		else
			sim->philos[i].right_fork = &sim->forks[(i + 1)
				% sim->nbr_of_philos];
		i++;
	}
}

void	init_thread(t_simulation *sim)
{
	int	i;
	int	error;

	i = 0;
	sim->start_time = get_time_in_ms();
	while (i < sim->nbr_of_philos)
	{
		error = pthread_create(&sim->philos[i].thread, NULL,
				philo_routine, &sim->philos[i]);
		if (error)
			exit(EXIT_FAILURE);
		i++;
	}
	error = pthread_create(&sim->monitor_thread, NULL, monitor_routine, sim);
	if (error)
		exit(EXIT_FAILURE);
}

void	init_sim(t_simulation *sim, int argc, char **argv)
{
	sim->nbr_of_philos = ft_atoi(argv[1]);
	sim->time_to_die = ft_atoi(argv[2]);
	sim->time_to_eat = ft_atoi(argv[3]);
	sim->time_to_sleep = ft_atoi(argv[4]);
	sim->nbr_to_eat = -1;
	sim->stop = 0;
	if (argc == 6)
		sim->nbr_to_eat = ft_atoi(argv[5]);
	sim->forks = malloc(sizeof(pthread_mutex_t) * sim->nbr_of_philos);
	sim->philos = malloc(sizeof(t_philo) * sim->nbr_of_philos);
	if (!sim->forks || !sim->philos)
		exit(EXIT_FAILURE);
	init_mutex(sim);
	init_philo(sim);
	init_thread(sim);
}

void	join_thread(t_simulation *sim)
{
	int	i;

	i = 0;
	while (i < sim->nbr_of_philos)
	{
		pthread_join(sim->philos[i].thread, NULL);
		i++;
	}
	pthread_join(sim->monitor_thread, NULL);
}

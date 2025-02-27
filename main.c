/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbenhami <nbenhami@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 21:15:22 by nbenhami          #+#    #+#             */
/*   Updated: 2025/02/26 18:08:07 by nbenhami         ###   ########.fr       */
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

void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
}

void	init_sim(t_simulation *sim, int argc, char **argv)
{
	int	i;
	int	error;
	
	printf("Start init simulation...\n");
	sim->nbr_of_philos = argv[1];
	sim->time_to_die = argv[2];
	sim->time_to_eat = argv[3];
	sim->time_to_sleep = argv[4];
	sim->nbr_to_eat = -1;
	if (argc == 6)
		sim->nbr_to_eat = argv[5];
	sim->forks = malloc(sizeof(pthread_mutex_t) * sim->nbr_of_philos);
	sim->philos = malloc(sizeof(t_philo) * sim->nbr_of_philos);
	if (!sim->forks || !sim->philos)
		exit(EXIT_FAILURE);
	i = 0;
	while (i < sim->nbr_of_philos)
	{
		sim->philos[i].id = i + 1;
		sim->philos[i].meals_eaten = 0;		
		sim->philos[i].nbr_to_eat = sim->nbr_to_eat;
		sim->philos[i].sim = sim;
		sim->philos[i].left_fork = &sim->forks[i];
		sim->philos[i].right_fork = &sim->forks[(i + 1) % sim->nbr_of_philos];
	}
	i = 0;
	while (i < sim->nbr_of_philos)
	{
		error = pthread_create(&sim->philos[i].thread, NULL, philo_routine, &sim->philos[i]);
	}
	printf("End init simulation.\n");
}

int	main(int argc, char *argv[])
{
	t_simulation	sim;

	if (argc != 5 && argc != 6)
	{
		return (1);
	}
	init_sim(&sim, argc, argv);
	printf("End of the program\n");
	return (0);
}
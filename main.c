/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbenhami <nbenhami@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 21:15:22 by nbenhami          #+#    #+#             */
/*   Updated: 2025/03/02 18:23:51 by nbenhami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_sim_stop(t_simulation *sim)
{
	pthread_mutex_lock(&sim->stop_mutex);
	int stop = sim->stop;
	pthread_mutex_unlock(&sim->stop_mutex);
	return stop;
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
		take_forks(philo); // Prend une fourchette
		pthread_mutex_unlock(philo->left_fork);
		while (!check_sim_stop(philo->sim))
			usleep(100); // Attend doucement la mort
	}
	else
	{
		while (!check_sim_stop(philo->sim))
		{
			sleep_ms(1); // Anti-famine pour plusieurs philosophes
			take_forks(philo);
			if (check_sim_stop(philo->sim))
			{
				pthread_mutex_unlock(philo->left_fork);
				pthread_mutex_unlock(philo->right_fork);
				break;
			}
			eat(philo);
			if (check_sim_stop(philo->sim))
				break;
			philo_sleep(philo);
			if (check_sim_stop(philo->sim))
				break;
			think(philo);
		}
	}
	return (NULL);
}

void	*monitor_routine(void *arg)
{
	t_simulation	*sim;
	int				i;
	int				all_eaten;

	sim = (t_simulation *)arg;
	while(!sim->stop)
	{
		i = 0;
		all_eaten = 0;
		while (i < sim->nbr_of_philos)
		{
			pthread_mutex_lock(&sim->meal_check_mutex);
			if (get_time_in_ms() - sim->philos[i].last_eat > sim->time_to_die)
			{
				pthread_mutex_lock(&sim->stop_mutex);
				sim->stop = 1;
				pthread_mutex_unlock(&sim->stop_mutex);
				pthread_mutex_unlock(&sim->meal_check_mutex);
				print_message(&sim->philos[i], "died");
				return (NULL);
			}
			if (sim->nbr_to_eat != -1 && sim->philos[i].meals_eaten >= sim->nbr_to_eat)
				all_eaten++;
			pthread_mutex_unlock(&sim->meal_check_mutex);
			i++;
		}
		if (all_eaten == sim->nbr_of_philos)
		{
			pthread_mutex_lock(&sim->stop_mutex);
			sim->stop = 1;
			pthread_mutex_unlock(&sim->stop_mutex);
			pthread_mutex_lock(&sim->print_mutex);
			printf("All philosophers have eaten enough meals\n");
			i = 0;
			while (i < sim->nbr_of_philos)
			{
				printf("%d has eaten %d meals\n", sim->philos[i].id, sim->philos[i].meals_eaten);
				pthread_mutex_unlock(sim->philos[i].left_fork);
				pthread_mutex_unlock(sim->philos[i].right_fork);
				i++;
			}
			pthread_mutex_unlock(&sim->print_mutex);
			return (NULL);
		}
	}
	return (NULL);
}

int	check_args(int argc, char **argv)
{
	int	i;
	int	j;

	i = 1;
	while (i < argc)
	{
		j = 0;
		while (argv[i][j] >= '0' && argv[i][j] <= '9')
			j++;
		if (j == 0 || argv[i][j] != '\0')
			return (0);
		i++;
	}
	i = 1;
	while (i < argc)
	{
		if (ft_atoi(argv[i]) < 0)
			return (0);
		i++;
	}
	return (1);
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

int	main(int argc, char *argv[])
{
	t_simulation	sim;

	if (argc != 5 && argc != 6)
	{
		write(1, "Error: wrong number of arguments\n", 34);
		write(1, "Usage: ./philo <number_of_philos> <time_to_die> <time_to_eat> <time_to_sleep> [number_of_meals]\n", 97);
		return (1);
	}
	if (!check_args(argc, argv))
	{
		write(1, "Error: invalid arguments\n", 25);
		return (1);
	}
	init_sim(&sim, argc, argv);
	join_thread(&sim);
	printf("End of the program\n");
	return (0);
}
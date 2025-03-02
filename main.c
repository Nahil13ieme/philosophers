/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbenhami <nbenhami@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 21:15:22 by nbenhami          #+#    #+#             */
/*   Updated: 2025/03/02 20:23:27 by nbenhami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	check_args(int argc, char **argv)
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
	free_sim(&sim);
	printf("End of the program\n");
	return (0);
}

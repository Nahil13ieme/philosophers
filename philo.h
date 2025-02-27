/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbenhami <nbenhami@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 22:14:29 by nbenhami          #+#    #+#             */
/*   Updated: 2025/02/25 17:40:30 by nbenhami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
#define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>
# include <unistd.h>
# include <string.h>

typedef struct s_simulation	t_simulation;

typedef struct s_philo
{
	int				id;
	int				meals_eaten;
	long			last_eat;
	int				nbr_to_eat;
	pthread_t		thread;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	t_simulation	*sim;
}	t_philo;


typedef struct s_simulation
{
	int				nbr_of_philos;
	int				stop;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	int				nbr_to_eat;
	long			start_time;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	meal_check_mutex;
	t_philo			*philos;
}	t_simulation;

long	get_time_in_ms(void);
void	sleep_ms(long ms);

#endif
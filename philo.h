/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbenhami <nbenhami@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 22:14:29 by nbenhami          #+#    #+#             */
/*   Updated: 2025/03/05 17:59:27 by nbenhami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>
# include <unistd.h>
# include <string.h>

# define RED 31
# define CYAN 36
# define GREEN 32
# define YELLOW 33
# define BLUE 34
# define MAGENTA 35

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
	pthread_mutex_t	stop_mutex;
	pthread_t		monitor_thread;
	t_philo			*philos;
}	t_simulation;

/*-------------- UTILS --------------*/
long	get_time_in_ms(void);
void	sleep_ms(long ms);
int		ft_atoi(char *str);
void	print_message(t_philo *philo, char *message, int color);
void	free_sim(t_simulation *sim);

/*-------------- INIT  --------------*/
void	init_mutex(t_simulation *sim);
void	init_philo(t_simulation *sim);
void	init_thread(t_simulation *sim);
void	init_sim(t_simulation *sim, int argc, char **argv);
void	join_thread(t_simulation *sim);

/*-------------- Main ---------------*/
//staitic int	check_args(int argc, char **argv);
//int			main(int argc, char **argv);

/*--------- PHILO ROUTINES ----------*/
void	take_forks(t_philo *philo);
void	eat(t_philo *philo);
void	philo_sleep(t_philo *philo);
void	think(t_philo *philo);
int		check_sim_stop(t_simulation *sim);

/*-------- MONITOR ROUTINES ---------*/
void	check_philo_routine(t_philo *philo);
void	*philo_routine(void *arg);
int		check_philo(t_simulation *sim, int *all_eaten);
void	*monitor_routine(void *arg);

#endif
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eakman <arcemirhanakman@gmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 21:28:53 by eakman            #+#    #+#             */
/*   Updated: 2024/05/24 21:28:53 by eakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdbool.h>
# include <sys/time.h>
# include <stdlib.h>

# define ERRORMSG1 "Error; number of arguments is incorrect"
# define ERRORMSG2 "Arguments contain unwanted expressions"
# define ERRORMSG3 "Error: mutex init for is_anyone_died failed"
# define ERRORMSG4 "Error: mutex init for print_mutex failed"
# define TAKE_FORK "has taken a fork"
# define DIED "is dead"
# define EATING "is eating"
# define SLEEPING "is sleeping"
# define THINKING "is thinking"

typedef struct s_info	t_info;
typedef struct s_philo	t_philo;

struct s_philo
{
	int				id;
	int				enought_meal;
	int				how_many_meals;
	time_t			last_meal_time;
	pthread_mutex_t	last_meal_mutex;
	pthread_mutex_t	enought_meal_mutex;
	pthread_t		thread;
	t_info			*info;
};

struct s_info
{
	int				number_of_philos;
	int				must_eat;
	bool			is_anyone_died;
	time_t			time_to_die;
	time_t			time_to_eat;
	time_t			time_to_sleep;
	time_t			start_time;
	pthread_mutex_t	*forks;
	pthread_mutex_t	is_anyone_died_mutex;
	pthread_mutex_t	print_mutex;
};

void	*monitor(void *arg);
void	create_philos(t_philo *philos, t_info *info);
void	senstive_usleep(time_t time);
int		ft_atoi(const char *str);
time_t	get_time(void);
void	print_kill(t_philo philo, char *msg, int flag);
int		death_control(t_philo *philo, int flag);
void	clean_up(pthread_t *monitor_thread, t_philo *philos);

// init
int		init_info(t_info *info, int ac, char **av);
int		init_philos(t_philo *philos, t_info *info);

#endif

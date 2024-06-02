/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eakman <arcemirhanakman@gmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 00:14:07 by eakman            #+#    #+#             */
/*   Updated: 2024/06/02 00:14:07 by eakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"
#include <stdio.h>

int	init_philos(t_philo *philos, t_info *info)
{
	int	i;

	i = 0;
	while (i < info->number_of_philos)
	{
		philos[i].id = i;
		philos[i].info = info;
		philos[i].last_meal_time = info->start_time;
		philos[i].how_many_meals = 0;
		if (pthread_mutex_init(&philos[i].last_meal_mutex, NULL)
			|| pthread_mutex_init(&philos[i].enought_meal_mutex, NULL))
		{
			printf("Error: mutex init for philo %d failed\n", i);
			return (0);
		}
		i++;
	}
	return (1);
}

static int	mutex_init(t_info *info)
{
	int	i;

	i = 0;
	while (i < info->number_of_philos)
	{
		if (pthread_mutex_init(&info->forks[i], NULL))
		{
			printf("Error: mutex init for fork %d failed\n", i);
			return (0);
		}
		i++;
	}
	if (pthread_mutex_init(&info->is_anyone_died_mutex, NULL))
	{
		printf("%s\n", ERRORMSG3);
		return (0);
	}
	if (pthread_mutex_init(&info->print_mutex, NULL))
	{
		printf("%s\n", ERRORMSG4);
		return (0);
	}
	return (1);
}

int	init_info(t_info *info, int ac, char **av)
{
	info->number_of_philos = ft_atoi(av[1]);
	info->time_to_die = ft_atoi(av[2]);
	info->time_to_eat = ft_atoi(av[3]);
	info->time_to_sleep = ft_atoi(av[4]);
	info->must_eat = -1;
	if (ac == 6)
		info->must_eat = ft_atoi(av[5]);
	info->start_time = get_time();
	info->is_anyone_died = false;
	info->forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t)
			* info->number_of_philos);
	if (!info->forks)
		return (0);
	if (info->number_of_philos == 0)
		return (free(info->forks), printf("Error: not number is number"), 0);
	if (!mutex_init(info))
		return (free(info->forks), 0);
	return (1);
}

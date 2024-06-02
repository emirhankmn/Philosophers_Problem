/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eakman <arcemirhanakman@gmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 23:53:44 by eakman            #+#    #+#             */
/*   Updated: 2024/05/24 23:53:44 by eakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

int	death_control(t_philo *philo, int flag)
{
	int	result;

	result = 0;
	if (flag == 1)
	{
		pthread_mutex_lock(&philo->last_meal_mutex);
		pthread_mutex_lock(&philo->enought_meal_mutex);
		if (philo->enought_meal == false && get_time()
			- philo->info->start_time - philo->last_meal_time
			> philo->info->time_to_die)
			result = 1;
		pthread_mutex_unlock(&philo->enought_meal_mutex);
		pthread_mutex_unlock(&philo->last_meal_mutex);
	}
	else if (flag == 0)
	{
		pthread_mutex_lock(&philo->info->is_anyone_died_mutex);
		if (philo->info->is_anyone_died == false)
			result = 1;
		pthread_mutex_unlock(&philo->info->is_anyone_died_mutex);
	}
	return (result);
}

static void	routine(t_philo *philo)
{
	pthread_mutex_lock(&philo->info->forks[philo->id]);
	print_kill(*philo, TAKE_FORK, 0);
	pthread_mutex_lock(&philo->info->forks[(philo->id + 1)
		% philo->info->number_of_philos]);
	print_kill(*philo, TAKE_FORK, 0);
	pthread_mutex_lock(&philo->last_meal_mutex);
	philo->last_meal_time = get_time() - philo->info->start_time;
	pthread_mutex_unlock(&philo->last_meal_mutex);
	print_kill(*philo, EATING, 0);
	senstive_usleep(philo->info->time_to_eat);
	pthread_mutex_unlock(&philo->info->forks[philo->id]);
	pthread_mutex_unlock(&philo->info->forks[(philo->id + 1)
		% philo->info->number_of_philos]);
	print_kill(*philo, SLEEPING, 0);
	senstive_usleep(philo->info->time_to_sleep);
	print_kill(*philo, THINKING, 0);
	if (philo->info->number_of_philos % 2 != 0)
		senstive_usleep(philo->info->time_to_eat / 2);
}

void	*philo_life(void *ar)
{
	t_philo	*philo;

	philo = (t_philo *)ar;
	if (philo->id % 2 == 0)
		senstive_usleep(philo->info->time_to_eat / 2);
	while (death_control(philo, 0))
	{
		routine(philo);
		philo->how_many_meals++;
		if (philo->info->must_eat != -1
			&& philo->how_many_meals == philo->info->must_eat)
		{
			break ;
		}
	}
	pthread_mutex_lock(&philo->enought_meal_mutex);
	philo->enought_meal = true;
	pthread_mutex_unlock(&philo->enought_meal_mutex);
	return (NULL);
}

void	create_philos(t_philo *philos, t_info *info)
{
	int	i;

	i = 0;
	while (i < info->number_of_philos)
	{
		pthread_create(&philos[i].thread, NULL, &philo_life, &philos[i]);
		i++;
	}
	i = 0;
	while (i < info->number_of_philos)
	{
		pthread_join(philos[i].thread, NULL);
		i++;
	}
}

void	*monitor(void *ar)
{
	int		i;
	int		j;
	t_philo	*philos;

	philos = (t_philo *)ar;
	while (1)
	{
		i = 0;
		j = 0;
		while (i < philos->info->number_of_philos)
		{
			if (death_control(&philos[i], 1))
				return (print_kill(philos[i], DIED, 1), NULL);
			pthread_mutex_lock(&philos[i].enought_meal_mutex);
			if (philos[i].enought_meal == true)
				j++;
			pthread_mutex_unlock(&philos[i].enought_meal_mutex);
			i++;
		}
		if (j == philos->info->number_of_philos)
			break ;
	}
	return (NULL);
}

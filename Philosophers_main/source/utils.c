/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eakman <arcemirhanakman@gmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 23:02:19 by eakman            #+#    #+#             */
/*   Updated: 2024/05/24 23:02:19 by eakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void    print_kill(t_philo philo, char *msg, int flag)
{
    if (flag)
    {
        pthread_mutex_lock(&philo.info->is_anyone_died_mutex);
        philo.info->is_anyone_died = true;
        pthread_mutex_unlock(&philo.info->is_anyone_died_mutex);
    }
    pthread_mutex_lock(&philo.info->is_anyone_died_mutex);
    if ((philo.enought_meal == false && philo.info->is_anyone_died == false) || flag == 1)
    {
        pthread_mutex_lock(&philo.info->print_mutex);
        printf("%ld %d %s\n", get_time() - philo.info->start_time, philo.id + 1, msg);
        pthread_mutex_unlock(&philo.info->print_mutex);
    }
    pthread_mutex_unlock(&philo.info->is_anyone_died_mutex);
}

void    clean_up(pthread_t *monitor_thread, t_philo *philos)
{
    int i;

    pthread_join(*monitor_thread, NULL);
    i = 0;
    while (i < philos->info->number_of_philos)
    {
        pthread_mutex_destroy(&philos[i].last_meal_mutex);
        pthread_mutex_destroy(&philos[i].enought_meal_mutex);
        i++;
    }
    pthread_mutex_destroy(&philos->info->is_anyone_died_mutex);
    free(philos->info->forks);
    free(philos);
}

time_t  get_time(void)
{
    struct timeval  time;

    gettimeofday(&time, NULL);
    return ((time.tv_usec / 1000 + time.tv_sec * 1000));
}

void    senstive_usleep(time_t time)
{
    time_t wake_up;

    wake_up = get_time() + time;
    while (get_time() < wake_up)
        usleep(200);
}

int	ft_atoi(const char *str)
{
	int	num;
	int	i;

	i = 0;
	num = 0;
	while (str[i])
	{
		num = num * 10 + (str[i] - '0');
		i++;
	}
	return (num);
}

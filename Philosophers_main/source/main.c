/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eakman <arcemirhanakman@gmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 21:29:09 by eakman            #+#    #+#             */
/*   Updated: 2024/05/24 21:29:09 by eakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

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

static void if_one_philo(t_info *info)
{
    printf("%d %d %s\n", 0, 1, TAKE_FORK);
    senstive_usleep(info->time_to_die);
    printf("%ld %d %s\n", info->time_to_die , 0, DIED);
    free(info->forks);
}

static int  init_philos(t_philo *philos, t_info *info)
{
    int i;

    i = 0;
    while (i < info->number_of_philos)
    {
        philos[i].id = i;
        philos[i].info = info;
        philos[i].last_meal_time = info->start_time;
        philos[i].how_many_meals = 0;
        if (pthread_mutex_init(&philos[i].last_meal_mutex, NULL) || pthread_mutex_init(&philos[i].enought_meal_mutex, NULL))
        {
            printf("Error: mutex init for philo %d failed\n", i);
            return (0);
        }
        i++;
    }
    return (1);
}

int mutex_init(t_info *info)
{
    int i;

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
        printf("Error: mutex init for is_anyone_died failed\n");
        return (0);
    }
    if (pthread_mutex_init(&info->print_mutex, NULL))
    {
        printf("Error: mutex init for print_mutex failed\n");
        return (0);
    }
    return (1);
}

static int  init_info(t_info *info, int ac, char **av)
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
    info->forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * info->number_of_philos);
    if (!info->forks)
        return (0);
    if (info->number_of_philos == 0)
        return (free(info->forks), write(2, "Error\n", 6), 0);
    if (!mutex_init(info))
        return (free(info->forks), 0);
    return (1);
}

int check_args(int ac, char **av)
{
	int	i;
	int	q;

	if (ac < 5 || ac > 6)
		return (printf("%s", ERRORMSG1), 1);
	i = 1;
	while (av[i])
	{
		q = 0;
		while (av[i][q])
		{
			if (av[i][q] < 48 || av[i][q] > 57)
				return (printf("%s", ERRORMSG2), 1);
			q++;
		}
		i++;
	}
	return (0);
}

int main(int ac, char **av)
{
    t_philo *philos;
    t_info info;
    pthread_t monitor_thread;

    if (check_args(ac, av))
        return (1);
    if (!init_info(&info, ac, av))
        return (1);
    philos = (t_philo *)malloc(sizeof(t_philo) * info.number_of_philos);
    if  (!philos)
        return (0);
    if (info.number_of_philos == 1)
        return (if_one_philo(&info), free(philos), 1);
    if (!init_philos(philos, &info) || pthread_create(&monitor_thread, NULL, &monitor, philos))
        return (free(info.forks), free(philos), 0);
    create_philos(philos, &info);
    return (clean_up(&monitor_thread, philos), 0);
}
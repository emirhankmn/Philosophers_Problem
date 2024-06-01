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

static void if_one_philo(t_info *info)
{
    printf("%d %d %s\n", 0, 1, TAKE_FORK);
    senstive_usleep(info->time_to_die);
    printf("%ld %d %s\n", info->time_to_die , 0, DIED);
    free(info->forks);
}

static int check_args(int ac, char **av)
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
        return (if_one_philo(&info), free(philos), 0);
    if (!init_philos(philos, &info) || pthread_create(&monitor_thread, NULL, &monitor, philos))
        return (free(info.forks), free(philos), 0);
    create_philos(philos, &info);
    return (clean_up(&monitor_thread, philos), 0);
}

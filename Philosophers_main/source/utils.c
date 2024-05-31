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
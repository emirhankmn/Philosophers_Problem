/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eakman <eakman@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 18:07:40 by eakman            #+#    #+#             */
/*   Updated: 2024/06/02 14:14:30 by eakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*monitor(void *data_pointer)//Yalnizca istedigimiz dongu sayisi kadar calisir.
{
	t_philo	*philo;

	philo = (t_philo *) data_pointer;
	pthread_mutex_lock(&philo->data->write);
	printf("data val: %d\n", philo->data->dead);
	pthread_mutex_unlock(&philo->data->write);
	pthread_mutex_lock(&philo->data->died);
	while (philo->data->dead == 0)//tum yemek yeme islemleri izlenir.
	{
		pthread_mutex_unlock(&philo->data->died);
		pthread_mutex_lock(&philo->lock);//kilitlenince hic bir thread bu kisma mudahale edemez.
		if (philo->data->finished >= philo->data->number_of_philosophers)//eger her filozof yeteri kadar yemek yemisse olum durumu varmis gibi davranilir ve donguden cikilir.
		{
			pthread_mutex_lock(&philo->data->died);
			philo->data->dead = 1;
			pthread_mutex_unlock(&philo->data->died);
		}
		pthread_mutex_unlock(&philo->lock);//kilit acilana kadar tabii ki.
	}
	return ((void *)0);
}

void	*supervisor(void *philo_pointer)
{
	t_philo	*philo;

	philo = (t_philo *) philo_pointer;
	pthread_mutex_lock(&philo->data->died);
	while (philo->data->dead == 0)// olen biri olana kadar dongu sonsuza kadar devam eder.
	{
		pthread_mutex_unlock(&philo->data->died);
		pthread_mutex_lock(&philo->lock);
		if (get_time() >= philo->dead_time && philo->eating == 0)//eger filozoflardan biri olurse messages fonsiyonuna gidilir.
			messages(DIED, philo);
		if (philo->eat_cont == philo->data->must_to_eat)//istenilen sayida filozoflarin yemek yemelerini saymak icin bir kosul.
		{
			pthread_mutex_lock(&philo->data->lock);
			philo->data->finished++;
			philo->eat_cont++;
			pthread_mutex_unlock(&philo->data->lock);
		}
		pthread_mutex_unlock(&philo->lock);
	}
	return ((void *)0);
}

void	*routine(void *philo_pointer)
{
	t_philo	*philo;

	philo = (t_philo *) philo_pointer;
	philo->dead_time = philo->data->time_to_die + get_time();
	if (pthread_create(&philo->t1, NULL, &supervisor, (void *)philo))//olum durumlarini takip edecek bir thread gerekiyor.
		return ((void *)1);
	pthread_mutex_lock(&philo->data->died);
	while (philo->data->dead == 0)
	{
		pthread_mutex_unlock(&philo->data->died);
		eat(philo);
		messages(THINKING, philo);
	}
	if (pthread_join(philo->t1, NULL))//threadlerin basariyla sonlanip sonlanmadigini kontrol eder.
		return ((void *)1);//hata durumu return eder.
	return ((void *)0);//hatasiz demek oldugu icin 0 return eder.
}

int	thread_init(t_data *data)
{
	int			i;
	pthread_t	t0;

	i = -1;
	data->start_time = get_time();
	if (data->must_to_eat > 0)//belirli sayida yeme zorunlulugu varsa monitor fonksiyonu cagirilir.
	{
		if (pthread_create(&t0, NULL, &monitor, &data->philos[0]))//her filozofun yemesi gereken miktari kontrol eden bir thread.
			return (error(THRCRTERRORMSG, data));
	}
	while (++i < data->number_of_philosophers)
	{
		if (pthread_create(&data->tid[i], NULL, &routine, &data->philos[i]))//her philo icin bir mutex acilir ve yeme,uyku,dusunme durumu icin bir thread baslatilir.
			return (error(THRCRTERRORMSG, data));
		ft_usleep(1);
	}
	i = -1;
	while (++i < data->number_of_philosophers)
	{
		if (pthread_join(data->tid[i], NULL))
			return (error(THRJOINERRORMSG, data));
	}
	return (0);
}

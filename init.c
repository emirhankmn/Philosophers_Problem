/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eakman <eakman@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 15:56:14 by eakman            #+#    #+#             */
/*   Updated: 2024/02/18 14:52:27 by eakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdbool.h>

void	init_philo(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->number_of_philosophers)
	{
		//her filozof icin gereken bilgileri tanimliyoruz yemek yeme sureleri olum sureleri uyku sureleri gibi.
		data->philos[i].data = data;//Her bir felsefecinin data alanı, veri yapısını işaret eder. Böylece felsefeci, genel veri yapısına erişebilir.
		data->philos[i].id = i + 1;// Her bir felsefecinin kimliği atanır. Kimlikler, 1'den başlayarak sıralı olarak atanır.
		data->philos[i].dead_time = data->time_to_die;//Her bir felsefecinin ölüm zamanı, genel ölüm zamanına atanır. Bu, her bir felsefecinin kaç milisaniye içinde öleceğini belirler.
		data->philos[i].eat_cont = 0;//Her bir felsefecinin yemek yeme sayısı sıfırlanır. Başlangıçta hiç yemek yememişlerdir.
		data->philos[i].eating = 0;//Her bir felsefecinin yemek yeme durumu sıfırlanır. Başlangıçta hiçbir felsefeci yemek yememektedir.
		data->philos[i].status = 0;//Her bir felsefenin durumu sıfırlanır. Başlangıçta hiçbir felsefe ölmemiştir.
		pthread_mutex_init(&data->philos[i].lock, NULL);//Her bir felsefecinin kilitlenmesi başlatılır. Bu kilitler, felsefecilerin aynı anda aynı kaynaklara erişmesini engeller ve senkronizasyonu sağlar.
		i++;
	}
}

int	init_forks(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->number_of_philosophers)
	{
		//her fork(catal) icin bir mutexe ihtiyacimiz var boylelikle catallara ayni anda erisimde bulunulma durumunu kontrol edecegiz.
		pthread_mutex_init(&data->forks[i], NULL);
	}
	data->philos[0].l_fork = &data->forks[0];//sol catalin baslanhic noktasini tanimloyoruz.
	data->philos[0].r_fork = &data->forks[data->number_of_philosophers -1];//sag catalin baslangic noktasini tanimloyoruz.
	i = 1;
	while (i < data->number_of_philosophers)
	{
		//bu dongude her bir filozofun sol ve sag catallarinin hangisi oldugunu tanimliyoruz.
		data->philos[i].l_fork = &data->forks[i];
		data->philos[i].r_fork = &data->forks[i - 1];
		i++;
	}
	return (0);
}

int	init_alloc(t_data *data)
{
	//tid dizisi, her bir felsefecinin (philosopher) iş parçacığı kimliğini taşır.
	data->tid = malloc(sizeof(pthread_t) * data->number_of_philosophers);
	if (!data->tid)
		return (error(ALLOCERRORMSG1, data));
	//forklar(catallar) icin bir yer aciyoruz. 
	data->forks = malloc(sizeof(pthread_mutex_t)
			* data->number_of_philosophers);
	if (!data->forks)
		return (error(ALLOCERRORMSG2, data));
	//her philonun bilgilerinin olacagi bir dizi acmamiz gerek.
	data->philos = malloc(sizeof(t_philo) * data->number_of_philosophers);
	if (!data->philos)
		return (error(ALLOCERRORMSG3, data));
	return (0);
}

int	init_data(t_data *data, int ac, char **av)
{
	data->number_of_philosophers = (int) ft_atoi(av[1]);
	data->time_to_die = (u_int64_t) ft_atoi(av[2]);
	data->time_to_eat = (u_int64_t) ft_atoi(av[3]);
	data->time_to_sleep = (u_int64_t) ft_atoi(av[4]);
	if (ac == 6)
		data->must_to_eat = (int) ft_atoi(av[5]);
	else
		data->must_to_eat = -1;
	data->dead = 0;
	data->finished = 0;
	if (data->number_of_philosophers <= 0 || data->number_of_philosophers > 200
		|| data->time_to_die < 0 || data->time_to_eat < 0
		|| data->time_to_sleep < 0)
		return (printf("%s", ERRORMSG3), 0);
	//POSIX standartlarına uygun olarak iş parçacıkları arasında eşzamanlılık (concurrency) kontrolü sağlamak için bir mutex (kilit) nesnesini başlatır.
	pthread_mutex_init(&data->write, NULL);//yazma islemi icin bir mutex acmamiz gerek.
	pthread_mutex_init(&data->lock, NULL);//philolarin erisimlerini sinirlandirmak icin bir mutex olusturmamiz gerek.
	return (0);
}

int	init(t_data *data, int ac, char **av)
{
	if (init_data(data, ac, av))
		return (1);
	if (init_alloc(data))
		return (1);
	if (init_forks(data))
		return (1);
	init_philo(data);
	return (0);
}

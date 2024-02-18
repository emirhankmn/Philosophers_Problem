/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eakman <eakman@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 16:18:44 by eakman            #+#    #+#             */
/*   Updated: 2024/02/18 21:33:51 by eakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <unistd.h>

int	only_one(t_data	*data)
{
	data->start_time = get_time();//oncelikle baslangic zamani alinir.
	if (pthread_create(&data->tid[0], NULL, &routine, &data->philos[0]))
		return (error(THRCRTERRORMSG, data), 0);
	//fonksiyon, yeni bir thread oluşturmak için kullanılır. 
	//İlk parametre olarak oluşturulan thread'in id'sini belirtiriz. 
	//İkinci parametre genellikle NULL olarak bırakılır ve thread'in varsayılan özelliklerini kullanmasını sağlar. 
	//Üçüncü parametre, thread'in hangi fonksiyonu çağıracağını belirtir.
	//Dördüncü parametre ise bu fonksiyona verilecek olan argümandır.
	pthread_detach(data->tid[0]);//oluşturulan thread'in sonlanmasını beklemeksizin kaynaklarını serbest bırakır.
	while (data->dead == 0)
		ft_usleep(0);
	ft_exit(data);
	return (0);
}

int	ft_usleep(u_int64_t time)//yeme uyuma gibi sureleri tutar aslinda diger threadlari bekletir.
{
	u_int64_t	start;

	start = get_time();
	while ((get_time() - start) < time)
	{
		usleep(time / 10);
	}
	return (0);
}

u_int64_t	get_time(void)
{
	struct timeval	time;

	if(gettimeofday(&time, NULL))//o anki zamani alir.
		return (error(GETTIMOFDAYFAIL, NULL));
	return ((time.tv_usec / 1000) + (time.tv_sec * 1000));//zamani mili saniye olarak ddondurur.(sec--saniye usec--milisaniye)
}

int	check_args(int ac, char **av)
{
	int	i;
	int	q;

	if (ac < 5 || ac > 6)//Argumanalrin sayisini kontrol ediyorum.
		return (printf("%s", ERRORMSG1), 1);
	i = 1;
	while (av[i])
	{
		q = 0;
		while (av[i][q])
		{
			if (av[i][q] == ' ')
			{
				q++;
				continue ;
			}
			if (av[i][q] < 48 || av[i][q] > 57)//Argumanlarin icerisinde sayi disinda bir sey var mi diye bakiyorum.
				return (printf("%s", ERRORMSG2), 1);
			q++;
		}
		i++;
	}
	return (0);
}

int	main(int ac, char **av)
{
	t_data	data;

	if (check_args(ac, av))//Arguman kontrolu yapiyorum.
		return (1);
	if (init(&data, ac, av))//yer aciyorum.
		return (1);
	if (data.number_of_philosophers == 1)//Yalnizca 1 filozof olma durumuna bakiyorum.
		return (only_one(&data));
	if (thread_init(&data))
		return (1);
	ft_exit(&data);
	return (0);
}

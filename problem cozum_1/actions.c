/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eakman <eakman@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 18:17:51 by eakman            #+#    #+#             */
/*   Updated: 2024/06/02 14:14:41 by eakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	messages(char *str, t_philo *philo)
{
	u_int64_t	time;

	pthread_mutex_lock(&philo->data->write);//yazma kismina erisim saglanip thredlerin mesajlari karismasin diye kilitlenir.
	time = get_time() - philo->data->start_time;//baslangictan su an cikarilarak gecen zaman bulunur.
	if (ft_strcmp(DIED, str) == 0 && philo->data->dead == 0)//eger olum durumu varsa girilir.
	{
		printf("%llu %d %s\n", time, philo->id, str);//string yazdirilir.
		philo->data->dead = 1;//olum oldugu eklenir ve tum threadlardeki donguler sona erer.
	}
	if (!philo->data->dead)//olum yoksa ve dongu devam ediyorsa mesaj yazdirilir.
		printf("%llu %d %s\n", time, philo->id, str);
	pthread_mutex_unlock(&philo->data->write);//yazmanin kilidi acilir.
}

void	drop_forks(t_philo *philo)
{
	pthread_mutex_unlock(philo->l_fork);//sol catal birakilir.
	pthread_mutex_unlock(philo->r_fork);//sag catal birakilir.
	messages(SLEEPING, philo);//filozof uyur ve bunu terminale yazdiracak fonksiyona gonderir.
	ft_usleep(philo->data->time_to_sleep);//uyunan sure beklenir.
}

void	take_forks(t_philo *philo)
{
	pthread_mutex_lock(philo->r_fork);//ilk sag alinir ve kilitlenir.
	messages(TAKE_FORK, philo);//alindigina dair string terminale yazilir.
	pthread_mutex_lock(philo->l_fork);//sol catal alinir.
	messages(TAKE_FORK, philo);//catalin alinmasi ile ilgili mesaj yazilir.
}

void	eat(t_philo *philo)
{
	take_forks(philo);//catallar alinir.
	pthread_mutex_lock(&philo->lock);//diger threadlar erisemesin diye kilitlenir.
	philo->eating = 1;//su an yemek yendigini gosterir.
	philo->dead_time = get_time() + philo->data->time_to_die;//
	messages(EATING, philo);//yemek yendi mesaji yazar.
	philo->eat_cont++;//yemek tuketildigi icin arttirilir.
	ft_usleep(philo->data->time_to_eat);//yemek yerken tutuan catallar belirli bir zaman ortada olmadigindan digerleri onu bekledi.
	philo->eating = 0;//yemegin yenmedigini gosterir.
	pthread_mutex_unlock(&philo->lock);//diger threadlarda artik erisebilir.
	drop_forks(philo);//catallar birakilir.
}

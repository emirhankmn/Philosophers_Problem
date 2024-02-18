/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eakman <eakman@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 16:22:48 by eakman            #+#    #+#             */
/*   Updated: 2024/02/18 17:52:11 by eakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <sys/time.h>
# include <stdlib.h>

//arguman sayisi mesaji
# define ERRORMSG1 "Error;number of arguments is incorrect"
//arguman karakter mesaji
# define ERRORMSG2 "Arguments contain unwanted expressions"
//argumanlarin sayi degerleri hatali
# define ERRORMSG3 "Argument number values are incorrect"
//philo malloc yer acma hatasi
# define ALLOCERRORMSG1 "Error making space for Philo in malloc"
//fork malloc yer acma hata durumu
# define ALLOCERRORMSG2 "Error making space for Fork in malloc"
//info malloc yer acma hata durumu
# define ALLOCERRORMSG3 "Error making space for Info in malloc"
//thread olusturmada hata durumu
# define THRCRTERRORMSG	"Error while creating thread"
//thread JOIN hata durumu
# define THRJOINERRORMSG "Error while join thread"
//gettimofday() fonsiyonun hatali calisma durumu
# define GETTIMOFDAYFAIL "gettimofday() the function failed to work error"
// fork alindi durumu
# define TAKE_FORK "has taken a fork"
//olum durumu
# define DIED "is dead"
//yemek yeme durumu
# define EATING "is eating"
//uyuma durumu
# define SLEEPING "is sleeping"
//dusunme durumu
# define THINKING "is thinking"

typedef struct s_philo
{
	struct s_data		*data;
	pthread_t			t1;
	int					id;
	int					eat_cont;//filozofun yemek yeme sayisi
	int					status;//olum durumu
	int					eating;//suan yemek yiyor mu
	u_int64_t			dead_time;//olum suresi oldu mu
	pthread_mutex_t		lock;// forklar icin erisim sinirlama icin mutex
	pthread_mutex_t		*r_fork;//sag catal
	pthread_mutex_t		*l_fork;//sol catal
}	t_philo;

typedef struct s_data
{
	pthread_t			*tid;
	int					number_of_philosophers;//filozof sayisi
	int					must_to_eat;//kac kez yemek yicekler
	int					dead;//olen biri var miiiii
	int					finished;//bitme durumu
	u_int64_t			time_to_die;//flzflrn olmdn once ymk ymlri gereken sure
	u_int64_t			time_to_eat;//filozoflarin ymk yerken gecirdikleri sure
	u_int64_t			time_to_sleep;//filozofun uyurken gecirdigi zaman
	u_int64_t			start_time;//baslangic zamani
	pthread_mutex_t		*forks;//catallar
	pthread_mutex_t		write;//yazma icin mutex
	pthread_mutex_t		lock;//filozoflar icin kilit/erisim sinirlama icin mutex
	t_philo				*philos;
}	t_data;

//baslangic zamanini aliyor
u_int64_t	get_time(void);
//arguman karakter ve sayisi kontrolu
int		check_args(int ac, char **av);
//tanimlama-yer acma
int		init(t_data *data, int ac, char **av);
//atoi
int		ft_atoi(const char *str);
//bellirli bir sure uyutma
int		ft_usleep(u_int64_t time);
//string karsilastirmasi
int		ft_strcmp(char *s1, char *s2);
//olusturma/yer acma
int		init_alloc(t_data *data);
int		init_data(t_data *data, int ac, char **av);
int		init(t_data *data, int ac, char **av);
//hata
int		error(char *msg, t_data *data);
void	ft_exit(t_data	*data);
void	clear_data(t_data	*data);
//Bu fonksiyon, bir thread'in çalışma mantığını ve işlevini belirler.
void	*routine(void *philo_pointer);
int		thread_init(t_data *data);
//
void	messages(char *str, t_philo *philo);
void	eat(t_philo *philo);

#endif
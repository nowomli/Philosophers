/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inovomli <inovomli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 15:32:48 by inovomli          #+#    #+#             */
/*   Updated: 2023/01/30 16:17:13 by inovomli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <pthread.h>
# include <stdlib.h> 
# include <stdio.h>
# include <sys/time.h>

enum e_print_st{nothing = 0, take_frk, eating, sleeping, thinking, die};

typedef struct Philo
{
	int				all_ph_num;
	int				num_ph;
	int				eating_time;
	int				sleep_time;
	int				dying_time;
	long			last_eat_time;
	int				*is_eating;
	int				eat_cnt;
	long			start_time;
	pthread_t		id_philo_t;
	pthread_mutex_t	*is_eating_m;
	pthread_mutex_t	eat_count_m;
	pthread_mutex_t	*write_cn;
	pthread_mutex_t	*m_rgt_fork;
	pthread_mutex_t	*m_lft_fork;
	pthread_mutex_t	m_last_eat_time;

}	t_philo;

typedef struct Environment
{
	int				total_ph;
	long			start_time;
	int				is_eating;
	int				add_cnt;
	t_philo			*philos;
	pthread_mutex_t	write_cn_env;	
	pthread_mutex_t	*forks_m;
	pthread_mutex_t	is_eating_m;	
}	t_environment;

// creation
int		create_env(t_environment *new_env, char **argv);
int		create_forks(t_environment *env);
int		create_threads(t_environment *env, char **argv);
int		join_threads(t_environment *env);
int		create_philo(char **argv, t_environment *env, int num);

// feedthreads
void	fn_eating(t_philo *philo);
void	*exist(void *data);
void	diyng_loop(int i, t_environment	*env, long time);
int		change_state(t_philo *philo, enum e_print_st str);
int		new_diyng(void	*data);

// philo
int		ft_atoi(const char *str);
long	get_time(void);
void	new_usleep(long time);
void	free_all(t_environment *env);
int		main(int argc, char **argv);

// additional
int		alive(t_philo *philo);
int		show_eat_cnt(t_philo *philo);
int		sim_cont(t_environment *env);
int		check_all_eat(t_environment *env);

#endif

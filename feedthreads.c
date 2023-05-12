/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   feedthreads.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inovomli <inovomli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 20:31:43 by inovomli          #+#    #+#             */
/*   Updated: 2023/05/12 21:48:58 by inovomli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	fn_eating(t_philo *philo)
{
	change_state(philo, eating);
	new_usleep(philo->eating_time * 1000);
	pthread_mutex_unlock(philo->m_rgt_fork);
}

void	*exist(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	while (alive(philo) != 0 && show_eat_cnt(philo) != 0)
	{
		pthread_mutex_lock(&philo->eat_count_m);
		philo->eat_cnt -= 1;
		pthread_mutex_unlock(&philo->eat_count_m);
		if (change_state(philo, thinking))
		{
			pthread_mutex_lock(philo->m_lft_fork);
			if (change_state(philo, take_frk))
			{
				if (philo->all_ph_num == 1)
					return (0);
				pthread_mutex_lock(philo->m_rgt_fork);
				if (change_state(philo, take_frk))
					fn_eating(philo);
			}
			pthread_mutex_unlock(philo->m_lft_fork);
		}
		change_state(philo, sleeping);
		new_usleep(philo->sleep_time * 1000);
	}
	return (0);
}

void	diyng_loop(int i, t_environment	*env, long time)
{
	t_philo			*philo;

	while (i < env->total_ph)
	{
		philo = &env->philos[i];
		if (env->is_eating && show_eat_cnt(philo) != 0)
		{
			pthread_mutex_lock(&philo->m_last_eat_time);
			time = get_time();
			if (time - philo->last_eat_time > philo->dying_time)
			{
				change_state(philo, die);
				pthread_mutex_lock(philo->is_eating_m);
				env->is_eating = 0;
				pthread_mutex_unlock(philo->is_eating_m);
				pthread_mutex_unlock(&philo->m_last_eat_time);
				return ;
			}
			pthread_mutex_unlock(&philo->m_last_eat_time);
		}
		i++;
	}	
}

int	new_diyng(void	*data)
{
	t_environment	*env;
	long			time;
	int				i;

	env = (t_environment *)data;
	while (env->is_eating && check_all_eat(env))
	{
		i = 0;
		time = get_time();
		diyng_loop(i, env, time);
		if (check_all_eat(env) == 0)
			return (0);
	}
	return (0);
}

int	change_state(t_philo *philo, enum e_print_st str)
{
	long	time;
	long	print_t;

	time = get_time();
	print_t = time - philo->start_time;
	if (!alive(philo))
		return (0);
	pthread_mutex_lock(philo->write_cn);
	if (str == eating && alive(philo))
	{
		pthread_mutex_lock(&philo->m_last_eat_time);
		philo->last_eat_time = time;
		pthread_mutex_unlock(&philo->m_last_eat_time);
		printf("%ld %d %s", print_t, philo->num_ph, " is eating\n");
	}
	else if (str == take_frk && alive(philo))
		printf("%ld %d %s", print_t, philo->num_ph, " has taken a fork\n");
	else if (str == sleeping && alive(philo))
		printf("%ld %d %s", print_t, philo->num_ph, " is sleeping\n");
	else if (str == thinking && alive(philo))
		printf("%ld %d %s", print_t, philo->num_ph, " is thinking\n");
	else if (str == die && alive(philo))
		printf("%ld %d %s", print_t, philo->num_ph, " died\n");
	pthread_mutex_unlock(philo->write_cn);
	return (1);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   creation.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inovomli <inovomli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 20:25:41 by inovomli          #+#    #+#             */
/*   Updated: 2023/05/12 21:49:01 by inovomli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	create_env(t_environment *new_env, char **argv)
{
	new_env->is_eating = 1;
	new_env->add_cnt = -1;
	new_env->start_time = get_time();
	new_env->total_ph = ft_atoi(argv[1]);
	new_env->philos = malloc(sizeof(t_philo) * new_env->total_ph);
	new_env->forks_m = malloc(sizeof(pthread_mutex_t) * new_env->total_ph);
	if (!new_env->philos || !new_env->forks_m)
	{
		return (1);
	}
	pthread_mutex_init(&new_env->write_cn_env, 0);
	pthread_mutex_init(&new_env->is_eating_m, 0);
	return (0);
}

int	create_forks(t_environment *env)
{
	int	i;
	int	check;

	i = -1;
	while (++i < env->total_ph)
	{
		check = pthread_mutex_init(&env->forks_m[i], 0);
		if (check)
			return (1);
	}
	return (0);
}

int	create_threads(t_environment *env, char **argv)
{
	int	i;

	i = 0;
	while (i < env->total_ph)
	{
		create_philo(argv, env, i);
		if (pthread_create(&env->philos[i].id_philo_t,
				0, exist, &env->philos[i]))
			return (1);
		usleep(50);
		i += 2;
	}
	i = 1;
	usleep(50);
	while (i < env->total_ph)
	{
		create_philo(argv, env, i);
		if (pthread_create(&env->philos[i].id_philo_t,
				0, exist, &env->philos[i]))
			return (1);
		usleep(50);
		i += 2;
	}	
	return (0);
}

int	join_threads(t_environment *env)
{
	int	i;
	int	check;

	i = -1;
	while (++i < env->total_ph)
	{	
		check = pthread_join(env->philos[i].id_philo_t, 0);
		usleep(50);
		if (check)
			return (1);
	}
	return (0);
}

int	create_philo(char **argv, t_environment *env, int num)
{
	env->philos[num].all_ph_num = env->total_ph;
	env->philos[num].num_ph = num + 1;
	env->philos[num].dying_time = ft_atoi(argv[2]);
	env->philos[num].eating_time = ft_atoi(argv[3]);
	env->philos[num].sleep_time = ft_atoi(argv[4]);
	env->philos[num].eat_cnt = -1;
	if (argv[5])
		env->philos[num].eat_cnt = ft_atoi(argv[5]);
	env->philos[num].is_eating = &env->is_eating;
	env->philos[num].is_eating_m = &env->is_eating_m;
	env->philos[num].last_eat_time = get_time();
	env->philos[num].start_time = env->start_time;
	env->philos[num].write_cn = &env->write_cn_env;
			env->philos[num].m_lft_fork = &env->forks_m[num];
	if (env->philos[num].num_ph == env->total_ph)
		env->philos[num].m_rgt_fork = &env->forks_m[0];
	else
		env->philos[num].m_rgt_fork = &env->forks_m[num + 1];
	pthread_mutex_init(&env->philos[num].m_last_eat_time, 0);
	pthread_mutex_init(&env->philos[num].eat_count_m, 0);
	return (0);
}

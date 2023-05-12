/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   additional.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inovomli <inovomli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/27 11:33:40 by inovomli          #+#    #+#             */
/*   Updated: 2023/01/29 21:51:19 by inovomli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	alive(t_philo *philo)
{
	int	*res;

	res = 0;
	pthread_mutex_lock(philo->is_eating_m);
	res = philo->is_eating;
	pthread_mutex_unlock(philo->is_eating_m);
	return (*res);
}

int	sim_cont(t_environment *env)
{
	int	res;

	res = 0;
	pthread_mutex_lock(&env->is_eating_m);
	res = env->is_eating;
	pthread_mutex_unlock(&env->is_eating_m);
	return (res);
}

int	show_eat_cnt(t_philo *philo)
{
	int	res;

	res = 0;
	pthread_mutex_lock(&philo->eat_count_m);
	res = philo->eat_cnt;
	pthread_mutex_unlock(&philo->eat_count_m);
	return (res);
}

int	check_all_eat(t_environment *env)
{
	int		i;
	t_philo	*philo;

	i = 0;
	while (i < env->total_ph)
	{
		philo = &env->philos[i];
		if (show_eat_cnt(philo) == 0)
			i++;
		else
			return (1);
	}
	return (0);
}

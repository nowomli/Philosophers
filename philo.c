/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inovomli <inovomli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/17 15:28:49 by inovomli          #+#    #+#             */
/*   Updated: 2023/01/30 16:15:46 by inovomli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_atoi(const char *str)
{
	long	num;
	int		i;
	int		mult;

	num = 0;
	i = 0;
	mult = 1;
	while (str[i] == ' ' || str[i] == '\n' || str[i] == '\t'
		|| str[i] == '\v' || str[i] == '\f' || str[i] == '\r')
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			mult = -mult;
		i++;
	}
	while ((str[i]) && ((str[i] >= '0') && (str[i] <= '9')))
	{
		num = num * 10 + (str[i] - '0');
		i++;
	}
	return ((int)(num * mult));
}

void	free_all(t_environment *env)
{
	int	i;

	i = -1;
	while (++i < env->total_ph)
		pthread_mutex_destroy(&env->forks_m[i]);
	free(env->forks_m);
	free(env->philos);
}

long	get_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

void	new_usleep(long time)
{
	long	start_time;

	start_time = get_time();
	while (1)
	{
		if ((get_time() - start_time) * 1000 >= time)
			break ;
		usleep(500);
	}
}

int	main(int argc, char **argv)
{
	t_environment	env;

	if (!(argc == 5 || argc == 6))
		return (1);
	if (create_env(&env, argv))
		return (1);
	if (create_forks(&env) || create_threads(&env, argv)
		|| new_diyng(&env) || join_threads(&env))
	{
		free_all(&env);
		return (1);
	}
	free_all(&env);
	return (0);
}

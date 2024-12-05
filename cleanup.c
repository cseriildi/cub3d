/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 10:51:15 by icseri            #+#    #+#             */
/*   Updated: 2024/12/05 11:48:36 by icseri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	free_array(char ***arr)
{
	int	i;

	i = 0;
	if (arr && *arr)
	{
		while ((*arr)[i])
			ft_free(&(*arr)[i++]);
		free(*arr);
		*arr = NULL;
	}
}

void	free_list(t_list **list)
{
	t_list	*current;
	t_list	*next;

	if (list && *list)
	{
		current = *list;
		while (current != NULL)
		{
			next = current->next;
			ft_free((char**)&current->content);
			free(current);
			current = next;
		}
		*list = NULL;
	}
	free(list);
}

void	print_error(int count, ...)
{
	va_list	args;
	char	*str;

	va_start(args, count);
	while (count--)
	{
		str = va_arg(args, char *);
		if (str)
			ft_putstr_fd(str, STDERR_FILENO);
	}
	va_end(args);
	ft_putstr_fd("\n", STDERR_FILENO);
}

void	safe_exit(t_data *data, int exit_code)
{
	ft_free(&data->north);
	ft_free(&data->south);
	ft_free(&data->west);
	ft_free(&data->east);
	ft_free(&data->floor);
	ft_free(&data->ceiling);
	free_array(&data->map);
	if (data->fd != -1)
	{
		close(data->fd);
		data->fd = -1;
	}
	exit(exit_code);
}

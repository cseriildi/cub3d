/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcsicsak <dcsicsak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 10:51:15 by icseri            #+#    #+#             */
/*   Updated: 2024/12/05 14:04:53 by dcsicsak         ###   ########.fr       */
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

void	safe_exit(t_map *map, int exit_code)
{
	(void)exit_code;
	ft_free(&map->north);
	ft_free(&map->south);
	ft_free(&map->west);
	ft_free(&map->east);
	ft_free(&map->floor);
	ft_free(&map->ceiling);
	free_array(&map->map);
	if (map->fd != -1)
	{
		close(map->fd);
		map->fd = -1;
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 10:51:15 by icseri            #+#    #+#             */
/*   Updated: 2024/12/12 16:46:34 by icseri           ###   ########.fr       */
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

void	free_texture(t_sprite *texture)
{
	int	i;

	if (texture)
	{
/* 		i = 0;
		if (texture->textures)
		{
			while (texture->textures[i])
				ft_free(&texture->textures[i++]);
			free(texture->textures);
			texture->textures = NULL;
		} */
		i = 0;
		if (texture->sizes)
		{
			while (i < texture->count)
				free(texture->sizes[i++]);
			free(texture->sizes);
			texture->sizes = NULL;
		}
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
			ft_free((char **)&current->content);
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

void	errors(int exit_code)
{
	if (exit_code == MALLOC)
		print_error(1, "Error\nMalloc fail");
	else if (exit_code == COLOR)
		print_error(1, "Error\nInvalid color");
	else if (exit_code == TEXTURE)
		print_error(1, "Error\nInvalid texture");
	else if (exit_code == MAP)
		print_error(1, "Error\nInvalid map");
}

void	safe_exit(t_map *map, int exit_code)
{
	free_array(&map->map);
	free_texture(&map->north);
	free_texture(&map->east);
	free_texture(&map->west);
	free_texture(&map->south);
	free_texture(&map->door);

	int i = 0;
	if (map->data->textures)
	{
		while (i < 5 && map->data->textures[i])
		{
			if (map->data->mlx && map->data->textures[i]->img)
				mlx_destroy_image(map->data->mlx, map->data->textures[i]->img);
			free(map->data->textures[i]);
			i++;
		}
		free(map->data->textures);
	}
	if (map->data->img)
		mlx_destroy_image(map->data->mlx, map->data->img);
	if (map->data->win)
		mlx_destroy_window(map->data->mlx, map->data->win);
	if (map->data->mlx)
	{
		mlx_destroy_display(map->data->mlx);
		free(map->data->mlx);
	}
	if (map->fd != -1)
		close(map->fd);
	get_next_line(-1);
	errors(exit_code);
	exit(exit_code);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 10:51:15 by icseri            #+#    #+#             */
/*   Updated: 2024/12/13 13:47:53 by icseri           ###   ########.fr       */
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

void	free_mlx(t_data *data)
{
	int	i;
	int	j;

	if (data->textures)
	{
		i = -1;
		while (++i < 5 && data->textures[i])
		{
			j = -1;
			while (data->mlx && data->textures[i][++j].img != NULL)
				mlx_destroy_image(data->mlx, data->textures[i][j].img);
			free(data->textures[i]);
		}
		free(data->textures);
	}
	if (data->img)
		mlx_destroy_image(data->mlx, data->img);
	if (data->win)
		mlx_destroy_window(data->mlx, data->win);
	if (data->mlx)
	{
		mlx_destroy_display(data->mlx);
		free(data->mlx);
	}
}

void	safe_exit(t_map *map, int exit_code)
{
	free_array(&map->map);
	free_texture(&map->north);
	free_texture(&map->east);
	free_texture(&map->west);
	free_texture(&map->south);
	free_texture(&map->door);
	free_mlx(map->data);
	if (map->fd != -1)
		close(map->fd);
	get_next_line(-1);
	errors(exit_code);
	exit(exit_code);
}

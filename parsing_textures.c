/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_textures.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcsicsak <dcsicsak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 12:34:28 by icseri            #+#    #+#             */
/*   Updated: 2024/12/11 09:55:19 by dcsicsak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	get_texture(char *line, char **texture, t_map *map)
{
	if (*texture || ft_strlen(line) <= 3)
		return (ft_free(&line), safe_exit(map, TEXTURE));
	*texture = ft_substr(line, 3, ft_strlen(line) - 4);
	ft_free(&line);
	if (!*texture)
		safe_exit(map, MALLOC);
}

void	set_size(int *size, char *texture, t_map *map)
{
	char	*line;
	char	**numbers;

	safe_open(texture, false, map);
	line = get_next_line(map->fd);
	ft_free(&line);
	line = get_next_line(map->fd);
	ft_free(&line);
	line = get_next_line(map->fd);
	close(map->fd);
	if (!line || ft_strlen(line) < 10)
		return (ft_free(&line), safe_exit(map, TEXTURE));
	numbers = ft_split(line + 1, ' ');
	ft_free(&line);
	if (!numbers)
		safe_exit(map, MALLOC);
	size[0] = ft_atoi(numbers[0]);
	size[1] = ft_atoi(numbers[1]);
	free_array(&numbers);
}

void	check_textures(t_map *map)
{
	if (map->ceiling < 0 || map->floor < 0)
		safe_exit(map, COLOR);
	if (!map->east || !map->west || !map->north || !map->south)
		safe_exit(map, TEXTURE);
	set_size(map->n_size, map->north, map);
	set_size(map->s_size, map->south, map);
	set_size(map->w_size, map->west, map);
	set_size(map->e_size, map->east, map);
	if (map->door)
		set_size(map->d_size, map->door, map);
}

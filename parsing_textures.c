/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_textures.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcsicsak <dcsicsak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 12:34:28 by icseri            #+#    #+#             */
/*   Updated: 2024/12/11 12:35:00 by dcsicsak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	*ft_realloc_array(void *array, size_t old_count, size_t new_count, size_t elem_size)
{
	void	*new_array;

	new_array = malloc(new_count * elem_size);
	if (!new_array)
		return (NULL);
	if (array)
	{
		ft_memcpy(new_array, array, old_count * elem_size);
		free(array);
	}
	return (new_array);
}

void	get_texture(char *line, char ***textures, int ***sizes, int *count, t_map *map)
{
	char	*new_texture;

	if (ft_strlen(line) <= 3)
		return (ft_free(&line), safe_exit(map, TEXTURE));
	new_texture = ft_substr(line, 3, ft_strlen(line) - 4);
	if (!new_texture)
		return (ft_free(&line), safe_exit(map, MALLOC));
	*textures = ft_realloc_array(*textures, *count, *count + 1, sizeof(char *));
	*sizes = ft_realloc_array(*sizes, *count, *count + 1, sizeof(int *));
	if (!*textures || !*sizes)
	{
		ft_free(&new_texture);
		safe_exit(map, MALLOC);
	}
	(*textures)[*count] = new_texture;
	(*sizes)[*count] = malloc(sizeof(int) * 2);
	if (!(*sizes)[*count])
		safe_exit(map, MALLOC);
	(*sizes)[*count][0] = 0;
	(*sizes)[*count][1] = 0;
	(*count)++;
	ft_free(&line);
}

void	set_sizes(int **sizes, char **textures, int count, t_map *map)
{
	int i;
	char *line;
	char **numbers;

	i = 0;
	while (i < count)
	{
		safe_open(textures[i], false, map);
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
		sizes[i][0] = ft_atoi(numbers[0]);
		sizes[i][1] = ft_atoi(numbers[1]);
		free_array(&numbers);
		i++;
	}
}

void check_textures(t_map *map)
{
	if (map->ceiling < 0 || map->floor < 0)
		safe_exit(map, COLOR);
	if (!map->north || !map->south || !map->west || !map->east)
		safe_exit(map, TEXTURE);
	set_sizes(map->n_size, map->north, map->north_count, map);
	set_sizes(map->s_size, map->south, map->south_count, map);
	set_sizes(map->w_size, map->west, map->west_count, map);
	set_sizes(map->e_size, map->east, map->east_count, map);
	if (map->door)
		set_sizes(map->d_size, map->door, map->door_count, map);
}

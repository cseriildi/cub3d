/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_textures.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 12:34:28 by icseri            #+#    #+#             */
/*   Updated: 2024/12/12 15:43:11 by icseri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	*ft_realloc_array(void *array, size_t old_count, size_t new_count,
	size_t elem_size)
{
	void	*new_array;

	new_array = malloc(new_count * elem_size);
	if (!new_array)
		return (NULL);
	if (array)
	{
		ft_memcpy(new_array, array, old_count * elem_size);
		free_array(array);
	}
	return (new_array);
}

void	get_texture(char *line, t_sprite *sprite, t_map *map)
{
	char	*new_texture;

	if (ft_strlen(line) <= 3)
		return (ft_free(&line), safe_exit(map, TEXTURE));
	new_texture = ft_substr(line, 3, ft_strlen(line) - 4);
	ft_free(&line);
	sprite->textures = ft_realloc_array(sprite->textures, sprite->count,
			sprite->count + 1, sizeof(char *));
	sprite->sizes = ft_realloc_array(sprite->sizes, sprite->count,
			sprite->count + 1, sizeof(int *));
	if (!sprite->textures || !sprite->sizes || !new_texture)
	{
		ft_free(&new_texture);
		safe_exit(map, MALLOC);
	}
	sprite->textures[sprite->count] = new_texture;
	sprite->sizes[sprite->count] = malloc(sizeof(int) * 2);
	if (!sprite->sizes[sprite->count])
		safe_exit(map, MALLOC);
	sprite->sizes[sprite->count][0] = 0;
	sprite->sizes[sprite->count][1] = 0;
	sprite->count++;
}

void	set_sizes(int **sizes, char **textures, int count, t_map *map)
{
	int		i;
	char	*line;
	char	**numbers;
	int		fd;

	i = -1;
	while (++i < count)
	{
		fd = safe_open(textures[i], false, map);
		line = get_next_line(fd);
		ft_free(&line);
		line = get_next_line(fd);
		ft_free(&line);
		line = get_next_line(fd);
		close(fd);
		if (!line || ft_strlen(line) < 10)
			return (ft_free(&line), safe_exit(map, TEXTURE));
		numbers = ft_split(line + 1, ' ');
		ft_free(&line);
		if (!numbers)
			safe_exit(map, MALLOC);
		sizes[i][0] = ft_atoi(numbers[0]);
		sizes[i][1] = ft_atoi(numbers[1]);
		free_array(&numbers);
	}
}

void	check_textures(t_map *map)
{
	if (map->ceiling < 0 || map->floor < 0)
		safe_exit(map, COLOR);
	if (!map->north.textures || !map->south.textures
		|| !map->west.textures || !map->east.textures)
		safe_exit(map, TEXTURE);
	set_sizes(map->north.sizes, map->north.textures, map->north.count, map);
	set_sizes(map->south.sizes, map->south.textures, map->south.count, map);
	set_sizes(map->west.sizes, map->west.textures, map->west.count, map);
	set_sizes(map->east.sizes, map->east.textures, map->east.count, map);
	if (map->door.textures)
		set_sizes(map->door.sizes, map->door.textures, map->door.count, map);
	set_counts(map->data);
	allocate_textures(map->data);
}

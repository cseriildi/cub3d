/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_textures.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 12:34:28 by icseri            #+#    #+#             */
/*   Updated: 2024/12/13 11:50:07 by icseri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	get_texture(char *line, t_sprite *sprite, t_map *map)
{
	char	*new_texture;
	t_list	*new_element;

	if (ft_strlen(line) <= 3)
		return (ft_free(&line), safe_exit(map, TEXTURE));
	new_texture = ft_substr(line, 3, ft_strlen(line) - 4);
	ft_free(&line);
	new_element = ft_lstnew(new_texture);
	if (!new_texture || !new_element)
	{
		ft_free(&new_texture);
		safe_exit(map, MALLOC);
	}
	ft_lstadd_back(&sprite->texture_list, new_element);
	sprite->count++;
}

void	set_sizes(t_sprite *sprite, t_map *map)
{
	int		i;
	char	*line;
	char	**numbers;
	int		fd;

	i = -1;
	while (sprite->textures && sprite->textures[++i])
	{
		fd = safe_open(sprite->textures[i], false, map);
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
		sprite->sizes[i][0] = ft_atoi(numbers[0]);
		sprite->sizes[i][1] = ft_atoi(numbers[1]);
		free_array(&numbers);
	}
}

void	check_textures(t_map *map)
{
	if (map->ceiling < 0 || map->floor < 0)
		safe_exit(map, COLOR);
	if (!map->north.texture_list || !map->south.texture_list
		|| !map->west.texture_list || !map->east.texture_list)
		safe_exit(map, TEXTURE);
	set_counts(map->data);
	set_sizes(&map->north, map);
	set_sizes(&map->south, map);
	set_sizes(&map->west, map);
	set_sizes(&map->east, map);
	set_sizes(&map->door, map);
	list_to_arr(&map->north.texture_list, &map->north.textures, map);
	list_to_arr(&map->south.texture_list, &map->south.textures, map);
	list_to_arr(&map->west.texture_list, &map->west.textures, map);
	list_to_arr(&map->east.texture_list, &map->east.textures, map);
	list_to_arr(&map->door.texture_list, &map->door.textures, map);
}

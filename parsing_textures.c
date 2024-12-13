/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_textures.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 12:34:28 by icseri            #+#    #+#             */
/*   Updated: 2024/12/13 15:52:55 by icseri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	get_texture(t_sprite *sprite, t_map *map)
{
	char	*new_texture;
	t_list	*new_element;

	if (ft_strlen(map->data->line) <= 3)
		return (safe_exit(map, TEXTURE));
	new_texture = ft_substr(map->data->line, 3, ft_strlen(map->data->line) - 4);
	ft_free(&map->data->line);
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
	char	**numbers;
	int		fd;

	i = -1;
	while (sprite->textures && sprite->textures[++i])
	{
		fd = safe_open(sprite->textures[i], false, map);
		map->data->line = get_next_line(fd);
		ft_free(&map->data->line);
		map->data->line = get_next_line(fd);
		ft_free(&map->data->line);
		map->data->line = get_next_line(fd);
		close(fd);
		if (!map->data->line || ft_strlen(map->data->line) < 10)
			safe_exit(map, TEXTURE);
		numbers = ft_split(map->data->line + 1, ' ');
		ft_free(&map->data->line);
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

void	create_and_add(t_list **map_list, t_map *map)
{
	t_list	*new;
	char	*content;

	content = ft_strtrim(map->data->line, "\n");
	ft_free(&map->data->line);
	if (!content)
		return (ft_lstclear(map_list, &free), safe_exit(map, MALLOC));
	new = ft_lstnew(content);
	if (!new)
	{
		ft_free(&content);
		ft_lstclear(map_list, &free);
		safe_exit(map, MALLOC);
	}
	ft_lstadd_back(map_list, new);
}

void	allocate_textures(t_data *data)
{
	int	i;

	data->textures = ft_calloc(5, sizeof(t_texture *));
	if (!data->textures)
		safe_exit(&data->map, MALLOC);
	data->textures[0] = ft_calloc(data->map.north.count + 1, sizeof(t_texture));
	data->textures[1] = ft_calloc(data->map.east.count + 1, sizeof(t_texture));
	data->textures[2] = ft_calloc(data->map.south.count + 1, sizeof(t_texture));
	data->textures[3] = ft_calloc(data->map.west.count + 1, sizeof(t_texture));
	data->textures[4] = ft_calloc(data->map.door.count + 1, sizeof(t_texture));
	if (!data->textures[0] || !data->textures[1] || !data->textures[2]
		|| !data->textures[3] || !data->textures[4])
	{
		i = -1;
		while (++i < 5)
		{
			if (data->textures[i])
			{
				free(data->textures[i]);
				data->textures[i] = NULL;
			}
		}
		free(data->textures);
		return (data->textures = NULL, safe_exit(&data->map, MALLOC));
	}
}

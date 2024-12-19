/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 14:43:35 by icseri            #+#    #+#             */
/*   Updated: 2024/12/19 11:01:10 by icseri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	load_texture(t_data *data, t_texture *texture, char *file)
{
	texture->img = mlx_xpm_file_to_image(data->mlx, file,
			&texture->width, &texture->height);
	if (!texture->img)
	{
		print_error(1, "Error: Failed to load texture");
		safe_exit(&data->map, EXIT_FAILURE);
	}
	texture->data = (int *)mlx_get_data_addr(texture->img,
			&data->bpp, &data->line_len, &data->endian);
	if (!texture->data)
	{
		print_error(1, "Error: Failed to retrieve texture data");
		safe_exit(&data->map, EXIT_FAILURE);
	}
}

void	set_counts(t_data *data)
{
	data->frame_count[NORTH] = data->map.north.count;
	data->frame_count[EAST] = data->map.east.count;
	data->frame_count[SOUTH] = data->map.south.count;
	data->frame_count[WEST] = data->map.west.count;
	data->frame_count[DOOR] = data->map.door.count;
	if (!data->map.is_bonus)
	{
		if (data->map.door.count != 0 || data->map.west.count != 1
			|| data->map.east.count != 1 || data->map.south.count != 1
			|| data->map.north.count != 1)
			safe_exit(&data->map, MAP);
	}
	data->map.north.sizes = ft_calloc(data->map.north.count, sizeof(int *));
	data->map.east.sizes = ft_calloc(data->map.east.count, sizeof(int *));
	data->map.south.sizes = ft_calloc(data->map.south.count, sizeof(int *));
	data->map.west.sizes = ft_calloc(data->map.west.count, sizeof(int *));
	data->map.door.sizes = ft_calloc(data->map.door.count, sizeof(int *));
	if (!data->map.north.sizes || !data->map.east.sizes
		|| !data->map.south.sizes || !data->map.west.sizes
		|| !data->map.door.sizes)
		safe_exit(&data->map, MALLOC);
}

void	load_all_textures(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->map.north.count)
		load_texture(data, &data->textures[NORTH][i],
			data->map.north.textures[i]);
	i = -1;
	while (++i < data->map.east.count)
		load_texture(data, &data->textures[EAST][i],
			data->map.east.textures[i]);
	i = -1;
	while (++i < data->map.south.count)
		load_texture(data, &data->textures[SOUTH][i],
			data->map.south.textures[i]);
	i = -1;
	while (++i < data->map.west.count)
		load_texture(data, &data->textures[WEST][i],
			data->map.west.textures[i]);
	i = -1;
	while (++i < data->map.door.count)
		load_texture(data, &data->textures[DOOR][i],
			data->map.door.textures[i]);
}

static void	free_texture(t_sprite *texture)
{
	int	i;

	if (texture)
	{
		free_array(&texture->textures);
		i = -1;
		if (texture->sizes)
		{
			while (++i < texture->count)
				free(texture->sizes[i]);
			free(texture->sizes);
			texture->sizes = NULL;
		}
		ft_lstclear(&texture->texture_list, &free);
	}
}

void	free_textures(t_data *data)
{
	int	i;
	int	j;

	if (data->textures)
	{
		i = -1;
		while (++i < 5 && data->textures[i])
		{
			j = -1;
			while (data->textures[i][++j].img != NULL)
				mlx_destroy_image(data->mlx, data->textures[i][j].img);
			free(data->textures[i]);
		}
		free(data->textures);
	}
	free_texture(&data->map.north);
	free_texture(&data->map.east);
	free_texture(&data->map.west);
	free_texture(&data->map.south);
	free_texture(&data->map.door);
	ft_lstclear(&data->map.north.texture_list, &free);
	ft_lstclear(&data->map.east.texture_list, &free);
	ft_lstclear(&data->map.west.texture_list, &free);
	ft_lstclear(&data->map.south.texture_list, &free);
	ft_lstclear(&data->map.door.texture_list, &free);
}

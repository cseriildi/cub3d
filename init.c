/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 13:25:09 by icseri            #+#    #+#             */
/*   Updated: 2024/12/11 14:26:19 by dcsicsak         ###   ########.fr       */
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

void	allocate_textures(t_data *data)
{
	int	i;
	int	j;

	data->textures = malloc(5 * sizeof(t_texture *));
	if (!data->textures)
		safe_exit(&data->map, MALLOC);
	data->textures[0] = malloc(data->map.north.count * sizeof(t_texture));
	data->textures[1] = malloc(data->map.east.count * sizeof(t_texture));
	data->textures[2] = malloc(data->map.south.count * sizeof(t_texture));
	data->textures[3] = malloc(data->map.west.count * sizeof(t_texture));
	data->textures[4] = malloc(data->map.door.count * sizeof(t_texture));
	i = -1;
	while (++i < 5)
	{
		if (!data->textures[i])
		{
			j = -1;
			while (++j < i)
				free(data->textures[j]);
			free(data->textures);
			safe_exit(&data->map, MALLOC);
		}
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
	else if (data->map.door.count != 1)
		safe_exit(&data->map, MAP);
}

void	load_all_textures(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->map.north.count)
		load_texture(data, &data->textures[0][i], data->map.north.textures[i]);
	i = -1;
	while (++i < data->map.east.count)
		load_texture(data, &data->textures[1][i], data->map.east.textures[i]);
	i = -1;
	while (++i < data->map.south.count)
		load_texture(data, &data->textures[2][i], data->map.south.textures[i]);
	i = -1;
	while (++i < data->map.west.count)
		load_texture(data, &data->textures[3][i], data->map.west.textures[i]);
	i = -1;
	while (++i < data->map.door.count)
		load_texture(data, &data->textures[4][i], data->map.door.textures[i]);
	set_counts(data);
}

void	init_data(t_data *data)
{
	int	i;

	i = -1;
	while (++i < WIDTH)
		data->ray_dir[i] = -1;
	data->frame = 1;
	data->last_frame = 0;
	data->map = (t_map){0};
	data->rect = (t_rectangle){0};
	data->minimap = (t_minimap){0};
	data->map.fd = -1;
	data->map.ceiling = -1;
	data->map.floor = -1;
}

void	init_mlx(t_data *data)
{
	data->mlx = mlx_init();
	if (!data->mlx)
	{
		print_error(1, "Error: Failed to initialize mlx\n");
		safe_exit(&data->map, EXIT_FAILURE);
	}
	data->win = mlx_new_window(data->mlx, WIDTH, HEIGHT, "cub3d");
	if (!data->win)
	{
		mlx_destroy_display(data->mlx);
		free(data->mlx);
		print_error(1, "Error: Failed to create window\n");
		safe_exit(&data->map, EXIT_FAILURE);
	}
	data->img = mlx_new_image(data->mlx, WIDTH, HEIGHT);
	if (!data->img)
	{
		mlx_destroy_window(data->mlx, data->win);
		mlx_destroy_display(data->mlx);
		free(data->mlx);
		print_error(1, "Error: Failed to create image\n");
		safe_exit(&data->map, EXIT_FAILURE);
	}
	data->addr = mlx_get_data_addr(data->img,
			&data->bpp, &data->line_len, &data->endian);
}

void	set_player(t_data *data)
{
	data->player_x = data->map.player[1];
	data->player_y = data->map.player[0];
	if (data->map.map[data->map.player[0]][data->map.player[1]] == 'N')
		data->player_angle = 3 * M_PI / 2;
	else if (data->map.map[data->map.player[0]][data->map.player[1]] == 'S')
		data->player_angle = M_PI / 2;
	else if (data->map.map[data->map.player[0]][data->map.player[1]] == 'E')
		data->player_angle = 0;
	else if (data->map.map[data->map.player[0]][data->map.player[1]] == 'W')
		data->player_angle = M_PI;
}

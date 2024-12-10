/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcsicsak <dcsicsak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 13:25:09 by icseri            #+#    #+#             */
/*   Updated: 2024/12/10 09:59:53 by dcsicsak         ###   ########.fr       */
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

void	load_all_textures(t_data *data)
{
	load_texture(data, &data->textures[NORTH], data->map.north);
	load_texture(data, &data->textures[EAST], data->map.east);
	load_texture(data, &data->textures[SOUTH], data->map.south);
	load_texture(data, &data->textures[WEST], data->map.west);
	load_texture(data, &data->textures[4], "./textures/door.xpm");
}

void	init_data(t_data *data)
{
	int	i;

	i = -1;
	while (++i < WIDTH)
		data->ray_dir[i] = -1;
	data->textures[0] = (t_texture){0};
	data->textures[1] = (t_texture){0};
	data->textures[2] = (t_texture){0};
	data->textures[3] = (t_texture){0};
	data->map = (t_map){0};
	data->map.fd = -1;
	data->map.enemy[0] = -1;
	data->map.enemy[1] = -1;
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

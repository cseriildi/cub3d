/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 13:25:09 by icseri            #+#    #+#             */
/*   Updated: 2024/12/12 15:19:33 by icseri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	init_data(t_data *data)
{
	int	i;

	i = -1;
	while (++i < WIDTH)
		data->ray_dir[i] = -1;
	data->frame = 1;
	data->map = (t_map){0};
	data->rect = (t_rectangle){0};
	data->minimap = (t_minimap){0};
	data->map.north = (t_sprite){0};
	data->map.east = (t_sprite){0};
	data->map.south = (t_sprite){0};
	data->map.west = (t_sprite){0};
	data->map.door = (t_sprite){0};
	data->textures = (t_texture **){0};
	data->map.fd = -1;
	data->map.ceiling = -1;
	data->map.floor = -1;
	data->map.data = data;
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

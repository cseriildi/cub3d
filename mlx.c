/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcsicsak <dcsicsak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 19:36:26 by icseri            #+#    #+#             */
/*   Updated: 2024/12/19 07:53:57 by dcsicsak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"


double	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec + tv.tv_usec / 1000000.0);
}

void	update_animation_frame(t_data *data)
{
	double	current_time;

	current_time = get_time();
	if (current_time - data->last_frame >= FRAME_DURATION)
	{
		data->frame = (data->frame + 1) % NUM_FRAMES;
		data->last_frame = current_time;
	}
}

void	render_scene(t_data *data)
{
	int	x;
	int	wall_height;

	update_animation_frame(data);
	cast_rays(data);
	wall_height = 0;
	x = -1;
	while (++x < WIDTH)
	{
		if (data->ray_distance[x] > 0)
		{
			wall_height = (TILE_SIZE / data->ray_distance[x]) * (HEIGHT / 55);
			draw_vertical_line(data, x, wall_height);
		}
	}
	draw_minimap(data);
	mlx_put_image_to_window(data->mlx, data->win, data->img, 0, 0);
}

int	key_hook(int keycode, t_data *data)
{
	if (keycode == KEY_ESC)
		close_window(data);
	mlx_destroy_image(data->mlx, data->img);
	data->img = mlx_new_image(data->mlx, WIDTH, HEIGHT);
	if (!data->img)
	{
		print_error(1, "Error: Failed to create new image\n");
		close_window(data);
	}
	data->addr = mlx_get_data_addr(data->img, &data->bpp,
			&data->line_len, &data->endian);
	update_player_position(data, keycode);
	if (keycode == KEY_E)
		check_and_open_door_nearby(data, data->player_x + 0.5,
			data->player_y + 0.5);
	render_scene(data);
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 13:28:30 by icseri            #+#    #+#             */
/*   Updated: 2024/12/10 11:15:24 by icseri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	render_scene(t_data *data)
{
	int	x;
	int	wall_height;

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

int	close_window(t_data *data)
{
	if (data->img)
		mlx_destroy_image(data->mlx, data->img);
	if (data->win)
		mlx_destroy_window(data->mlx, data->win);
	if (data->mlx)
	{
		mlx_destroy_display(data->mlx);
		free(data->mlx);
	}
	safe_exit(&data->map, EXIT_SUCCESS);
	return (0);
}

int	track_mouse(void *param)
{
	t_data	*data;
	int		x;
	int		y;

	data = (t_data *)param;
	mlx_mouse_get_pos(data->mlx, data->win, &x, &y);
	if (x != WIDTH / 2)
		data->player_angle += (x - WIDTH / 2) * MOUSE_SENSITIVITY;
	data->player_angle = fmod(data->player_angle + 2 * M_PI, 2 * M_PI);
	mlx_mouse_move(data->mlx, data->win, WIDTH / 2, HEIGHT / 2);
	mlx_mouse_hide(data->mlx, data->win);
	mlx_destroy_image(data->mlx, data->img);
	data->img = mlx_new_image(data->mlx, WIDTH, HEIGHT);
	if (!data->img)
	{
		print_error(1, "Error: Failed to create new image\n");
		close_window(data);
	}
	data->addr = mlx_get_data_addr(data->img, &data->bpp,
			&data->line_len, &data->endian);
	render_scene(data);
	return (0);
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

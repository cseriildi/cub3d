/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcsicsak <dcsicsak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 13:28:30 by icseri            #+#    #+#             */
/*   Updated: 2024/12/19 08:16:45 by dcsicsak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
	char	*dst;

	if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
		return ;
	dst = data->addr + (y * data->line_len + x * (data->bpp / 8));
	*(unsigned int *)dst = color;
}

int	close_window(t_data *data)
{
	safe_exit(&data->map, EXIT_SUCCESS);
	return (0);
}

int	track_mouse(void *param)
{
	t_data		*data;
	int			x;
	int			y;
	int			delta_x;
	static int	prev_x = -1;

	data = (t_data *)param;
	mlx_mouse_get_pos(data->mlx, data->win, &x, &y);
	if (prev_x == -1)
		prev_x = x;
	delta_x = x - prev_x;
	prev_x = x;
	data->player_angle += delta_x * MOUSE_SENSITIVITY;
	turn_player(data, 0);
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

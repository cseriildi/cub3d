/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcsicsak <dcsicsak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 13:01:37 by icseri            #+#    #+#             */
/*   Updated: 2024/12/11 12:21:11 by dcsicsak         ###   ########.fr       */
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

void	draw_rectangle(t_data *data, t_rectangle *rect)
{
	int	i;
	int	j;

	j = 0;
	while (j <= rect->height)
	{
		i = 0;
		while (i <= rect->width)
		{
			if (j == 0 || j == rect->height || i == 0 || i == rect->width)
				my_mlx_pixel_put(data, rect->x + i, rect->y + j, GRID_COLOR);
			else
				my_mlx_pixel_put(data, rect->x + i, rect->y + j, rect->color);
			i++;
		}
		j++;
	}
}

static void	draw_line_with_params(t_data *data,
	t_line *line, t_line_params *params)
{
	while (1)
	{
		my_mlx_pixel_put(data, line->start_x, line->start_y, line->color);
		if (line->start_x == line->end_x && line->start_y == line->end_y)
			break ;
		params->error2 = params->error * 2;
		if (params->error2 > -params->delta_y)
		{
			params->error -= params->delta_y;
			line->start_x += params->step_x;
		}
		if (params->error2 < params->delta_x)
		{
			params->error += params->delta_x;
			line->start_y += params->step_y;
		}
	}
}

void	draw_line(t_data *data, t_line *line)
{
	t_line_params	params;

	params.delta_x = abs(line->end_x - line->start_x);
	params.delta_y = abs(line->end_y - line->start_y);
	if (line->start_x < line->end_x)
		params.step_x = 1;
	else
		params.step_x = -1;
	if (line->start_y < line->end_y)
		params.step_y = 1;
	else
		params.step_y = -1;
	params.error = params.delta_x - params.delta_y;
	draw_line_with_params(data, line, &params);
}

void	draw_vertical_line(t_data *data, int x, int wall_height)
{
	int			y;
	int			wall_top;
	int			wall_bottom;
	int			texture_y;
	double		step;
	double		texture_pos;
	int			original_wall_top;
	t_texture	*texture;

	if (data->ray_dir[x] < 0 || data->ray_dir[x] >= 5)
		data->ray_dir[x] = NORTH;
	texture = &data->textures[data->ray_dir[x]][data->frame % data->frame_count[data->ray_dir[x]]];
	wall_top = (HEIGHT - wall_height) / 2;
	wall_bottom = wall_top + wall_height - 1;
	original_wall_top = wall_top;
	if (wall_top < 0)
		wall_top = 0;
	if (wall_bottom >= HEIGHT)
		wall_bottom = HEIGHT - 1;
	step = (double)texture->height / wall_height;
	if (original_wall_top < 0)
		texture_pos = -original_wall_top * step;
	else
		texture_pos = 0;
	y = 0;
	while (y < wall_top)
		my_mlx_pixel_put(data, x, y++, data->map.ceiling);
	while (y <= wall_bottom)
	{
		texture_y = (int)texture_pos % texture->height;
		my_mlx_pixel_put(data, x, y++,
			texture->data[texture_y * texture->width
			+ ((int)(data->texture_x[x] * texture->width) % texture->width)]);
		texture_pos += step;
	}
	while (y < HEIGHT)
		my_mlx_pixel_put(data, x, y++, data->map.floor);
}

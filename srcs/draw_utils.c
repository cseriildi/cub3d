#include "../includes/cub3d.h"

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
	while (j < rect->height)
	{
		i = 0;
		while (i < rect->width)
		{
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

static void	generate_texture(int texture[TEXTURE_SIZE][TEXTURE_SIZE], int color)
{
	int		r;
	int		g;
	int		b;
	double	factor;
	int		tx;
	int		ty;

	r = (color >> 16) & 0xFF;
	g = (color >> 8) & 0xFF;
	b = color & 0xFF;
	ty = 0;
	while (ty < TEXTURE_SIZE)
	{
		tx = 0;
		while (tx < TEXTURE_SIZE)
		{
			factor = 1.0 - ((double)(tx + ty) / (2 * TEXTURE_SIZE));
			texture[ty][tx] = ((int)(r * factor) << 16)
							| ((int)(g * factor) << 8)
							| ((int)(b * factor));
			tx++;
		}
		ty++;
	}
}

void	draw_vertical_line(t_data *data, int x, int wall_height, int wall_color)
{
	int		texture[TEXTURE_SIZE][TEXTURE_SIZE];
	int		y;
	int		wall_top;
	int		wall_bottom;
	int		texture_y;
	double	step;
	double	texture_position;
	int		texture_start;

	generate_texture(texture, wall_color);
	wall_top = (HEIGHT - wall_height) / 2;
	wall_bottom = wall_top + wall_height - 1;
	if (wall_top < 0)
	{
		texture_start = -wall_top;
		wall_top = 0;
	}
	else
		texture_start = 0;
	if (wall_bottom >= HEIGHT)
		wall_bottom = HEIGHT - 1;
	step = (double)TEXTURE_SIZE / wall_height;
	texture_position = texture_start * step;
	y = 0;
	while (y < wall_top)
		my_mlx_pixel_put(data, x, y++, CEIL_COLOR);
	while (y <= wall_bottom)
	{
		texture_y = (int)texture_position % TEXTURE_SIZE;
		my_mlx_pixel_put(data, x, y++, texture[texture_y]
			[(int)(data->texture_x[x] * TEXTURE_SIZE) % TEXTURE_SIZE]);
		texture_position += step;
	}
	while (y < HEIGHT)
		my_mlx_pixel_put(data, x, y++, FLOOR_COLOR);
}

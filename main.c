/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 10:43:22 by icseri            #+#    #+#             */
/*   Updated: 2024/12/06 13:47:32 by icseri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

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

void	load_texture(t_data *data, t_texture *texture, char *file)
{
	texture->img = mlx_xpm_file_to_image(data->mlx, file,
			&texture->width, &texture->height);
	if (!texture->img)
	{
		ft_putstr_fd("Error: Failed to load texture\n", 2);
		exit(EXIT_FAILURE);
	}
	texture->data = (int *)mlx_get_data_addr(texture->img,
			&data->bpp, &data->line_len, &data->endian);
	if (!texture->data)
	{
		ft_putstr_fd("Error: Failed to retrieve texture data\n", 2);
		exit(EXIT_FAILURE);
	}
}

void	load_all_textures(t_data *data)
{
	load_texture(data, &data->textures[NORTH], data->map.north);
	load_texture(data, &data->textures[EAST], data->map.east);
	load_texture(data, &data->textures[SOUTH], data->map.south);
	load_texture(data, &data->textures[WEST], data->map.west);
}

void	render_scene(t_data *data)
{
	int	x;
	int	wall_height;

	wall_height = 0;
	x = 0;
	while (x < WIDTH)
	{
		if (data->ray_distance[x] > 0)
		{
			wall_height = (TILE_SIZE / data->ray_distance[x]) * (HEIGHT / 55);
			draw_vertical_line(data, x, wall_height);
		}
		x++;
	}
}

void	init_data(t_data *data)
{

	int	i;

	i = 0;
	while (i < WIDTH)
	{
		data->ray_dir[i] = -1;
		i++;
	}
	data->textures[0] = (t_texture){0};
	data->textures[1] = (t_texture){0};
	data->textures[2] = (t_texture){0};
	data->textures[3] = (t_texture){0};
	data->map = (t_map){0};
	data->map.fd = -1;
	data->map.player[0] = -1;
	data->map.player[1] = -1;
	data->map.enemy[0] = -1;
	data->map.enemy[1] = -1;
	data->map.ceiling = -1;
	data->map.floor = -1;
}
int track_mouse(void *param)
{
	t_data	*data;
	int		x;
	int		y;
	int		center_x;
	int		center_y;

	data = (t_data *)param;
	center_x = WIDTH / 2;
	center_y = HEIGHT / 2;
	mlx_mouse_get_pos(data->mlx, data->win, &x, &y);
	if (x != center_x)
		data->player_angle += (x - center_x) * MOUSE_SENSITIVITY;
	data->player_angle = fmod(data->player_angle + 2 * M_PI, 2 * M_PI);
	mlx_mouse_move(data->mlx, data->win, center_x, center_y);
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
	cast_rays(data);
	render_scene(data);
	draw_minimap(data);
	mlx_put_image_to_window(data->mlx, data->win, data->img, 0, 0);
	return (0);
}

int	main(int argc, char **argv)
{
	t_data	data;

	init_data(&data);
	parsing(argc, argv, &data.map);
	init_mlx(&data);
	mlx_mouse_move(data.mlx, data.win, WIDTH / 2, HEIGHT / 2);
	mlx_mouse_hide(data.mlx, data.win);
	set_player(&data);
	cast_rays(&data);
	load_all_textures(&data);
	render_scene(&data);
	draw_minimap(&data);
	//key_hook(KEY_W, &data); // really sketchy solution
	//key_hook(KEY_S, &data); // really sketchy solution
	mlx_put_image_to_window(data.mlx, data.win, data.img, 0, 0);
	mlx_hook(data.win, 2, 1L << 0, key_hook, &data);
	mlx_hook(data.win, 17, 0, close_window, &data);
	mlx_loop_hook(data.mlx, track_mouse, &data);
	mlx_loop(data.mlx);
	safe_exit(&data.map, EXIT_SUCCESS);
}

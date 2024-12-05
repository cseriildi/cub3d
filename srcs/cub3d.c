#include "../includes/cub3d.h"

void	render_scene(t_data *data)
{
	int	x;
	int	wall_height;

	x = 0;
	while (x < WIDTH)
	{
		if (data->ray_distance[x] > 0)
		{
			wall_height = (TILE_SIZE / data->ray_distance[x]) * (HEIGHT / 55);
			draw_vertical_line(data, x, wall_height, data->ray_color[x]);
		}
		x++;
	}
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
	exit(0);
}

void	initialize_map(t_data *data)
{
	int		i;
	int		j;
	char	temp_map[MAP_HEIGHT][MAP_WIDTH] = {
		"11111111",
		"10000001",
		"111E0101",
		"10000101",
		"11111111"
	};

	i = 0;
	while (i < 5)
	{
		j = 0;
		while (j < 8)
		{
			data->map[i][j] = temp_map[i][j];
			if (temp_map[i][j] == 'N' || temp_map[i][j] == 'S'
				|| temp_map[i][j] == 'E' || temp_map[i][j] == 'W')
			{
				data->player_x = j;
				data->player_y = i;
				if (temp_map[i][j] == 'N')
					data->player_angle = -M_PI / 2;
				else if (temp_map[i][j] == 'S')
					data->player_angle = M_PI / 2;
				else if (temp_map[i][j] == 'E')
					data->player_angle = 0;
				else if (temp_map[i][j] == 'W')
					data->player_angle = M_PI;
			}
			j++;
		}
		i++;
	}
	data->map_width = MAP_WIDTH;
	data->map_height = MAP_HEIGHT;
}

int	main(int argc, char **argv)
{
	t_data	data;

	//parse(argc, argv);
	(void)argc;
	(void)argv;
	data = (t_data){0};
	data.mlx = mlx_init();
	if (!data.mlx)
	{
		printf("Error: MLX initialization failed.\n");
		return (1);
	}
	data.win = mlx_new_window(data.mlx, WIDTH, HEIGHT, "cub3d");
	if (!data.win)
	{
		mlx_destroy_display(data.mlx);
		free(data.mlx);
		return (1);
	}
	data.img = mlx_new_image(data.mlx, WIDTH, HEIGHT);
	if (!data.img)
	{
		mlx_destroy_window(data.mlx, data.win);
		mlx_destroy_display(data.mlx);
		free(data.mlx);
		return (1);
	}
	data.addr = mlx_get_data_addr(data.img,
			&data.bpp, &data.line_len, &data.endian);
	initialize_map(&data);
	cast_rays(&data);
	render_scene(&data);
	draw_minimap(&data);
	mlx_put_image_to_window(data.mlx, data.win, data.img, 0, 0);
	mlx_hook(data.win, 2, 1L << 0, key_hook, &data);
	mlx_hook(data.win, 17, 0, close_window, &data);
	mlx_loop(data.mlx);
	return (0);
}

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

int	main(void)
{
	t_data	data;

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

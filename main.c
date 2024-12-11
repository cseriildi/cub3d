/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcsicsak <dcsicsak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 10:43:22 by icseri            #+#    #+#             */
/*   Updated: 2024/12/11 12:23:13 by dcsicsak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	main(int argc, char **argv)
{
	t_data	data;

	init_data(&data);
	data.map.is_bonus = false;
	parsing(argc, argv, &data.map);
	allocate_textures(&data);
	init_mlx(&data);
	mlx_mouse_move(data.mlx, data.win, WIDTH / 2, HEIGHT / 2);
	mlx_mouse_hide(data.mlx, data.win);
	set_player(&data);
	cast_rays(&data);
	load_all_textures(&data);
	render_scene(&data);
	mlx_put_image_to_window(data.mlx, data.win, data.img, 0, 0);
	mlx_hook(data.win, 2, 1L << 0, key_hook, &data);
	mlx_hook(data.win, 17, 0, close_window, &data);
	mlx_loop_hook(data.mlx, track_mouse, &data);
	mlx_loop(data.mlx);
	safe_exit(&data.map, EXIT_SUCCESS);
}

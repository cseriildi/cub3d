/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcsicsak <dcsicsak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 10:43:35 by icseri            #+#    #+#             */
/*   Updated: 2024/12/06 09:16:01 by dcsicsak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include "libft/libft.h"
# include <stdbool.h>
# include <errno.h>
# include <stdio.h>
# include <fcntl.h>
# include <stddef.h>
#include <mlx.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>

# define WIDTH 800
# define HEIGHT 600
#define TILE_SIZE 64
#define MOVE_SPEED 0.2
#define TURN_SPEED 0.1
#define FIELD_OF_VIEW M_PI / 3

#define PLAYER_COLOR 0xFF0000 // Red
#define GRID_COLOR 0xFF0000 // Red

# define KEY_ESC 65307
# define KEY_W 119
# define KEY_A 97
# define KEY_S 115
# define KEY_D 100
# define ARROW_LEFT 65361
# define ARROW_RIGHT 65363

typedef enum e_dir
{
	NORTH,
	EAST,
	SOUTH,
	WEST
}	t_dir;

typedef struct s_map
{
	char	*north; //mlx_xpm_file_to_image
	int		n_size[2];
	char	*south;
	int		s_size[2];
	char	*west;
	int		w_size[2];
	char	*east;
	int		e_size[2];
	int		floor;
	int		ceiling;
	char	**map;
	int		row;
	int		column;
	int		player[2];
	int		enemy[2];
	int		fd;
}	t_map;

typedef struct s_texture {
	void	*img;
	int		*data;
	int		width;
	int		height;
}	t_texture;


typedef struct s_data
{
	void			*mlx;
	void			*win;
	void			*img;
	char			*addr;
	int				bpp;
	int				line_len;
	int				endian;
	struct s_map	map;
	double			player_x;
	double			player_y;
	double			player_angle;
	double			ray_distance[WIDTH];
	double			texture_x[WIDTH];
	int				ray_dir[WIDTH];
	void			*texture_img;
	int				*texture_data;
	t_texture		textures[4];
}	t_data;

typedef struct s_line
{
	int	start_x;
	int	start_y;
	int	end_x;
	int	end_y;
	int	color;
}	t_line;

typedef struct s_line_params
{
	int	delta_x;
	int	delta_y;
	int	step_x;
	int	step_y;
	int	error;
	int	error2;
}	t_line_params;

typedef struct s_ray
{
	double	angle;
	double	closest_distance;
	double	hit_x;
	double	hit_y;
	int		hit_color;
	int		index;
}	t_ray;

typedef struct s_ray_trace_state
{
	int		direction;
	double	y_intercept;
	double	x_intercept;
	double	y_step;
	double	x_step;
	double	cur_x;
	double	cur_y;
	int		map_x;
	int		map_y;
	double	dist;
}	t_ray_trace_state;

typedef struct s_rectangle
{
	int	x;
	int	y;
	int	width;
	int	height;
	int	color;
}	t_rectangle;

typedef struct s_minimap
{
	int	map_rows;
	int	map_cols;
	int	tile_size;
	int	player_x;
	int	player_y;
	int	dir_x;
	int	dir_y;
}	t_minimap;

//parsing
void	parsing(int argc, char **argv, t_map *map);
void	get_texture(char *line, char **texture, t_map *map);
void	get_color(char *line, int *color, t_map *map);
void	get_map(char *line, t_map *map);
void	check_map(t_map *map);
void	list_to_arr(t_list **map_list, t_map *map);

//cleanup
void	print_error(int count, ...);
void	safe_exit(t_map *map, int exit_code);
void	free_array(char ***arr);
void	free_list(t_list **list);

#endif

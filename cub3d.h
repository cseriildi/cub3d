/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: icseri <icseri@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 10:43:35 by icseri            #+#    #+#             */
/*   Updated: 2024/11/20 17:02:46 by icseri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include "libft/libft.h"
# include <stdbool.h>
# include <errno.h>
# include <stdio.h>
# include <fcntl.h>

typedef struct s_color
{
	int	red;
	int	green;
	int	blue;
}	t_color;

typedef struct s_map
{
	char	*north;
	char	*south;
	char	*west;
	char	*east;
	t_color	floor;
	t_color	ceiling;
	char	**map;
}	t_map;

typedef struct s_data
{
	t_map	map;
	int		fd;
}	t_data;

//parsing
void	parsing(int argc, char **argv, t_data *data);
void	get_texture(char *line, t_data *data);

//cleanup
void	print_error(int count, ...);
void	safe_exit(t_data *data, int exit_code);

#endif
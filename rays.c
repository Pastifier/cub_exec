/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rays.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: diahmed <diahmed@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 16:59:41 by diahmed           #+#    #+#             */
/*   Updated: 2024/12/23 18:46:44 by diahmed          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "cub3d.h"

double	count_distance(t_point point, t_point player)
{
	double	distance;

	distance = sqrt(pow(player.x - point.x, 2) + pow(player.y - point.y, 2));
	return (distance);
}
double	round_num(double num)
{
	num = num / TILE_SIZE;
	num = floor (num);
	// printf("num = %f \n\n", num);
	return (num);
}

t_point horizontal_coord(t_map *map)
{
    t_point intersect; // Point of intersection
    t_point step;      // Steps to move to the next horizontal grid line
	t_point p;

    reset_angle(&map->ray_angle);
	p.x = map->player.x + TILE_SIZE / 2;
	p.y = map->player.y + TILE_SIZE / 2;
    if (map->ray_angle > 0 && map->ray_angle < 180)
        intersect.y = floor(map->player.y / TILE_SIZE) * TILE_SIZE - 1; // Facing up
    else
        intersect.y = floor(map->player.y / TILE_SIZE) * TILE_SIZE + TILE_SIZE; // Facing down
    intersect.x = p.x + (p.y - intersect.y) / tan(deg_to_rad(map->ray_angle));
	step.y = TILE_SIZE;
	if (map->ray_angle > 0 && map->ray_angle < 180)
		step.y = -TILE_SIZE;
    step.x = -step.y / tan(deg_to_rad(map->ray_angle));
    while ((intersect.x >= 0 && intersect.x < map->map_width * TILE_SIZE) &&
           (intersect.y >= 0 && intersect.y < map->map_height * TILE_SIZE))
    {
        // Check if the grid cell at the intersection contains a wall
        int map_x = (int)(intersect.x / TILE_SIZE);
        int map_y = (int)(intersect.y / TILE_SIZE);

        if (map->map[map_y][map_x] == '1')
            return intersect; // Return the intersection point if a wall is found
        intersect.x += step.x;
        intersect.y += step.y;
    }
    return (t_point){NO_HIT, NO_HIT};
}

t_point vertical_coord(t_map *map)
{
    t_point intersect;
    t_point step;
	t_point p;

    reset_angle(&map->ray_angle);
	p.x = map->player.x + TILE_SIZE / 2;
	p.y = map->player.y + TILE_SIZE / 2;
    if (map->ray_angle > 90 && map->ray_angle < 270)
        intersect.x = floor(map->player.x / TILE_SIZE) * TILE_SIZE - 1;
    else
        intersect.x = floor(map->player.x / TILE_SIZE) * TILE_SIZE + TILE_SIZE;
    intersect.y = p.y + (p.x - intersect.x) * tan(deg_to_rad(map->ray_angle));
	step.x = TILE_SIZE;
	if (map->ray_angle > 90 && map->ray_angle < 270)
        step.x = -TILE_SIZE;              
    step.y = step.x * tan(deg_to_rad(map->ray_angle));
    while ((intersect.x >= 0 && intersect.x < map->map_width * TILE_SIZE) &&
           (intersect.y >= 0 && intersect.y < map->map_height * TILE_SIZE))
    {
        if (map->map[(int)(intersect.y / TILE_SIZE)][(int)(intersect.x / TILE_SIZE)] == '1')
            return intersect;

        intersect.x += step.x;
        intersect.y += step.y;
    }

    return (t_point){NO_HIT, NO_HIT};
}

int	is_wall(double pix_x, double pix_y, t_cub *cub)
{
	double	x;
	double	y;

	x = round_num(pix_x);
	y = round_num(pix_y);
	if (cub->map.map[(int)y][(int)x] == '1')
		return (1);
	return (0);	
}

t_point	intersect_point(t_map *map)
{
	t_point	horizontal_hit;
	t_point	vertical_hit;
	double	h_distance;
	double	v_distance;

	h_distance = -1;
	v_distance = -1;
	reset_angle(&map->ray_angle);
	horizontal_hit = horizontal_coord(map);
	vertical_hit = vertical_coord(map);
	// printf("in ray h_x = %f\th_y = %f\n\n", horizontal_hit.x, horizontal_hit.y);
	// printf("in ray v_x = %f\tv_y = %f\n\n", vertical_hit.x, vertical_hit.y);
	if (horizontal_hit.x != NO_HIT && horizontal_hit.y != NO_HIT)
		h_distance = count_distance(horizontal_hit, map->player);
	if (vertical_hit.x != NO_HIT && vertical_hit.y != NO_HIT)
		v_distance = count_distance(vertical_hit, map->player);
	// printf("h_d = %f\n\n", h_distance);
	// printf("v_d = %f\n\n", v_distance);
	if (h_distance < v_distance && v_distance != -1 && h_distance != -1)
		return (horizontal_hit);
	else if (v_distance == -1)
		return (horizontal_hit);
	return (vertical_hit);
}
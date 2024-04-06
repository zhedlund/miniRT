#include "../../minirt.h"

// cc rt1wkd_mlx.c -Lminilibx-linux -lmlx_Linux -lX11 -lXext -lm

typedef struct
{
	double x;
	double y;
	double z;
} vec3;

// aliases for vec3, used for clarity in the code
typedef vec3 point3;
typedef vec3 color;

typedef struct
{
    vec3 orig; // Origin point of the ray
    vec3 dir;  // Direction of the ray
} ray;

typedef struct
{
    vec3 center;
    double radius;
} sphere;

typedef struct
{
    double y; // y-coordinate of the ground plane
} plane;

double dot(const vec3 *u, const vec3 *v) 
{
    return (u->x * v->x + u->y * v->y + u->z * v->z);
}

double vec3_length_squared(const vec3 *v)
{
    return (v->x * v->x + v->y * v->y + v->z * v->z);
}

void write_color(color pixel_color, t_img *img, int x, int y) 
{
    int color_value = ((int)(255.999 * pixel_color.x) << 16) +
                      ((int)(255.999 * pixel_color.y) << 8) +
                      (int)(255.999 * pixel_color.z);
    ft_pixel_put(img, x, y, color_value);
}

double hit_plane(const plane *ground, const ray *r) {
    if (r->dir.y == 0) {
        return -1.0; // Ray is parallel to the ground plane
    }
    // Calculate the intersection point using the parametric equation of a ray
    double t = (ground->y - r->orig.y) / r->dir.y;
    if (t < 0) {
        return -1.0; // Intersection point is behind the ray origin
    }
    return (t);
}

double hit_sphere(const point3 *center, double radius, const ray *r)
{
    vec3 oc = {r->orig.x - center->x, r->orig.y - center->y, r->orig.z - center->z };
    double a = vec3_length_squared(&r->dir);
    double half_b = dot(&oc, &r->dir);
    double c = vec3_length_squared(&oc) - radius * radius;
    double discriminant = half_b * half_b - a * c;

    if (discriminant < 0)
        return -1.0;
    else
        return ((-half_b - sqrt(discriminant)) / a);
}

/*color ray_color(const ray *r)
{
    point3 center = { 0, 0, -1 };
    double radius = 0.5;
    double t = hit_sphere(&center, radius, r);

    if (t > 0.0) // ray intersects with the object
	{
        vec3 n = { r->orig.x + t * r->dir.x, r->orig.y + t * r->dir.y, r->orig.z + t * r->dir.z }; // calculates intersection point in 3D space using the parametric equation of a ray
        vec3 temp = { 0, 0, -1 }; // holds the normal direction of the surface at the intersection point, pointing in the negative z-direction
        vec3 unit_n = { (n.x - temp.x) / radius, (n.y - temp.y) / radius, (n.z - temp.z) / radius }; // normalizes the normal vector
        color pixel_color = { 0.5 * (unit_n.x + 1), 0.5 * (unit_n.y + 1), 0.5 * (unit_n.z + 1) }; // maps the normal vector to a color value
        return (pixel_color); // represents the shading of the surface at the intersection point
    }
    vec3 unit_direction = { r->dir.x / sqrt(dot(&r->dir, &r->dir)),
                             r->dir.y / sqrt(dot(&r->dir, &r->dir)),
                             r->dir.z / sqrt(dot(&r->dir, &r->dir)) };
    double a = 0.5 * (unit_direction.y + 1.0);

	// Light color scene
    color light = { 1.0, 1.0, 1.0 };
    color dark = { 0.5, 0.7, 1.0 };
	// ambient light contribution
    color ambient = { (1.0 - a) * light.x, (1.0 - a) * light.y, (1.0 - a) * light.z };
	// diffuse reflection contribution
    color diffusion = { a * dark.x, a * dark.y, a * dark.z };
	// sum of ambient and diffuse light to get final pixel color
    color pixel_color = { ambient.x + diffusion.x, ambient.y + diffusion.y, ambient.z + diffusion.z };
    return (pixel_color);
}*/

color ray_color(const ray *r)
{
    point3 center = { 0, 0, -1 };
    double radius = 0.5;
    double t_sphere = hit_sphere(&center, radius, r);
    plane ground = {-.4}; // y-coordinate
    double t_plane = hit_plane(&ground, r);

    if (t_sphere > 0.0 && (t_plane < 0 || t_sphere < t_plane))
	{
        // Intersection with the sphere
        vec3 n = { r->orig.x + t_sphere * r->dir.x, r->orig.y + t_sphere * r->dir.y, r->orig.z + t_sphere * r->dir.z };
        vec3 temp = { 0, 0, -1 };
        vec3 unit_n = { (n.x - temp.x) / radius, (n.y - temp.y) / radius, (n.z - temp.z) / radius };
        color pixel_color = { 0.5 * (unit_n.x + 1), 0.5 * (unit_n.y + 1), 0.5 * (unit_n.z + 1) };
        return (pixel_color);
    }
	else if (t_plane> 0) 
	/*{
        // Intersection with the ground plane
        color ground_color = {0.3, 0.5, 0.5};
        return (ground_color);
    }*/
	{   // psychedelic ground plane
		vec3 n = { r->orig.x + t_plane * r->dir.x, r->orig.y + t_plane * r->dir.y, r->orig.z + t_plane * r->dir.z };
		vec3 temp = { 0, 0, -1 };
		vec3 unit_n = { (n.x - temp.x) / radius, (n.y - temp.y) / radius, (n.z - temp.z) / radius };
		color pixel_color = { 0.5 * (unit_n.x + 1), 0.5 * (unit_n.y + 1), 0.5 * (unit_n.z + 1) };
		return (pixel_color);
	}
	else 
	{
        // No intersection with either the sphere or the ground
        vec3 unit_direction = { r->dir.x / sqrt(dot(&r->dir, &r->dir)),
                             r->dir.y / sqrt(dot(&r->dir, &r->dir)),
                             r->dir.z / sqrt(dot(&r->dir, &r->dir)) };
    double a = 0.5 * (unit_direction.y + 1.0);

	// Light color scene (rn it's just a gradient in the background)
    color light = { 1.0, 1.0, 1.0 };
    color dark = { 0.5, 0.7, 1.0 };
	// ambient light contribution
    color ambient = { (1.0 - a) * light.x, (1.0 - a) * light.y, (1.0 - a) * light.z };
	// diffuse reflection contribution
    color diffusion = { a * dark.x, a * dark.y, a * dark.z };
	// sum of ambient and diffuse light to get final pixel color
    color pixel_color = { ambient.x + diffusion.x, ambient.y + diffusion.y, ambient.z + diffusion.z };
    return (pixel_color);
    }
}


/*
	RGB normalized color ranges 0.0-1.0:
    Pure Red: (1.0, 0.0, 0.0)
    Pure Green: (0.0, 1.0, 0.0)
    Pure Blue: (0.0, 0.0, 1.0)
    White: (1.0, 1.0, 1.0)
    Black: (0.0, 0.0, 0.0)
    Yellow: (1.0, 1.0, 0.0)
    Cyan: (0.0, 1.0, 1.0)
    Magenta: (1.0, 0.0, 1.0)
    Orange: (1.0, 0.5, 0.0)
    Light Gray: (0.8, 0.8, 0.8)
	Light Blue: (0.5, 0.7, 1.0)

*/

void ft_pixel_put(t_img *img, int x, int y, int color)
{
    char *pixel;

    pixel = img->addr + (y * img->line_len + x * (img->bpp / 8));
    *(unsigned int *)pixel = color;
}

int	close_window(t_data *data)
{
	//mlx_destroy_window(data->mlx_ptr, data->win_ptr);
	//mlx_destroy_image(data->mlx_ptr, data->img.mlx_img);
	//mlx_destroy_display(data->mlx_ptr);
	//free(data->mlx_ptr);
	exit(0);
}

int key_handler(int keycode, t_data *data)
{
	if (keycode == XK_Escape)
		close_window(data);
	return (0);
}

int main()
{
	t_data data;
    // Image
    double aspect_ratio = 16.0 / 9.0;
    int image_width = 400;
    int image_height = (int)(image_width / aspect_ratio); // Calculate image height
	if (image_height < 1)
    	image_height = 1; // ensure height is at least 1

	/* initialize mlx stuff*/
    data.mlx_ptr = mlx_init();
	if (data.mlx_ptr == NULL)
		return (1);
	data.win_ptr = mlx_new_window(data.mlx_ptr, image_width, image_height, "MiniRT");
	if (data.win_ptr == NULL)
	{
		free(data.win_ptr);
		return (1);
	}
	/* set up hooks*/
	data.img.mlx_img = mlx_new_image(data.mlx_ptr, image_width, image_height);
	data.img.addr = mlx_get_data_addr(data.img.mlx_img, &data.img.bpp, &data.img.line_len, &data.img.endian);

    // Camera
    double focal_length = 1.0;
    double viewport_height = 2.0;
    double viewport_width = viewport_height * ((double)image_width / image_height);
    vec3 camera_center = {0, 0, 0};

    // Calculate the vectors across the horizontal and down the vertical viewport edges.
    vec3 viewport_u = {viewport_width, 0, 0};
    vec3 viewport_v = {0, -viewport_height, 0};

    // Calculate the horizontal and vertical delta vectors from pixel to pixel.
    vec3 pixel_delta_u = {viewport_u.x / image_width, 0, 0};
    vec3 pixel_delta_v = {0, viewport_v.y / image_height, 0};

    // Calculate the location of the upper left pixel.
    vec3 viewport_up_left = {camera_center.x - 0.5 * viewport_u.x - 0.5 * viewport_v.x,
                                camera_center.y - 0.5 * viewport_u.y - 0.5 * viewport_v.y,
                                camera_center.z - focal_length};
    vec3 pixel00_loc = {viewport_up_left.x + 0.5 * (pixel_delta_u.x + pixel_delta_v.x),
                        viewport_up_left.y + 0.5 * (pixel_delta_u.y + pixel_delta_v.y),
                        viewport_up_left.z};

	// Render
    int j = 0;
	while (j < image_height)
	{
    	int i = 0;
    	while (i < image_width)
		{
        	vec3 pixel_center = {pixel00_loc.x + i * pixel_delta_u.x, pixel00_loc.y + j * pixel_delta_v.y, pixel00_loc.z};
        	vec3 ray_direction = {pixel_center.x - camera_center.x, pixel_center.y - camera_center.y, pixel_center.z - camera_center.z};
        	ray r = {camera_center, ray_direction};

        	color pixel_color = ray_color(&r);
        	write_color(pixel_color, &data.img, i, j);
        	i++;
    	}
    	j++;
	}
	mlx_put_image_to_window(data.mlx_ptr, data.win_ptr, data.img.mlx_img, 0, 0);
	//mlx_loop_hook(data.mlx_ptr, &render, &data);
	mlx_hook(data.win_ptr, KeyPress, KeyPressMask, &key_handler, &data); 
	mlx_hook(data.win_ptr, 17, 0, &close_window, NULL); // close window by clicking x
	mlx_loop(data.mlx_ptr);
	mlx_destroy_image(data.mlx_ptr, data.img.mlx_img);
	mlx_destroy_display(data.mlx_ptr);
	free(data.mlx_ptr);
    return (0);
}

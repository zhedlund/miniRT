#include "minirt.h"

// cc rt_mlx_nv.c -Lminilibx-linux -lmlx_Linux -lX11 -lXext -lm

typedef struct s_vec
{
	double x;
	double y;
	double z;
} t_vec;

// alias for t_vec, used for clarity in the code
typedef t_vec point3;
typedef struct s_color
{
    double r;
    double g;
    double b;
} t_color;

typedef struct s_ray
{
    t_vec origin; // Origin point of the ray
    t_vec dir;  // Direction of the ray
} t_ray;

typedef struct s_cam
{
    double focal_length;
    double fov;
    double viewport_width;
    double viewport_height;
    t_vec center; // camera position
	t_vec orientation; // normalized orientation vector (direction the camera is pointing)
    t_vec viewport_u;
    t_vec viewport_v;
    t_vec px_delta_u;
    t_vec px_delta_v;
    t_vec viewport_up_left;
    t_vec px_00;
} t_cam;

typedef struct s_amb
{
    double ratio; // Ambient lighting ratio in the range [0.0, 1.0]
    color color;
    double diffuse; // Diffuse intensity of ambient light
} t_amb;

typedef struct s_light
{
    t_vec dir;
    double ratio;
    double diffuse; // Diffuse intensity of the light source
    t_color color;
} t_light;

typedef struct
{
    ambient ambient;
    light light;
} lighting;

typedef struct
{
    point3 center;
    double radius;
	color color;
	double t1; // first intersection point along the ray
	double t2; // second intersection point
	double discriminant; // discriminant of the quadratic equation
} sphere;

typedef struct
{
    point3 point; // A point on the plane
    t_vec normal;  // The normal vector to the plane
	color color;
} plane;


void ft_pixel_put(t_img *img, int x, int y, int color)
{
    char *pixel;

    pixel = img->addr + (y * img->line_len + x * (img->bpp / 8));
    *(unsigned int *)pixel = color;
}

/*	calculates the dot product between two 3D vectors u and v.
	repr cosine of the angle between them multiplied by the magnitudes of the vectors. 
	u⋅v = ux⋅vx + uy⋅vy + uz⋅vz
*/
double dot(const t_vec *u, const t_vec *v) 
{
    return ((u->x * v->x) + (u->y * v->y) + (u->z * v->z));
}

/*	calculates the length of a vector represented by a t_vec struct,
	by summing the squares of the components of the vector and taking the square root.
*/
double vec3_length(const t_vec *v)
{
    return (sqrt((v->x * v->x) + (v->y * v->y) + (v->z * v->z)));
}

/*	calculates the squared length of a vector represented by a t_vec struct,
	by summing the squares of the components of the vector.
*/
double vec3_length_squared(const t_vec *v)
{
    return ((v->x * v->x) + (v->y * v->y) + (v->z * v->z));
}

/* 	Normalizes a vector represented by a t_vec struct.
	Returns a new t_vec struct representing the normalized vector.
*/
t_vec vec3_unit_vector(const t_vec *v)
{
    double length = sqrt((v->x * v->x) + (v->y * v->y) + (v->z * v->z));
    return ((t_vec){(v->x / length), (v->y / length), (v->z / length)});
}

t_vec vec3_subtract(const t_vec a, const t_vec b)
{
	return ((t_vec){(a.x - b.x), (a.y - b.y), (a.z - b.z)});
}

t_vec vec3_add(const t_vec a, const t_vec b)
{
	return ((t_vec){(a.x + b.x), (a.y + b.y), (a.z + b.z)});
}

/* 	Writes the color value of a pixel to the image buffer.
	Color values are represented as 24-bit RGB values.
*/
void write_color(color px, t_img *img, int x, int y) 
{
    int color_value = ((int)(255.999 * px.r) << 16) +
                      ((int)(255.999 * px.g) << 8) +
                      (int)(255.999 * px.b);
    ft_pixel_put(img, x, y, color_value);
}

/* 	Adds two color values together.
	Returns a new color value that is the sum of the two input color values.
*/
color blend_color(const color *c1, const color *c2)
{
	double r = c1->r + c2->r;
	double g = c1->g + c2->g;
	double b = c1->b + c2->b;

	// Clamp the color values to the range [0.0, 1.0]
	if (r > 1.0)
		r = 1.0;
	if (g > 1.0)
		g = 1.0;
	if (b > 1.0)
		b = 1.0;
    return ((color){r, g, b});
}

/* 	Calculates the point of intersection between a ray and a plane.
	Returns the distance from the ray origin to the intersection point,
	or -1 if the ray is parallel to the plane, or the intersection point is behind the ray origin.
*/
double hit_plane(const plane *pl, const ray *r)
{
    double denominator = dot(&pl->normal, &r->dir);
    if (denominator == 0)
        return (-1.0); // Ray is parallel to the plane
    t_vec origin_to_point = {
		(pl->point.x - r->origin.x),
		(pl->point.y - r->origin.y),
		(pl->point.z - r->origin.z)
		};
    double t = dot(&origin_to_point, &pl->normal) / denominator;
    if (t < 0)
        return (-1.0); // Intersection point is behind the ray origin
    return (t);
}

/* 	Calculates the point of intersection between a ray and a sphere.
	Stores the intersection points in the sphere struct.
*/
double hit_sphere(const point3 *center, double radius, const ray *r)
{
    t_vec oc = vec3_subtract(r->origin, *center);
	double a = vec3_length_squared(&r->dir); // 
    double half_b = dot(&oc, &r->dir); // half dot product of vector oc and the direction vector of the ray.
    double c = vec3_length_squared(&oc) - radius * radius;
    double discriminant = half_b * half_b - a * c;

    if (discriminant < 0)
        return (-1.0);
    else
        return ((-half_b - sqrt(discriminant)) / a);
}

t_vec sphere_normal(const sphere *sp, const point3 *intersect)
{
    return ((t_vec){(intersect->x - sp->center.x) / sp->radius,
                     (intersect->y - sp->center.y) / sp->radius,
                     (intersect->z - sp->center.z) / sp->radius});
}

point3 intersect_point(const ray *r, double t)
{
    return ((point3){r->origin.x + (t * r->dir.x),
                   r->origin.y + (t * r->dir.y),
                   r->origin.z + (t * r->dir.z)});
}

color ambient_color(const ambient *a, const color *c)
{
    return ((color){a->ratio * c->r, a->ratio * c->g, a->ratio * c->b});
}

color diffuse_color(const light *l, const color *c, double diffuse_factor)
{
    return ((color){fmax(0, diffuse_factor) * c->r * l->color.r * l->ratio,
                     fmax(0, diffuse_factor) * c->g * l->color.g * l->ratio,
                     fmax(0, diffuse_factor) * c->b * l->color.b * l->ratio});
}
//fmax returns the greater of two values

/* 	Calculates the color of a pixel based on the intersection of a ray with a sphere or plane.
	Returns a color value based on the normal vector at the intersection point.
*/
color ray_color(const ray *r, const sphere *sp, const plane *pl, const lighting *lights)
{
    double ts = hit_sphere(&sp->center, sp->radius, r);
    double tp = hit_plane(pl, r);
    color px;

	if (ts > 0.0 && (tp < 0 || ts < tp)) // Intersection with the sphere
	{ 
        // Calculate intersection point and normal vector
		point3 intersect = intersect_point(r, ts);
        t_vec normal = sphere_normal(sp, &intersect);
		color ambient_light = ambient_color(&lights->ambient, &sp->color);

        // Diffuse lighting for shadow effect
        t_vec light_dir = vec3_unit_vector(&lights->light.dir);
        double diffuse_factor = dot(&light_dir, &normal);
		color diffuse = diffuse_color(&lights->light, &sp->color, diffuse_factor);
        px = blend_color(&ambient_light, &diffuse); // add ambient and diffuse components
    } 
    else if (tp > 0) // Intersection with the plane
    {
        point3 intersect_plane = intersect_point(r, tp);
        // Check if the sphere is blocking the light
        t_vec temp = vec3_subtract(lights->light.dir, intersect_plane);
		t_vec to_light = vec3_unit_vector(&temp); // Direction from plane to light source

        ray shadow_ray = {intersect_plane, to_light};
        double t_shadow_sphere = hit_sphere(&sp->center, sp->radius, &shadow_ray);
        if (t_shadow_sphere > 0 && t_shadow_sphere < 1.0) // Sphere is blocking the light
		{
            px = (color){0.0, 0.3, 0.2}; // shadow color
		}
		else // No shadow
        {
            color ambient_light = ambient_color(&lights->ambient, &pl->color); // Ambient lighting for plane
            px = ambient_light; // Plane pixel color with ambient component
        }
    }
    else // No intersection with the plane
    {
        px = (color){0.5, 0.7, 1.0}; // Background color
    }
    return (px);
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
	t_data	data;
	camera	cam;
	sphere	sp;
	plane	pl;
	ambient a;
	light	l;

	// Image
    double aspect_ratio = 16.0 / 9.0;
    int image_width = 400;
    int image_height = (int)(image_width / aspect_ratio); // Calculate image height
	if (image_height < 1)
    	image_height = 1; // ensure height is at least 1

	// initialize mlx stuff
    data.mlx_ptr = mlx_init();
	if (data.mlx_ptr == NULL)
		return (1);
	data.win_ptr = mlx_new_window(data.mlx_ptr, image_width, image_height, "MiniRT");
	if (data.win_ptr == NULL)
	{
		free(data.win_ptr);
		return (1);
	}
	data.img.mlx_img = mlx_new_image(data.mlx_ptr, image_width, image_height);
	data.img.addr = mlx_get_data_addr(data.img.mlx_img, &data.img.bpp, &data.img.line_len, &data.img.endian);

	// init sphere & plane structs 
    sp.center = (point3){0.0, 0.0, -1}; // center coordinates
    sp.radius = 0.5; 
	sp.color = (color){0.7, 0.1, 0.7}; // color of the sphere

    pl.point = (point3){0, -0.4, 0}; // point on the plane
    pl.normal = (t_vec){0, 1, 0}; // assigning normal vector
	pl.color = (color){0.1, 0.9, 0.6}; // color of the plane

	// init light structs
    a.ratio = 1; // ambient light ratio, higher = brighter
    a.color = (color){1.0, 1.0, 1.0}; 
    a.diffuse = 0.0; // higher value = more light is scattered, brighter shading on the surface

    l.dir = (t_vec){-40.0, 50.0, 0.0}; 
    l.ratio = 1; // direct light ratio, higher = brighter
    l.diffuse = -0.5;
    l.color = (color){1.0, 1.0, 0.0}; 

    // initialize camera struct
    cam.focal_length = 1.0;
    cam.fov = 70.0;
	cam.center = (t_vec){0, 0, 0.1}; // viewpoint coordinates. x = left-right, y = up-down, z = forward-backward
	cam.orientation = (t_vec){0.0, 0.0, 0.0}; // normalized orientation vector. cam orientation along xyz axis

   	double fov_radians = cam.fov * M_PI / 180.0; // Convert FOV to radians
   	cam.viewport_height = 2.0 * tan(fov_radians / 2.0); // viewport height based on FOV
    cam.viewport_width = cam.viewport_height * ((double)image_width / image_height);

    // the vectors across the horizontal & down the vertical viewport edges
    cam.viewport_u = (t_vec){cam.viewport_width, 0, 0};
    cam.viewport_v = (t_vec){0, -cam.viewport_height, 0};

    // the horizontal and vertical delta vectors from pixel to pixel
    cam.px_delta_u = (t_vec){cam.viewport_u.x / image_width, 0, 0};
    cam.px_delta_v = (t_vec){0, cam.viewport_v.y / image_height, 0};

    // location of the upper left pixel
    cam.viewport_up_left = (t_vec) {
    cam.center.x - 0.5 * cam.viewport_u.x - 0.5 * cam.viewport_v.x,
    cam.center.y - 0.5 * cam.viewport_u.y - 0.5 * cam.viewport_v.y,
    cam.center.z - cam.focal_length
	};
    // coordinates of the upper-left corner of the viewport
    cam.px_00 = (t_vec) {
    cam.viewport_up_left.x + 0.5 * (cam.px_delta_u.x + cam.px_delta_v.x),
    cam.viewport_up_left.y + 0.5 * (cam.px_delta_u.y + cam.px_delta_v.y),
    cam.viewport_up_left.z
	};

	int j = 0;
	while (j < image_height)
	{
    	int i = 0;
    	while (i < image_width)
	    {
    	    // calculates position of the current pixel
        	t_vec px_center = {
            	cam.px_00.x + (i * cam.px_delta_u.x),
            	cam.px_00.y + (j * cam.px_delta_v.y),
        		cam.px_00.z
        	};
		 	// calculates direction based on the pixel's position and cam center
        	t_vec ray_dir = {
            	px_center.x - cam.center.x,
            	px_center.y - cam.center.y,
            	px_center.z - cam.center.z
        	};
			// Rotate the ray direction according to the camera orientation
        	//ray_dir = vec3_unit_vector(&ray_dir); // normalize ray direction
        	//ray_dir.x += cam.orientation.x;
			//ray_dir.y += cam.orientation.y;
			//ray_dir.z += cam.orientation.z;

			//ray_dir = vec3_unit_vector(&ray_dir); // normalize ray direction again
			ray r = {cam.center, ray_dir}; // create the ray

        	// calculate pixel color and write to image buffer
        	color px_color = ray_color(&r, &sp, &pl, &(lighting){a, l});
        	write_color(px_color, &data.img, i, j);
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

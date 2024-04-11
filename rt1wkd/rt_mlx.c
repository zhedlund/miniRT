#include "../minirt.h"

// cc rt_mlx.c -Lminilibx-linux -lmlx_Linux -lX11 -lXext -lm

typedef struct
{
	double x;
	double y;
	double z;
} vec3;

// alias for vec3, used for clarity in the code
typedef vec3 point3;
typedef struct
{
    double r;
    double g;
    double b;
} color;

typedef struct
{
    vec3 origin; // Origin point of the ray
    vec3 dir;  // Direction of the ray
} ray;

typedef struct
{
    double focal_length;
    double fov;
    double viewport_width;
    double viewport_height;
    vec3 center; // camera position
	vec3 orientation; // normalized orientation vector (direction the camera is pointing)
    vec3 viewport_u;
    vec3 viewport_v;
    vec3 px_delta_u;
    vec3 px_delta_v;
    vec3 viewport_up_left;
    vec3 px_00;
} camera;

typedef struct
{
    double ratio; // Ambient lighting ratio in the range [0.0, 1.0]
    color color;
    double diffuse; // Diffuse intensity of ambient light
} ambient;

typedef struct
{
    vec3 dir;
    double ratio;
    double diffuse; // Diffuse intensity of the light source
    color color;
} light;

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
} sphere;

typedef struct
{
    point3 point; // A point on the plane
    vec3 normal;  // The normal vector to the plane
	color color;
} plane;

/*	calculates the dot product between two 3D vectors u and v.
	repr cosine of the angle between them multiplied by the magnitudes of the vectors. 
	u⋅v = ux⋅vx + uy⋅vy + uz⋅vz
*/
double dot(const vec3 *u, const vec3 *v) 
{
    return ((u->x * v->x) + (u->y * v->y) + (u->z * v->z));
}

/*	calculates the squared length of a vector represented by a vec3 struct,
	by summing the squares of the components of the vector.
*/
double vec3_length_squared(const vec3 *v)
{
    return ((v->x * v->x) + (v->y * v->y) + (v->z * v->z));
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

/* 	Calculates the point of intersection between a ray and a plane.
	Returns the distance from the ray origin to the intersection point,
	or -1 if the ray is parallel to the plane, or the intersection point is behind the ray origin.
*/
double hit_plane(const plane *pl, const ray *r)
{
    double denominator = dot(&pl->normal, &r->dir);
    if (denominator == 0)
        return -1.0; // Ray is parallel to the plane
    vec3 origin_to_point = {
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
	Returns the distance from the ray origin to the intersection point,
	or -1 if the ray does not intersect the sphere.
*/
double hit_sphere(const point3 *center, double radius, const ray *r)
{
    vec3 oc = {r->origin.x - center->x, r->origin.y - center->y, r->origin.z - center->z }; // vector from the origin of the ray to the center of the sphere
    double a = vec3_length_squared(&r->dir); // 
    double half_b = dot(&oc, &r->dir); // half dot product of vector oc and the direction vector of the ray.
    double c = vec3_length_squared(&oc) - radius * radius;
    double discriminant = half_b * half_b - a * c;

    if (discriminant < 0)
        return (-1.0);
    else
        return ((-half_b - sqrt(discriminant)) / a);
}

color add_color(const color *c1, const color *c2)
{
    return (color){c1->r + c2->r, c1->g + c2->g, c1->b + c2->b};
}

/* 	Normalizes a vector represented by a vec3 struct.
	Returns a new vec3 struct representing the normalized vector.
*/
vec3 vec3_unit_vector(const vec3 *v)
{
    double length = sqrt(v->x * v->x + v->y * v->y + v->z * v->z);
    return (vec3){v->x / length, v->y / length, v->z / length};
}

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
        point3 intersect = {r->origin.x + (ts * r->dir.x), r->origin.y + (ts * r->dir.y), r->origin.z + (ts * r->dir.z)};
        vec3 normal = {(intersect.x - sp->center.x) / sp->radius, (intersect.y - sp->center.y) / sp->radius, (intersect.z - sp->center.z) / sp->radius};
        
        // Ambient lighting
        color ambient_color = {lights->ambient.ratio * sp->color.r * lights->ambient.color.r,
                               lights->ambient.ratio * sp->color.g * lights->ambient.color.g,
                               lights->ambient.ratio * sp->color.b * lights->ambient.color.b};

        // Diffuse lighting for shadow effect
        vec3 light_dir = vec3_unit_vector(&lights->light.dir);
        double diffuse_factor = dot(&light_dir, &normal);
        color diffuse_color = {fmax(0, diffuse_factor) * sp->color.r * lights->light.color.r * lights->light.ratio,
                               fmax(0, diffuse_factor) * sp->color.g * lights->light.color.g * lights->light.ratio,
                               fmax(0, diffuse_factor) * sp->color.b * lights->light.color.b * lights->light.ratio};

        // add ambient and diffuse components
        px = add_color(&ambient_color, &diffuse_color);
    } 
	else if (tp > 0) // Intersection with the plane
	{
        // Calculate intersection point
        point3 intersect = {r->origin.x + (tp * r->dir.x), r->origin.y + (tp * r->dir.y), r->origin.z + (tp * r->dir.z)};
        
        // Ambient lighting for plane
        color ambient_color = {lights->ambient.ratio * pl->color.r * lights->ambient.color.r,
                               lights->ambient.ratio * pl->color.g * lights->ambient.color.g,
                               lights->ambient.ratio * pl->color.b * lights->ambient.color.b};

        // Final pixel color with ambient component
        px = ambient_color;
    } 
	else { // No intersection
        px = (color){0.5, 0.7, 1.0}; // Background color
    }
    return px;
}

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
	t_data	data;
	camera	cam;
	sphere	sp;
	plane	pl;
	ambient a;
	light	l;
	ray		r;

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
    pl.normal = (vec3){0, 1, 0}; // assigning normal vector
	pl.color = (color){0.1, 0.9, 0.6}; // color of the plane

	// init light structs
    a.ratio = 0.4; // ambient light ratio, higher = brighter
    a.color = (color){1.0, 1.0, 1.0}; 
    a.diffuse = 0.0; // higher value = more light is scattered, brighter shading on the surface

    l.dir = (vec3){-40.0, 50.0, 0.0}; 
    l.ratio = 0.6; // direct light ratio, higher = brighter
    l.diffuse = -0.5;
    l.color = (color){1.0, 1.0, 0.0}; 

	//r.origin = (vec3){0, 0, 0}; // ray origin
	//r.dir = (vec3){0, 0, 0}; // ray direction

    // initialize camera struct
    cam.focal_length = 1.0;
    cam.fov = 70.0;
	cam.center = (vec3){0.0, 0, 0.1}; // viewpoint coordinates. x = left-right, y = up-down, z = forward-backward
	cam.orientation = (vec3){0.0, 0.0, 1.0}; // normalized orientation vector. cam orientation along xyz axis

   	double fov_radians = cam.fov * M_PI / 180.0; // Convert FOV to radians
   	cam.viewport_height = 2.0 * tan(fov_radians / 2.0); // viewport height based on FOV
    cam.viewport_width = cam.viewport_height * ((double)image_width / image_height);

    // the vectors across the horizontal & down the vertical viewport edges
    cam.viewport_u = (vec3){cam.viewport_width, 0, 0};
    cam.viewport_v = (vec3){0, -cam.viewport_height, 0};

    // the horizontal and vertical delta vectors from pixel to pixel
    cam.px_delta_u = (vec3){cam.viewport_u.x / image_width, 0, 0};
    cam.px_delta_v = (vec3){0, cam.viewport_v.y / image_height, 0};

    // location of the upper left pixel
    cam.viewport_up_left = (vec3) {
    cam.center.x - 0.5 * cam.viewport_u.x - 0.5 * cam.viewport_v.x,
    cam.center.y - 0.5 * cam.viewport_u.y - 0.5 * cam.viewport_v.y,
    cam.center.z - cam.focal_length
	};
    // coordinates of the upper-left corner of the viewport
    cam.px_00 = (vec3) {
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
            vec3 px_center = 
			{
                cam.px_00.x + (i * cam.px_delta_u.x),
                cam.px_00.y + (j * cam.px_delta_v.y),
                cam.px_00.z
            };
            vec3 ray_dir = 
			{
                (px_center.x - cam.center.x),
                (px_center.y - cam.center.y),
                (px_center.z - cam.center.z)
            };
            ray r = {cam.center, ray_dir};
            color px_color = ray_color(&r, &sp, &pl, &(lighting){a, l});
            write_color(px_color, &data.img, i, j);
            i++;
        }
        j++;
    }
	/*int j = 0;
	while (j < image_height)
	{
    	int i = 0;
    	while (i < image_width)
	    {
    	    // calculates position of the current pixel
        	vec3 px_center = {
            	cam.px_00.x + (i * cam.px_delta_u.x),
            	cam.px_00.y + (j * cam.px_delta_v.y),
        		cam.px_00.z
        	};
		 // calculates direction based on the pixel's position and cam center
        	vec3 ray_dir = {
            	px_center.x - cam.center.x,
            	px_center.y - cam.center.y,
            	px_center.z - cam.center.z
        	};
			// Rotate the ray direction according to the camera orientation
        	ray_dir = vec3_unit_vector(&ray_dir); // normalize ray direction
        	ray_dir.x += cam.orientation.x;
			ray_dir.y += cam.orientation.y;
			ray_dir.z += cam.orientation.z;

			ray_dir = vec3_unit_vector(&ray_dir); // normalize ray direction again
			ray r = {cam.center, ray_dir}; // create the ray

        	// calculate pixel color and write to image buffer
        	color px_color = ray_color(&r, &sp, &pl, &(lighting){a, l});
        	write_color(px_color, &data.img, i, j);
			i++;
    	}
    	j++;
	}*/

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

#include "../../minirt.h"
#include <stdio.h>
#include <math.h>

// alias for M_PI from math.h
#define PI M_PI

typedef struct
{
    double e[3]; // x, y, z
} vec3;

// aliases for vec3, useful for clarity in the code
typedef vec3 point3;
typedef vec3 color;

typedef struct
{
    vec3 orig; // Origin point of the ray
    vec3 dir;  // Direction of the ray
} ray;

typedef struct {
    vec3 center;
    double radius;
} sphere;

// Utility Functions
double dot(const vec3 *u, const vec3 *v) 
{
    return (u->e[0] * v->e[0] + u->e[1] * v->e[1] + u->e[2] * v->e[2]);
}

double vec3_length_squared(const vec3 *v)
{
    return (v->e[0] * v->e[0] + v->e[1] * v->e[1] + v->e[2] * v->e[2]);
}

// I/O function
void vec3_print(const vec3 *v)
{
    printf("%f %f %f\n", v->e[0], v->e[1], v->e[2]);
}

double hit_sphere(const point3 *center, double radius, const ray *r)
{
    vec3 oc = {r->orig.e[0] - center->e[0], r->orig.e[1] - center->e[1], r->orig.e[2] - center->e[2] };
    double a = vec3_length_squared(&r->dir);
    double half_b = dot(&oc, &r->dir);
    double c = vec3_length_squared(&oc) - radius * radius;
    double discriminant = half_b * half_b - a * c;

    if (discriminant < 0)
        return -1.0;
    else
        return ((-half_b - sqrt(discriminant)) / a);
}

// Write the value of each color component to stdout
void write_color(color pixel_color) 
{
    printf("%d %d %d\n",
           (int)(255.999 * pixel_color.e[0]),
           (int)(255.999 * pixel_color.e[1]),
           (int)(255.999 * pixel_color.e[2]));
}

color ray_color(const ray *r)
{
    point3 center = { 0, 0, -1 };
    double radius = 0.5;
    double t = hit_sphere(&center, radius, r);

    if (t > 0.0)
	{
        vec3 n = { r->orig.e[0] + t * r->dir.e[0], r->orig.e[1] + t * r->dir.e[1], r->orig.e[2] + t * r->dir.e[2] };
        vec3 temp = { 0, 0, -1 };
        vec3 unit_n = { (n.e[0] - temp.e[0]) / radius, (n.e[1] - temp.e[1]) / radius, (n.e[2] - temp.e[2]) / radius };
        color res = { 0.5 * (unit_n.e[0] + 1), 0.5 * (unit_n.e[1] + 1), 0.5 * (unit_n.e[2] + 1) };
        return res;
    }
    vec3 unit_direction = { r->dir.e[0] / sqrt(dot(&r->dir, &r->dir)),
                             r->dir.e[1] / sqrt(dot(&r->dir, &r->dir)),
                             r->dir.e[2] / sqrt(dot(&r->dir, &r->dir)) };
    double a = 0.5 * (unit_direction.e[1] + 1.0);
    color white = { 1.0, 1.0, 1.0 };
    color blue = { 0.5, 0.7, 1.0 };
    color term1 = { (1.0 - a) * white.e[0], (1.0 - a) * white.e[1], (1.0 - a) * white.e[2] };
    color term2 = { a * blue.e[0], a * blue.e[1], a * blue.e[2] };
    color res = { term1.e[0] + term2.e[0], term1.e[1] + term2.e[1], term1.e[2] + term2.e[2] };
    return (res);
}

int main()
{
    // Image
    double aspect_ratio = 16.0 / 9.0;
    int image_width = 400;
    int image_height = (int)(image_width / aspect_ratio); // Calculate image height
	if (image_height < 1)
    	image_height = 1; // ensure height is at least 1

    // Camera
    double focal_length = 1.0;
    double viewport_height = 2.0;
    double viewport_width = viewport_height * ((double)image_width / image_height);
    vec3 camera_center = {0, 0, 0};

    // Calculate the vectors across the horizontal and down the vertical viewport edges.
    vec3 viewport_u = {viewport_width, 0, 0};
    vec3 viewport_v = {0, -viewport_height, 0};

    // Calculate the horizontal and vertical delta vectors from pixel to pixel.
    vec3 pixel_delta_u = {viewport_u.e[0] / image_width, 0, 0};
    vec3 pixel_delta_v = {0, viewport_v.e[1] / image_height, 0};

    // Calculate the location of the upper left pixel.
    vec3 viewport_up_left = {camera_center.e[0] - 0.5 * viewport_u.e[0] - 0.5 * viewport_v.e[0],
                                camera_center.e[1] - 0.5 * viewport_u.e[1] - 0.5 * viewport_v.e[1],
                                camera_center.e[2] - focal_length};
    vec3 pixel00_loc = {viewport_up_left.e[0] + 0.5 * (pixel_delta_u.e[0] + pixel_delta_v.e[0]),
                        viewport_up_left.e[1] + 0.5 * (pixel_delta_u.e[1] + pixel_delta_v.e[1]),
                        viewport_up_left.e[2]};
    
	// Render
    printf("P3\n%d %d\n255\n", image_width, image_height);
    int j = 0;
	while (j < image_height)
	{
    	int i = 0;
    	while (i < image_width)
		{
        	vec3 pixel_center = {pixel00_loc.e[0] + i * pixel_delta_u.e[0], pixel00_loc.e[1] + j * pixel_delta_v.e[1], pixel00_loc.e[2]};
        	vec3 ray_direction = {pixel_center.e[0] - camera_center.e[0], pixel_center.e[1] - camera_center.e[1], pixel_center.e[2] - camera_center.e[2]};
        	ray r = {camera_center, ray_direction};

        	color pixel_color = ray_color(&r);
        	write_color(pixel_color);
        	i++;
    	}
    	j++;
	}
    write(2, "\rDone.\n", 7);
    return (0);
}
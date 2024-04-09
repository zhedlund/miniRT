#include "../../minirt.h"
#include <stdio.h>
#include <math.h>

// alias for M_PI from math.h
#define PI M_PI

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

double dot(const vec3 *u, const vec3 *v) 
{
    return (u->x * v->x + u->y * v->y + u->z * v->z);
}

double vec3_length_squared(const vec3 *v)
{
    return (v->x * v->x + v->y * v->y + v->z * v->z);
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

// Write the value of each color component to stdout
void write_color(color pixel_color) 
{
    printf("%d %d %d\n",
           (int)(255.999 * pixel_color.x),
           (int)(255.999 * pixel_color.y),
           (int)(255.999 * pixel_color.z));
}

color ray_color(const ray *r)
{
    point3 center = { 0, 0, -1 };
    double radius = 0.5;
    double t = hit_sphere(&center, radius, r);

    if (t > 0.0)
	{
        vec3 n = { r->orig.x + t * r->dir.x, r->orig.y + t * r->dir.y, r->orig.z + t * r->dir.z };
        vec3 temp = { 0, 0, -1 };
        vec3 unit_n = { (n.x - temp.x) / radius, (n.y - temp.y) / radius, (n.z - temp.z) / radius };
        color res = { 0.5 * (unit_n.x + 1), 0.5 * (unit_n.y + 1), 0.5 * (unit_n.z + 1) };
        return (res);
    }
    vec3 unit_direction = { r->dir.x / sqrt(dot(&r->dir, &r->dir)),
                             r->dir.y / sqrt(dot(&r->dir, &r->dir)),
                             r->dir.z / sqrt(dot(&r->dir, &r->dir)) };
    double a = 0.5 * (unit_direction.y + 1.0);
    color white = { 1.0, 1.0, 1.0 };
    color blue = { 0.5, 0.7, 1.0 };
    color term1 = { (1.0 - a) * white.x, (1.0 - a) * white.y, (1.0 - a) * white.z };
    color term2 = { a * blue.x, a * blue.y, a * blue.z };
    color res = { term1.x + term2.x, term1.y + term2.y, term1.z + term2.z };
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
    printf("P3\n%d %d\n255\n", image_width, image_height);
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
        	write_color(pixel_color);
        	i++;
    	}
    	j++;
	}
    write(2, "\rDone.\n", 7);
    return (0);
}
#include "../../minirt.h"
#include <stdio.h>
#include <math.h> // Include math.h for the sqrt function

// Constants
#define PI M_PI

typedef struct {
    double e[3];
} vec3;

typedef vec3 point3;

// Structure representing a ray
typedef struct {
    vec3 orig; // Origin point of the ray
    vec3 dir;  // Direction of the ray
} ray;

typedef vec3 color;

typedef struct {
    vec3 center;
    double radius;
} sphere;

// Utility Functions
double dot(const vec3 *u, const vec3 *v) 
{
    return (u->e[0] * v->e[0] + u->e[1] * v->e[1] + u->e[2] * v->e[2]);
}

// Function to calculate the dot product of two vectors
double vec3_dot(const vec3 *v1, const vec3 *v2) 
{
    return (v1->e[0] * v2->e[0] + v1->e[1] * v2->e[1] + v1->e[2] * v2->e[2]);
}

double degrees_to_radians(double degrees) 
{
    return (degrees * PI / 180.0);
}

// Function to negate a vector
vec3 vec3_negate(const vec3 *v) 
{
    return ((vec3){-v->e[0], -v->e[1], -v->e[2]});
}

void write_color(color pixel_color) 
{
    // Write the translated [0,255] value of each color component to stdout
    printf("%d %d %d\n",
           (int)(255.999 * pixel_color.e[0]),
           (int)(255.999 * pixel_color.e[1]),
           (int)(255.999 * pixel_color.e[2]));
}

// Constructors
ray make_ray(vec3 origin, vec3 direction) 
{
    ray r;
    r.orig = origin;
    r.dir = direction;
    return (r);
}

// Member functions
vec3 ray_origin(const ray *r) 
{
    return (r->orig);
}

vec3 ray_direction(const ray *r) 
{
    return (r->dir);
}

// Function declarations for vec3 operations
vec3 vec3_add(const vec3 *u, const vec3 *v);
vec3 vec3_scale(double t, const vec3 *v);

vec3 ray_at(const ray *r, double t) 
{
    vec3 scaled_dir = vec3_scale(t, &(r->dir));
    return (vec3_add(&(r->orig), &scaled_dir));
}

// Constructors
vec3 make_vec3(double e0, double e1, double e2) 
{
    vec3 v;
    v.e[0] = e0;
    v.e[1] = e1;
    v.e[2] = e2;
    return (v);
}

vec3 vec3_zero() 
{
    return (make_vec3(0, 0, 0));
}

// Accessors
double vec3_x(const vec3 *v)
{
	return (v->e[0]);
}

double vec3_y(const vec3 *v)
{
	return (v->e[1]);
}

double vec3_z(const vec3 *v)
{
	return (v->e[2]);
}

// Unary operators
vec3 vec3_neg(const vec3 *v)
{
    return (make_vec3(-v->e[0], -v->e[1], -v->e[2]));
}

// Binary operators
vec3 vec3_add(const vec3 *u, const vec3 *v)
{
    return (make_vec3(u->e[0] + v->e[0], u->e[1] + v->e[1], u->e[2] + v->e[2]));
}

vec3 vec3_sub(const vec3 *u, const vec3 *v)
{
    return (make_vec3(u->e[0] - v->e[0], u->e[1] - v->e[1], u->e[2] - v->e[2]));
}

vec3 vec3_mul(const vec3 *u, const vec3 *v)
{
    return (make_vec3(u->e[0] * v->e[0], u->e[1] * v->e[1], u->e[2] * v->e[2]));
}

vec3 vec3_scale(double t, const vec3 *v)
{
    return (make_vec3(t * v->e[0], t * v->e[1], t * v->e[2]));
}

vec3 vec3_div(const vec3 *v, double t)
{
    return (vec3_scale(1 / t, v));
}

// Utility functions
double vec3_length_squared(const vec3 *v)
{
    return (v->e[0] * v->e[0] + v->e[1] * v->e[1] + v->e[2] * v->e[2]);
}

double vec3_length(const vec3 *v)
{
    return (sqrt(vec3_length_squared(v)));
}

void vec3_unit_vector(vec3 *v)
{
    double len = vec3_length(v);
    v->e[0] /= len;
    v->e[1] /= len;
    v->e[2] /= len;
}

// I/O functions
void vec3_print(const vec3 *v)
{
    printf("%f %f %f\n", v->e[0], v->e[1], v->e[2]);
}

double length_squared(const vec3 *v)
{
    return v->e[0] * v->e[0] + v->e[1] * v->e[1] + v->e[2] * v->e[2];
}

double hit_sphere(const point3 *center, double radius, const ray *r)
{
    vec3 oc = { r->orig.e[0] - center->e[0], r->orig.e[1] - center->e[1], r->orig.e[2] - center->e[2] };
    double a = length_squared(&r->dir);
    double half_b = dot(&oc, &r->dir);
    double c = length_squared(&oc) - radius * radius;
    double discriminant = half_b * half_b - a * c;

    if (discriminant < 0) {
        return -1.0;
    } else {
        return ((-half_b - sqrt(discriminant)) / a);
    }
}

color ray_color(const ray *r)
{
    point3 center = { 0, 0, -1 };
    double radius = 0.5;
    double t = hit_sphere(&center, radius, r);

    if (t > 0.0) {
        vec3 N = { r->orig.e[0] + t * r->dir.e[0], r->orig.e[1] + t * r->dir.e[1], r->orig.e[2] + t * r->dir.e[2] };
        vec3 temp = { 0, 0, -1 };
        vec3 unit_N = { (N.e[0] - temp.e[0]) / radius, (N.e[1] - temp.e[1]) / radius, (N.e[2] - temp.e[2]) / radius };
        color res = { 0.5 * (unit_N.e[0] + 1), 0.5 * (unit_N.e[1] + 1), 0.5 * (unit_N.e[2] + 1) };
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

    // Calculate the image height, and ensure that it's at least 1.
    int image_height = (int)(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;

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
    vec3 viewport_upper_left = {camera_center.e[0] - 0.5 * viewport_u.e[0] - 0.5 * viewport_v.e[0],
                                camera_center.e[1] - 0.5 * viewport_u.e[1] - 0.5 * viewport_v.e[1],
                                camera_center.e[2] - focal_length};
    vec3 pixel00_loc = {viewport_upper_left.e[0] + 0.5 * (pixel_delta_u.e[0] + pixel_delta_v.e[0]),
                        viewport_upper_left.e[1] + 0.5 * (pixel_delta_u.e[1] + pixel_delta_v.e[1]),
                        viewport_upper_left.e[2]};
    
	// Render
    printf("P3\n%d %d\n255\n", image_width, image_height);
    for (int j = 0; j < image_height; ++j) {
        for (int i = 0; i < image_width; ++i) {
            vec3 pixel_center = {pixel00_loc.e[0] + i * pixel_delta_u.e[0], pixel00_loc.e[1] + j * pixel_delta_v.e[1], pixel00_loc.e[2]};
            vec3 ray_direction = {pixel_center.e[0] - camera_center.e[0], pixel_center.e[1] - camera_center.e[1], pixel_center.e[2] - camera_center.e[2]};
            ray r = {camera_center, ray_direction};

            color pixel_color = ray_color(&r);
            write_color(pixel_color);
        }
    }
    fprintf(stderr, "\rDone.                 \n");
    return (0);
}
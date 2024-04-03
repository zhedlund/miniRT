#include <stdio.h>
#include <math.h>
#include <stdlib.h>

typedef struct {
    float x, y, z;
} Vec3;

typedef struct {
    Vec3 center;
    float radius;
} Sphere;

typedef struct {
    Vec3 origin;
    Vec3 direction;
} Ray;


void write_bmp(const char *filename, int width, int height, unsigned char *data) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        printf("Error: Unable to open file %s\n", filename);
        return;
    }

    unsigned int headers[13];
    unsigned int paddedsize, extrabytes;
    unsigned char *data_buffer;

    extrabytes = 4 - ((width * 3) % 4); // How many bytes of padding to add to each
    // horizontal line - the size of which must
    // be a multiple of 4 bytes.
    if (extrabytes == 4)
        extrabytes = 0;

    paddedsize = ((width * 3) + extrabytes) * height;

    // Headers...
    headers[0] = paddedsize + 54; // bfSize (whole file size)
    headers[1] = 0;               // bfReserved (both)
    headers[2] = 54;              // bfOffbits
    headers[3] = 40;              // biSize
    headers[4] = width;           // biWidth
    headers[5] = height;          // biHeight
    headers[6] = 0x00180001;      // biPlanes & biBitcount
    headers[7] = 0;               // biCompression
    headers[8] = paddedsize;      // biSizeImage
    headers[9] = 0;               // biXPelsPerMeter
    headers[10] = 0;              // biYPelsPerMeter
    headers[11] = 0;              // biClrUsed
    headers[12] = 0;              // biClrImportant

    fwrite("BM", 1, 2, file); // bmp signature
    fwrite(&headers[0], 4, 1, file);
    fwrite(&headers[1], 4, 1, file);
    fwrite(&headers[2], 4, 1, file);
    fwrite(&headers[3], 4, 1, file);
    fwrite(&headers[4], 4, 1, file);
    fwrite(&headers[5], 4, 1, file);
    fwrite(&headers[6], 4, 1, file);
    fwrite(&headers[7], 4, 1, file);
    fwrite(&headers[8], 4, 1, file);
    fwrite(&headers[9], 4, 1, file);
    fwrite(&headers[10], 4, 1, file);
    fwrite(&headers[11], 4, 1, file);
    fwrite(&headers[12], 4, 1, file);

    data_buffer = (unsigned char *)malloc(paddedsize);
    if (!data_buffer) {
        printf("Error: Unable to allocate memory for BMP data buffer.\n");
        fclose(file);
        return;
    }

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            unsigned char pixel[3];
            pixel[0] = (unsigned char)(data[(i * width + j) * 3 + 0] * 255); // R
            pixel[1] = (unsigned char)(data[(i * width + j) * 3 + 1] * 255); // G
            pixel[2] = (unsigned char)(data[(i * width + j) * 3 + 2] * 255); // B
            fwrite(pixel, 1, 3, file);
        }
        for (int k = 0; k < extrabytes; k++) {
            fwrite("\0", 1, 1, file);
        }
    }
    free(data_buffer);
    fclose(file);
}



float vec_dot(Vec3 v1, Vec3 v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

int intersect_sphere(Sphere sphere, Ray ray, float *t) {
    // Ray-sphere intersection
    Vec3 oc = { ray.origin.x - sphere.center.x, ray.origin.y - sphere.center.y, ray.origin.z - sphere.center.z };
    float a = vec_dot(ray.direction, ray.direction);
    float b = 2.0 * vec_dot(oc, ray.direction);
    float c = vec_dot(oc, oc) - sphere.radius * sphere.radius;
    float discriminant = b*b - 4*a*c;

    if (discriminant < 0) {
        return 0;
    } else {
        // Find the nearest root that lies in the acceptable range
        float sqrt_discriminant = sqrt(discriminant);
        float t0 = (-b - sqrt_discriminant) / (2.0 * a);
        float t1 = (-b + sqrt_discriminant) / (2.0 * a);
        if (t0 > t1) {
            float temp = t0;
            t0 = t1;
            t1 = temp;
        }
        if (t1 < 0) {
            return 0;
        }
        *t = t0 < 0 ? t1 : t0;
        return 1;
    }
}

Vec3 trace(Ray ray, Sphere sphere) {
    float t;
    if (intersect_sphere(sphere, ray, &t)) {
        // Return sphere color
        return (Vec3){1, 0, 0}; // Red color for simplicity
    } else {
        // Background color
        return (Vec3){0.7, 0.8, 1}; // Light blue
    }
}

int main() {
    int width = 800, height = 600;
    unsigned char data[width * height * 3]; // RGB data for each pixel

    // Define camera and viewport
    Vec3 viewport_origin = {0, 0, 1};
    float viewport_width = 2, viewport_height = 2;
    float pixel_width = viewport_width / width;
    float pixel_height = viewport_height / height;

    // Define scene
    Sphere sphere = {{0, 0, -3}, 1}; // Centered at (0, 0, -3) with radius 1

    // Ray tracing loop
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            // Calculate ray direction for current pixel
            float x = viewport_origin.x + (i + 0.5) * pixel_width - viewport_width / 2;
            float y = viewport_origin.y + (j + 0.5) * pixel_height - viewport_height / 2;
            Ray ray = {{0, 0, 0}, {x, y, -1}}; // Origin at (0, 0, 0), direction towards viewport

            // Trace ray and get pixel color
            Vec3 color = trace(ray, sphere);

            // Store color values in data array (assuming RGB format)
            data[(j * width + i) * 3 + 0] = (unsigned char)(color.x * 255); // R
            data[(j * width + i) * 3 + 1] = (unsigned char)(color.y * 255); // G
            data[(j * width + i) * 3 + 2] = (unsigned char)(color.z * 255); // B
        }
    }

    // Write data to BMP file
    write_bmp("output.bmp", width, height, data);

    return 0;
}



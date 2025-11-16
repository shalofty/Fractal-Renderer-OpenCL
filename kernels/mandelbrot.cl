// Mandelbrot / Julia kernel.
// juliaMode == 0 -> Mandelbrot (c from pixel, z0 = 0)
// juliaMode != 0 -> Julia (c from (juliaRe, juliaImag), z0 from pixel)

// Kernel constants (matches C++ constants.h for consistency).
#define VIEWPORT_SCALE_X 3.5f
#define VIEWPORT_SCALE_Y 2.0f
#define PIXEL_OFFSET 0.5f
#define ESCAPE_RADIUS_SQUARED 4.0f
#define JULIA_MULTIPLIER 2.0f

__kernel void mandelbrot_iterations(__global int* iterations,
                                    int width,
                                    int height,
                                    float centerX,
                                    float centerY,
                                    float zoom,
                                    int maxIterations,
                                    float juliaRe,
                                    float juliaImag,
                                    int juliaMode) {
    const int gx = get_global_id(0);
    const int gy = get_global_id(1);

    if (gx >= width || gy >= height) {
        return;
    }

    const int idx = gy * width + gx;

    // Map pixel coordinate to complex plane.
    float px = ((float)gx / (float)width - PIXEL_OFFSET) * VIEWPORT_SCALE_X / zoom + centerX;
    float py = ((float)gy / (float)height - PIXEL_OFFSET) * VIEWPORT_SCALE_Y / zoom + centerY;

    float x, y;
    float cx, cy;

    if (juliaMode == 0) {
        // Mandelbrot: z0 = 0, c from pixel.
        x = 0.0f;
        y = 0.0f;
        cx = px;
        cy = py;
    } else {
        // Julia: z0 from pixel, c from parameter.
        x = px;
        y = py;
        cx = juliaRe;
        cy = juliaImag;
    }
    int iter = 0;

    while (x * x + y * y <= ESCAPE_RADIUS_SQUARED && iter < maxIterations) {
        float xtemp = x * x - y * y + cx;
        y = JULIA_MULTIPLIER * x * y + cy;
        x = xtemp;
        ++iter;
    }

    iterations[idx] = iter;
}



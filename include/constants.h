// Constants for the OpenCL Fractal Renderer.
// Centralizes magic numbers to improve maintainability and readability.

#pragma once

namespace FractalConstants {

// Default render configuration values.
namespace Defaults {
    constexpr int WIDTH = 1920;
    constexpr int HEIGHT = 1080;
    constexpr int MAX_ITERATIONS = 1000;
    constexpr double CENTER_X = -0.5;
    constexpr double CENTER_Y = 0.0;
    constexpr double ZOOM = 1.0;
    constexpr double JULIA_REAL = -0.7;
    constexpr double JULIA_IMAG = 0.27015;
    constexpr int LOCAL_SIZE_AUTO = 0;  // Let OpenCL choose work-group size.
}

// Color/graphics constants.
namespace Color {
    constexpr unsigned char MAX_RGB = 255;
    constexpr float MAX_RGB_F = 255.0f;
}

// Kernel/mathematical constants.
namespace Kernel {
    // Complex plane viewport scaling factors (from pixel space to complex plane).
    constexpr float VIEWPORT_SCALE_X = 3.5f;
    constexpr float VIEWPORT_SCALE_Y = 2.0f;
    constexpr float PIXEL_OFFSET = 0.5f;  // Center pixel coordinate.
    
    // Mandelbrot/Julia iteration constants.
    constexpr float ESCAPE_RADIUS_SQUARED = 4.0f;  // |z|^2 threshold.
    constexpr float JULIA_MULTIPLIER = 2.0f;  // 2 * z in z^2 + c.
}

// Device/system constants.
namespace Device {
    constexpr size_t INFO_BUFFER_SIZE = 256;  // Size for device name/vendor queries.
}

} // namespace FractalConstants


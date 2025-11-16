// OutputWriter - writes PPM/PNG images from iteration data.
// Uses stb_image_write.h for cross-platform PNG support.

#include "output_writer.h"

#include <algorithm>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "constants.h"

// Include stb_image_write implementation.
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../vendor/stb_image_write.h"

namespace {

// Simple palettes mapped from iterations.
static void defaultPalette(float t, unsigned char& r,
                           unsigned char& g, unsigned char& b) {
    // Blueish gradient.
    using namespace FractalConstants;
    constexpr float minVal = 20.0f;
    constexpr float maxVal = 255.0f;
    r = static_cast<unsigned char>(minVal + (maxVal - minVal) * t);
    g = static_cast<unsigned char>(minVal + (maxVal - minVal) * t);
    b = static_cast<unsigned char>(80.0f + 175.0f * t);
}

static void sunsetPalette(float t, unsigned char& r,
                          unsigned char& g, unsigned char& b) {
    // Deep purple -> orange -> yellow.
    using namespace FractalConstants;
    r = static_cast<unsigned char>(Color::MAX_RGB_F * t);
    g = static_cast<unsigned char>(80.0f + 150.0f * t);
    b = static_cast<unsigned char>(40.0f + 60.0f * (1.0f - t));
}

static void neonPalette(float t, unsigned char& r,
                        unsigned char& g, unsigned char& b) {
    // Neon cyan/magenta mix.
    using namespace FractalConstants;
    r = static_cast<unsigned char>(Color::MAX_RGB_F * t);
    g = static_cast<unsigned char>(Color::MAX_RGB_F * (0.5f + 0.5f * t));
    b = static_cast<unsigned char>(Color::MAX_RGB_F * (0.2f + 0.8f * (1.0f - t)));
}

static void iterationToRGB(int iter, int maxIter, const std::string& palette,
                           unsigned char& r, unsigned char& g, unsigned char& b) {
    if (iter >= maxIter) {
        // Inside the set: black.
        r = g = b = 0;
        return;
    }

    const float t = static_cast<float>(iter) / static_cast<float>(maxIter);
    if (palette == "sunset") {
        sunsetPalette(t, r, g, b);
    } else if (palette == "neon") {
        neonPalette(t, r, g, b);
    } else {
        defaultPalette(t, r, g, b);
    }
}

} // namespace

void OutputWriter::writeImage(const RenderConfig& cfg,
                              const std::vector<int>& iterations,
                              const std::string& path) const {
    // Very simple extension check.
    auto hasSuffix = [](const std::string& s, const std::string& suffix) {
        return s.size() >= suffix.size() &&
               s.compare(s.size() - suffix.size(), suffix.size(), suffix) == 0;
    };

    if (hasSuffix(path, ".png")) {
        writePNG(cfg, iterations, path);
    } else {
        // Default to PPM.
        writePPM(cfg, iterations, path);
    }
}

void OutputWriter::writePPM(const RenderConfig& cfg,
                            const std::vector<int>& iterations,
                            const std::string& path) const {
    const int width = cfg.width;
    const int height = cfg.height;
    const int maxIter = std::max(1, cfg.maxIterations);

    if (static_cast<int>(iterations.size()) != width * height) {
        throw std::runtime_error("Iteration buffer size does not match image dimensions");
    }

    std::ofstream out(path, std::ios::binary);
    if (!out) {
        throw std::runtime_error("Failed to open output image file: " + path);
    }

    // PPM header (P6).
    out << "P6\n" << width << " " << height << "\n255\n";

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            const int idx = y * width + x;
            unsigned char r = 0, g = 0, b = 0;
            iterationToRGB(iterations[idx], maxIter, cfg.palette, r, g, b);
            out.write(reinterpret_cast<const char*>(&r), 1);
            out.write(reinterpret_cast<const char*>(&g), 1);
            out.write(reinterpret_cast<const char*>(&b), 1);
        }
    }

    if (!out) {
        throw std::runtime_error("Failed while writing PPM image data");
    }
}

void OutputWriter::writePNG(const RenderConfig& cfg,
                            const std::vector<int>& iterations,
                            const std::string& path) const {
    const int width = cfg.width;
    const int height = cfg.height;
    const int maxIter = std::max(1, cfg.maxIterations);

    if (static_cast<int>(iterations.size()) != width * height) {
        throw std::runtime_error("Iteration buffer size does not match image dimensions");
    }

    // Allocate RGB buffer (row-major order, 3 bytes per pixel).
    std::vector<unsigned char> rgbData(width * height * 3);

    // Convert iterations to RGB using palette.
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            const int idx = y * width + x;
            unsigned char r = 0, g = 0, b = 0;
            iterationToRGB(iterations[idx], maxIter, cfg.palette, r, g, b);
            
            // stb_image_write expects row-major, RGB interleaved.
            const int pixelIdx = (y * width + x) * 3;
            rgbData[pixelIdx + 0] = r;
            rgbData[pixelIdx + 1] = g;
            rgbData[pixelIdx + 2] = b;
        }
    }

    // Write PNG using stb_image_write.
    const int stride = width * 3;  // Bytes per row.
    const int result = stbi_write_png(path.c_str(), width, height, 3,
                                      rgbData.data(), stride);
    if (result == 0) {
        throw std::runtime_error("Failed to write PNG image: " + path);
    }
}



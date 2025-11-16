// Render configuration and builder for the OpenCL Fractal Renderer.
// This is a lightweight value type used to pass user options into the renderer.

#pragma once

#include <string>

#include "constants.h"

struct RenderConfig {
    int width = FractalConstants::Defaults::WIDTH;
    int height = FractalConstants::Defaults::HEIGHT;
    int maxIterations = FractalConstants::Defaults::MAX_ITERATIONS;

    // "mandelbrot" or "julia"
    std::string fractalType = "mandelbrot";

    // Complex plane center
    double centerX = FractalConstants::Defaults::CENTER_X;
    double centerY = FractalConstants::Defaults::CENTER_Y;

    double zoom = FractalConstants::Defaults::ZOOM;

    // Julia set parameter c = juliaReal + i * juliaImag
    double juliaReal = FractalConstants::Defaults::JULIA_REAL;
    double juliaImag = FractalConstants::Defaults::JULIA_IMAG;

    // Optional work-group size override (0 = let OpenCL decide).
    int localSizeX = FractalConstants::Defaults::LOCAL_SIZE_AUTO;
    int localSizeY = FractalConstants::Defaults::LOCAL_SIZE_AUTO;

    std::string palette = "default";
    std::string outputPath = "images/fractal.png";  // Default output goes to images/.

    struct Builder;

    static Builder builder();
};

struct RenderConfig::Builder {
    RenderConfig cfg{};

    Builder& width(int w) { cfg.width = w; return *this; }
    Builder& height(int h) { cfg.height = h; return *this; }
    Builder& maxIterations(int it) { cfg.maxIterations = it; return *this; }
    Builder& fractalType(const std::string& t) { cfg.fractalType = t; return *this; }
    Builder& center(double x, double y) { cfg.centerX = x; cfg.centerY = y; return *this; }
    Builder& zoom(double z) { cfg.zoom = z; return *this; }
    Builder& palette(const std::string& p) { cfg.palette = p; return *this; }
    Builder& outputPath(const std::string& path) { cfg.outputPath = path; return *this; }
    Builder& julia(double real, double imag) { cfg.juliaReal = real; cfg.juliaImag = imag; return *this; }
    Builder& localSize(int lx, int ly) { cfg.localSizeX = lx; cfg.localSizeY = ly; return *this; }

    RenderConfig build() const { return cfg; }
};

inline RenderConfig::Builder RenderConfig::builder() { return Builder{}; }



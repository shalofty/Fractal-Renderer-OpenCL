// Stub implementations for MandelbrotStrategy and JuliaStrategy.

#include "fractal_strategy.h"

#include <iostream>

void MandelbrotStrategy::configure(const RenderConfig& cfg) {
    std::cout << "[Strategy] Configuring Mandelbrot with "
              << cfg.width << "x" << cfg.height
              << ", maxIterations=" << cfg.maxIterations
              << ", center=(" << cfg.centerX << ", " << cfg.centerY << ")"
              << ", zoom=" << cfg.zoom << "\n";
}

void JuliaStrategy::configure(const RenderConfig& cfg) {
    std::cout << "[Strategy] Configuring Julia with "
              << cfg.width << "x" << cfg.height
              << ", maxIterations=" << cfg.maxIterations
              << ", center=(" << cfg.centerX << ", " << cfg.centerY << ")"
              << ", zoom=" << cfg.zoom
              << ", c=(" << cfg.juliaReal << ", " << cfg.juliaImag << ")"
              << "\n";
}



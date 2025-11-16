// FractalStrategy - strategy interface for different fractal types.

#pragma once

#include <string>

#include "config.h"

class FractalStrategy {
public:
    virtual ~FractalStrategy() = default;

    // Human-readable name for diagnostics.
    virtual std::string name() const = 0;

    // In the real implementation, this will configure kernels and buffers
    // according to the chosen fractal type.
    virtual void configure(const RenderConfig& cfg) = 0;
};

class MandelbrotStrategy : public FractalStrategy {
public:
    std::string name() const override { return "Mandelbrot"; }
    void configure(const RenderConfig& cfg) override;
};

class JuliaStrategy : public FractalStrategy {
public:
    std::string name() const override { return "Julia"; }
    void configure(const RenderConfig& cfg) override;
};



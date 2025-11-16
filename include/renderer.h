// Renderer - orchestrates the rendering pipeline using strategies and managers.

#pragma once

#include <memory>

#include "config.h"
#include "device_manager.h"
#include "kernel_manager.h"
#include "memory_manager.h"
#include "fractal_strategy.h"

class Renderer {
public:
    Renderer(DeviceManager& deviceManager,
             KernelManager& kernelManager,
             MemoryManager& memoryManager);

    // Set the active fractal strategy.
    void setStrategy(std::unique_ptr<FractalStrategy> strategy);

    // Perform a render using the active strategy and Mandelbrot kernel.
    void render(const RenderConfig& cfg);

private:
    DeviceManager& deviceManager_;
    KernelManager& kernelManager_;
    MemoryManager& memoryManager_;
    std::unique_ptr<FractalStrategy> strategy_;
};



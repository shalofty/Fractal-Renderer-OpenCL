// Renderer implementation - dispatch Mandelbrot kernel and read iteration buffer.

#include "renderer.h"

#include <fstream>
#include <iostream>
#include <stdexcept>

#include "output_writer.h"

Renderer::Renderer(DeviceManager& deviceManager,
                   KernelManager& kernelManager,
                   MemoryManager& memoryManager)
    : deviceManager_(deviceManager)
    , kernelManager_(kernelManager)
    , memoryManager_(memoryManager)
{
}

void Renderer::setStrategy(std::unique_ptr<FractalStrategy> strategy) {
    strategy_ = std::move(strategy);
}

namespace {

void printKernelTimeMs(const char* label, cl_event evt) {
    if (!evt) {
        return;
    }
    cl_ulong startNs = 0;
    cl_ulong endNs = 0;
    clGetEventProfilingInfo(evt, CL_PROFILING_COMMAND_START,
                            sizeof(startNs), &startNs, nullptr);
    clGetEventProfilingInfo(evt, CL_PROFILING_COMMAND_END,
                            sizeof(endNs), &endNs, nullptr);
    const double ms = static_cast<double>(endNs - startNs) * 1e-6;
    std::cout << "[" << label << "] " << ms << " ms\n";
}

} // namespace

void Renderer::render(const RenderConfig& cfg) {
    if (!strategy_) {
        std::cerr << "[Renderer] No strategy set; cannot render.\n";
        return;
    }

    std::cout << "[Renderer] Starting render using strategy: "
              << strategy_->name() << "\n";
    strategy_->configure(cfg);

    memoryManager_.initialize(cfg);

    cl_kernel kernel = kernelManager_.mandelbrotKernel();
    if (!kernel) {
        throw std::runtime_error("Mandelbrot kernel not initialized");
    }

    cl_int err = CL_SUCCESS;
    const int width = cfg.width;
    const int height = cfg.height;
    const float centerX = static_cast<float>(cfg.centerX);
    const float centerY = static_cast<float>(cfg.centerY);
    const float zoom = static_cast<float>(cfg.zoom);
    const int maxIterations = cfg.maxIterations;
    const float juliaRe = static_cast<float>(cfg.juliaReal);
    const float juliaImag = static_cast<float>(cfg.juliaImag);
    const int juliaMode = (cfg.fractalType == "julia") ? 1 : 0;

    cl_mem iterationsBuf = memoryManager_.iterationBuffer();

    err  = clSetKernelArg(kernel, 0, sizeof(cl_mem), &iterationsBuf);
    err |= clSetKernelArg(kernel, 1, sizeof(int), &width);
    err |= clSetKernelArg(kernel, 2, sizeof(int), &height);
    err |= clSetKernelArg(kernel, 3, sizeof(float), &centerX);
    err |= clSetKernelArg(kernel, 4, sizeof(float), &centerY);
    err |= clSetKernelArg(kernel, 5, sizeof(float), &zoom);
    err |= clSetKernelArg(kernel, 6, sizeof(int), &maxIterations);
    err |= clSetKernelArg(kernel, 7, sizeof(float), &juliaRe);
    err |= clSetKernelArg(kernel, 8, sizeof(float), &juliaImag);
    err |= clSetKernelArg(kernel, 9, sizeof(int), &juliaMode);
    if (err != CL_SUCCESS) {
        throw std::runtime_error("Failed to set Mandelbrot kernel arguments");
    }

    const size_t globalSize[2] = {
        static_cast<size_t>(width),
        static_cast<size_t>(height)
    };

    const size_t* localSizePtr = nullptr;
    size_t localSize[2];
    if (cfg.localSizeX > 0 && cfg.localSizeY > 0) {
        localSize[0] = static_cast<size_t>(cfg.localSizeX);
        localSize[1] = static_cast<size_t>(cfg.localSizeY);
        localSizePtr = localSize;
    }

    cl_event evt = nullptr;
    err = clEnqueueNDRangeKernel(deviceManager_.commandQueue(),
                                 kernel,
                                 2,
                                 nullptr,
                                 globalSize,
                                 localSizePtr,
                                 0,
                                 nullptr,
                                 &evt);
    if (err != CL_SUCCESS) {
        throw std::runtime_error("Failed to enqueue Mandelbrot kernel");
    }

    clFinish(deviceManager_.commandQueue());
    printKernelTimeMs("Fractal kernel", evt);
    if (evt) {
        clReleaseEvent(evt);
    }

    auto& hostIters = memoryManager_.hostIterationBuffer();
    const size_t byteCount = hostIters.size() * sizeof(int);
    err = clEnqueueReadBuffer(deviceManager_.commandQueue(),
                              iterationsBuf,
                              CL_TRUE,
                              0,
                              byteCount,
                              hostIters.data(),
                              0,
                              nullptr,
                              nullptr);
    if (err != CL_SUCCESS) {
        throw std::runtime_error("Failed to read Mandelbrot iteration buffer");
    }

    std::cout << "[Renderer] Mandelbrot/Julia iterations computed. ("
              << hostIters.size() << " pixels)\n";

    // Ensure output goes to images/ directory (unless path is absolute or contains directory separators).
    std::string outputPath = cfg.outputPath;
    if (!outputPath.empty() &&
        outputPath.find('/') == std::string::npos &&
        outputPath.find('\\') == std::string::npos &&
        outputPath[0] != '/') {
        outputPath = "images/" + outputPath;
    }

    OutputWriter writer;
    writer.writeImage(cfg, hostIters, outputPath);
    std::cout << "[Renderer] Wrote image to '" << outputPath << "'\n";
}



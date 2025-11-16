// MemoryManager implementation - allocation for Mandelbrot iteration buffer.

#include "memory_manager.h"

#include <stdexcept>

MemoryManager::MemoryManager(DeviceManager& deviceManager)
    : deviceManager_(deviceManager) {}

MemoryManager::~MemoryManager() {
    if (iterationBuffer_) {
        clReleaseMemObject(iterationBuffer_);
    }
}

void MemoryManager::initialize(const RenderConfig& cfg) {
    const size_t pixelCount = static_cast<size_t>(cfg.width) * static_cast<size_t>(cfg.height);
    hostIterations_.assign(pixelCount, 0);

    cl_int err = CL_SUCCESS;
    const size_t bytes = pixelCount * sizeof(int);
    iterationBuffer_ = clCreateBuffer(deviceManager_.context(),
                                      CL_MEM_WRITE_ONLY,
                                      bytes,
                                      nullptr,
                                      &err);
    if (err != CL_SUCCESS || !iterationBuffer_) {
        throw std::runtime_error("Failed to create OpenCL iteration buffer");
    }
}



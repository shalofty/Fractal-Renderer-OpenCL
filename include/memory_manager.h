// MemoryManager - manages OpenCL buffers and host-side image data.

#pragma once

#include <vector>

#include <OpenCL/opencl.h>

#include "config.h"
#include "device_manager.h"

class MemoryManager {
public:
    explicit MemoryManager(DeviceManager& deviceManager);
    ~MemoryManager();

    void initialize(const RenderConfig& cfg);

    cl_mem iterationBuffer() const { return iterationBuffer_; }
    std::vector<int>& hostIterationBuffer() { return hostIterations_; }

private:
    DeviceManager& deviceManager_;
    cl_mem iterationBuffer_{};
    std::vector<int> hostIterations_;
};


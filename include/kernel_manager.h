// KernelManager - stub for loading and building OpenCL kernels.

#pragma once

#include <string>

#include <OpenCL/opencl.h>

class KernelManager {
public:
    KernelManager() = default;
    ~KernelManager();

    // Load sources and build programs.
    void initialize(const std::string& kernelsRoot, cl_context context, cl_device_id device);

    // Diagnostics about available kernel sources.
    void printDiagnostics() const;

    cl_kernel mandelbrotKernel() const { return mandelbrotKernel_; }

private:
    std::string kernelsRoot_{"kernels"};
    cl_program program_{};
    cl_kernel mandelbrotKernel_{};
};





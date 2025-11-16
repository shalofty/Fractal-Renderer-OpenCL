// KernelManager - load and build OpenCL kernels.

#include "kernel_manager.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

namespace {

std::string readTextFile(const std::string& path) {
    std::ifstream in(path);
    if (!in) {
        throw std::runtime_error("Failed to open kernel file: " + path);
    }
    std::ostringstream ss;
    ss << in.rdbuf();
    return ss.str();
}

} // namespace

KernelManager::~KernelManager() {
    if (mandelbrotKernel_) {
        clReleaseKernel(mandelbrotKernel_);
    }
    if (program_) {
        clReleaseProgram(program_);
    }
}

void KernelManager::initialize(const std::string& kernelsRoot,
                               cl_context context,
                               cl_device_id device) {
    kernelsRoot_ = kernelsRoot;

    const std::string mandelbrotPath = kernelsRoot_ + "/mandelbrot.cl";
    std::string source = readTextFile(mandelbrotPath);
    const char* srcPtr = source.c_str();
    const size_t srcLen = source.size();

    cl_int err = CL_SUCCESS;
    program_ = clCreateProgramWithSource(context, 1, &srcPtr, &srcLen, &err);
    if (err != CL_SUCCESS || !program_) {
        throw std::runtime_error("Failed to create OpenCL program from mandelbrot.cl");
    }

    err = clBuildProgram(program_, 1, &device, nullptr, nullptr, nullptr);
    if (err != CL_SUCCESS) {
        // Try to fetch and print the build log for easier debugging.
        size_t logSize = 0;
        clGetProgramBuildInfo(program_, device, CL_PROGRAM_BUILD_LOG, 0, nullptr, &logSize);
        std::string log(logSize, '\0');
        if (logSize > 0) {
            clGetProgramBuildInfo(program_, device, CL_PROGRAM_BUILD_LOG,
                                  logSize, &log[0], nullptr);
        }
        std::cerr << "[Kernels] Build log for mandelbrot program:\n" << log << "\n";
        throw std::runtime_error("Failed to build OpenCL program (mandelbrot)");
    }

    mandelbrotKernel_ = clCreateKernel(program_, "mandelbrot_iterations", &err);
    if (err != CL_SUCCESS || !mandelbrotKernel_) {
        throw std::runtime_error("Failed to create mandelbrot_iterations kernel");
    }
}

void KernelManager::printDiagnostics() const {
    std::cout << "[Kernels] KernelManager initialized with root: " << kernelsRoot_ << "\n";
    std::cout << "[Kernels]  - mandelbrot_iterations kernel: "
              << (mandelbrotKernel_ ? "ready" : "NOT READY")
              << "\n";
}





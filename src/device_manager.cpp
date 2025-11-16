// DeviceManager - basic OpenCL device/context/queue setup.

#include "device_manager.h"

#include <iostream>
#include <vector>

#include "constants.h"

DeviceManager::DeviceManager() = default;

DeviceManager::~DeviceManager() {
    if (queue_) {
        clReleaseCommandQueue(queue_);
    }
    if (context_) {
        clReleaseContext(context_);
    }
}

void DeviceManager::initialize() {
    cl_int err = CL_SUCCESS;

    cl_uint numPlatforms = 0;
    err = clGetPlatformIDs(0, nullptr, &numPlatforms);
    if (err != CL_SUCCESS || numPlatforms == 0) {
        throw std::runtime_error("No OpenCL platforms found");
    }

    std::vector<cl_platform_id> platforms(numPlatforms);
    err = clGetPlatformIDs(numPlatforms, platforms.data(), nullptr);
    if (err != CL_SUCCESS) {
        throw std::runtime_error("Failed to get OpenCL platform IDs");
    }

    // Pick the first platform with a GPU, otherwise fall back to CPU on the first platform.
    platform_ = platforms[0];
    cl_device_type desiredTypes[] = {CL_DEVICE_TYPE_GPU, CL_DEVICE_TYPE_CPU};
    cl_device_id chosenDevice = nullptr;

    for (cl_device_type type : desiredTypes) {
        cl_uint numDevices = 0;
        err = clGetDeviceIDs(platform_, type, 0, nullptr, &numDevices);
        if (err == CL_SUCCESS && numDevices > 0) {
            std::vector<cl_device_id> devices(numDevices);
            err = clGetDeviceIDs(platform_, type, numDevices, devices.data(), nullptr);
            if (err == CL_SUCCESS && !devices.empty()) {
                chosenDevice = devices[0];
                break;
            }
        }
    }

    if (!chosenDevice) {
        throw std::runtime_error("No suitable OpenCL device found");
    }

    device_ = chosenDevice;

    // Query name and vendor.
    char nameBuf[FractalConstants::Device::INFO_BUFFER_SIZE] = {};
    char vendorBuf[FractalConstants::Device::INFO_BUFFER_SIZE] = {};
    clGetDeviceInfo(device_, CL_DEVICE_NAME, sizeof(nameBuf), nameBuf, nullptr);
    clGetDeviceInfo(device_, CL_DEVICE_VENDOR, sizeof(vendorBuf), vendorBuf, nullptr);
    deviceName_ = nameBuf;
    deviceVendor_ = vendorBuf;

    // Create context.
    context_ = clCreateContext(nullptr, 1, &device_, nullptr, nullptr, &err);
    if (err != CL_SUCCESS || !context_) {
        throw std::runtime_error("Failed to create OpenCL context");
    }

    // Create command queue (OpenCL 1.2 style) with profiling enabled.
    queue_ = clCreateCommandQueue(context_, device_, CL_QUEUE_PROFILING_ENABLE, &err);
    if (err != CL_SUCCESS || !queue_) {
        throw std::runtime_error("Failed to create OpenCL command queue");
    }
}

void DeviceManager::printDiagnostics() const {
    if (!device_) {
        std::cout << "[Device] No OpenCL device initialized\n";
        return;
    }

    size_t wgSize = 0;
    clGetDeviceInfo(device_, CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(wgSize), &wgSize, nullptr);

    cl_bool imageSupport = CL_FALSE;
    clGetDeviceInfo(device_, CL_DEVICE_IMAGE_SUPPORT, sizeof(imageSupport), &imageSupport, nullptr);

    std::cout << "[Device] Selected device: " << deviceName_ << " (" << deviceVendor_ << ")\n";
    std::cout << "[Device]  Max work-group size: " << wgSize << "\n";
    std::cout << "[Device]  Image support      : " << (imageSupport ? "yes" : "no") << "\n";
}




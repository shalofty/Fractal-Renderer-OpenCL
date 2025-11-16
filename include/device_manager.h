// DeviceManager - OpenCL device selection and core context/queue management.

#pragma once

#include <string>

#include <OpenCL/opencl.h>

class DeviceManager {
public:
    DeviceManager();
    ~DeviceManager();

    // Query platforms/devices and pick a default GPU (fallback to CPU).
    void initialize();

    // Print basic device info.
    void printDiagnostics() const;

    std::string deviceName() const { return deviceName_; }

    cl_context context() const { return context_; }
    cl_command_queue commandQueue() const { return queue_; }
    cl_device_id device() const { return device_; }

private:
    std::string deviceName_{"(no device initialized)"};
    std::string deviceVendor_{"(unknown vendor)"};

    cl_platform_id platform_{};
    cl_device_id device_{};
    cl_context context_{};
    cl_command_queue queue_{};
};




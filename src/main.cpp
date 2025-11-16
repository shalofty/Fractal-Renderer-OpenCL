// OpenCL Fractal Renderer - main entry point.

#include <iostream>
#include <stdexcept>

#include "cli_parser.h"
#include "device_manager.h"
#include "kernel_manager.h"
#include "memory_manager.h"
#include "renderer.h"
#include "fractal_strategy.h"

int main(int argc, char** argv) {
    try {
        RenderConfig cfg = parse_args(argc, argv);
        std::cout << "OpenCL Fractal Renderer scaffold.\n";
        print_config_summary(cfg);

        // Initialize core host-side managers.
        DeviceManager deviceManager;
        deviceManager.initialize();
        deviceManager.printDiagnostics();

        KernelManager kernelManager;
        kernelManager.initialize("kernels", deviceManager.context(), deviceManager.device());
        kernelManager.printDiagnostics();

        MemoryManager memoryManager(deviceManager);

        Renderer renderer(deviceManager, kernelManager, memoryManager);
        if (cfg.fractalType == "julia") {
            renderer.setStrategy(std::make_unique<JuliaStrategy>());
        } else {
            renderer.setStrategy(std::make_unique<MandelbrotStrategy>());
        }

        renderer.render(cfg);
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << "\n\n";
        print_help();
        return 1;
    }

    return 0;
}




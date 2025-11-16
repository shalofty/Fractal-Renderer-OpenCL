// OpenCL Fractal Renderer - initial scaffold with basic CLI and config parsing.

#include <iostream>
#include <stdexcept>
#include <string>

#include "config.h"
#include "constants.h"
#include "device_manager.h"
#include "kernel_manager.h"
#include "memory_manager.h"
#include "renderer.h"
#include "fractal_strategy.h"

namespace {

void print_help() {
    std::cout
        << "OpenCL Fractal Renderer (scaffold)\n\n"
        << "Usage:\n"
        << "  fractal_renderer [options]\n\n"
        << "Options:\n"
        << "  --type mandelbrot|julia       Fractal type (default: mandelbrot)\n"
        << "  --width <int>                 Image width in pixels (default: "
        << FractalConstants::Defaults::WIDTH << ")\n"
        << "  --height <int>                Image height in pixels (default: "
        << FractalConstants::Defaults::HEIGHT << ")\n"
        << "  --iterations <int>            Max iterations (default: "
        << FractalConstants::Defaults::MAX_ITERATIONS << ")\n"
        << "  --center <real> <imag>        Complex plane center (default: "
        << FractalConstants::Defaults::CENTER_X << " "
        << FractalConstants::Defaults::CENTER_Y << ")\n"
        << "  --zoom <real>                 Zoom factor (default: "
        << FractalConstants::Defaults::ZOOM << ")\n"
        << "  --julia-real <real>           Julia parameter real part (default: "
        << FractalConstants::Defaults::JULIA_REAL << ")\n"
        << "  --julia-imag <real>           Julia parameter imaginary part (default: "
        << FractalConstants::Defaults::JULIA_IMAG << ")\n"
        << "  --local-size-x <int>          Optional local work-group size in X (default: auto)\n"
        << "  --local-size-y <int>          Optional local work-group size in Y (default: auto)\n"
        << "  --palette <name>              Color palette name (default: default)\n"
        << "  --output <file>               Output image path (default: fractal.png/ppm/png)\n"
        << "  -h, --help                    Show this help and exit\n";
}

RenderConfig parse_args(int argc, char** argv) {
    auto builder = RenderConfig::builder();

    for (int i = 1; i < argc; ++i) {
        std::string arg{argv[i]};
        if (arg == "--help" || arg == "-h") {
            print_help();
            std::exit(0);
        } else if (arg == "--type" && i + 1 < argc) {
            builder.fractalType(argv[++i]);
        } else if (arg == "--width" && i + 1 < argc) {
            builder.width(std::stoi(argv[++i]));
        } else if (arg == "--height" && i + 1 < argc) {
            builder.height(std::stoi(argv[++i]));
        } else if (arg == "--iterations" && i + 1 < argc) {
            builder.maxIterations(std::stoi(argv[++i]));
        } else if (arg == "--center" && i + 2 < argc) {
            double x = std::stod(argv[++i]);
            double y = std::stod(argv[++i]);
            builder.center(x, y);
        } else if (arg == "--zoom" && i + 1 < argc) {
            builder.zoom(std::stod(argv[++i]));
        } else if (arg == "--julia-real" && i + 1 < argc) {
            double jr = std::stod(argv[++i]);
            builder.julia(jr, builder.build().juliaImag);
        } else if (arg == "--julia-imag" && i + 1 < argc) {
            double ji = std::stod(argv[++i]);
            builder.julia(builder.build().juliaReal, ji);
        } else if (arg == "--palette" && i + 1 < argc) {
            builder.palette(argv[++i]);
        } else if (arg == "--local-size-x" && i + 1 < argc) {
            int lx = std::stoi(argv[++i]);
            builder.localSize(lx, builder.build().localSizeY);
        } else if (arg == "--local-size-y" && i + 1 < argc) {
            int ly = std::stoi(argv[++i]);
            builder.localSize(builder.build().localSizeX, ly);
        } else if (arg == "--output" && i + 1 < argc) {
            builder.outputPath(argv[++i]);
        } else {
            throw std::runtime_error("Unknown or incomplete argument: " + arg);
        }
    }

    return builder.build();
}

void print_config_summary(const RenderConfig& cfg) {
    std::cout << "Render configuration:\n"
              << "  Type       : " << cfg.fractalType << "\n"
              << "  Size       : " << cfg.width << "x" << cfg.height << "\n"
              << "  Iterations : " << cfg.maxIterations << "\n"
              << "  Center     : (" << cfg.centerX << ", " << cfg.centerY << ")\n"
              << "  Zoom       : " << cfg.zoom << "\n"
              << "  Palette    : " << cfg.palette << "\n"
              << "  Output     : " << cfg.outputPath << "\n";
}

} // namespace

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




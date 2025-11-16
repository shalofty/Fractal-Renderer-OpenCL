// Command-line interface parser for the OpenCL Fractal Renderer.
// Provides functions for parsing CLI arguments and displaying help/configuration.

#pragma once

#include "config.h"

// Prints usage help message.
void print_help();

// Parses command-line arguments and returns a RenderConfig.
// Throws std::runtime_error on invalid arguments.
RenderConfig parse_args(int argc, char** argv);

// Prints a summary of the render configuration.
void print_config_summary(const RenderConfig& cfg);


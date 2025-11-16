// OutputWriter - simple image writer for iteration data.
// Supports PPM directly and PNG via stb_image_write (cross-platform).

#pragma once

#include <string>
#include <vector>

#include "config.h"

class OutputWriter {
public:
    // Choose format based on file extension:
    //  - ".ppm": write PPM directly
    //  - ".png": write PNG using stb_image_write (cross-platform)
    void writeImage(const RenderConfig& cfg,
                    const std::vector<int>& iterations,
                    const std::string& path) const;

    // Write a PPM image based on iteration counts.
    void writePPM(const RenderConfig& cfg,
                  const std::vector<int>& iterations,
                  const std::string& path) const;

private:
    // Write a PNG image using stb_image_write.
    void writePNG(const RenderConfig& cfg,
                  const std::vector<int>& iterations,
                  const std::string& path) const;
};



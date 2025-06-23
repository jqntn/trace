#include <cstdint>
#include <cstdlib>
#include <vector>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

int
main()
{
  uint16_t image_width = 256;
  uint16_t image_height = 256;
  uint8_t image_channels = 3;

  std::vector<uint8_t> pixels;

  for (size_t j = 0; j < image_height; j++) {
    for (size_t i = 0; i < image_width; i++) {
      float r = i / float(image_width - 1);
      float g = j / float(image_height - 1);
      float b = 0.0f;

      pixels.emplace_back(255.999f * r);
      pixels.emplace_back(255.999f * g);
      pixels.emplace_back(255.999f * b);
    }
  }

  stbi_write_png("out/out.png",
                 image_width,
                 image_height,
                 image_channels,
                 pixels.data(),
                 image_width * image_channels);

  std::system("start out/out.png");

  return EXIT_SUCCESS;
}
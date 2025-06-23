#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <vector>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define RGFW_IMPLEMENTATION
#define RGFW_BUFFER
#include "RGFW.h"

int
main()
{
  uint16_t image_width = 512;
  uint16_t image_height = 512;
  uint8_t image_channels = 4;

  RGFW_window* p_window =
    RGFW_createWindow("trace",
                      RGFW_RECT(0, 0, image_width, image_height),
                      RGFW_windowCenter | RGFW_windowNoResize);

  while (RGFW_window_shouldClose(p_window) == RGFW_FALSE) {
    while (RGFW_window_checkEvent(p_window)) {
      if (p_window->event.type == RGFW_quit) {
        break;
      }

      if (RGFW_isMousePressed(p_window, RGFW_mouseLeft)) {
        std::vector<uint8_t> pixels;
        for (size_t j = 0; j < image_height; j++) {
          for (size_t i = 0; i < image_width; i++) {
            float r = i / float(image_width - 1);
            float g = j / float(image_height - 1);
            float b = 0.0f;
            pixels.emplace_back(255.999f * r);
            pixels.emplace_back(255.999f * g);
            pixels.emplace_back(255.999f * b);
            pixels.emplace_back(255);
          }
        }

        std::copy(pixels.begin(), pixels.end(), p_window->buffer);
        RGFW_window_swapBuffers(p_window);

        stbi_write_png("out/out.png",
                       image_width,
                       image_height,
                       image_channels,
                       pixels.data(),
                       image_width * image_channels);
      }
    }
  }

  RGFW_window_close(p_window);

  return EXIT_SUCCESS;
}
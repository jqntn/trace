#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include <thirdparty/stb_image_resize2.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <thirdparty/stb_image_write.h>

#define RGFW_IMPLEMENTATION
#define RGFW_BUFFER
#include <thirdparty/RGFW.h>

#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <vector>

int
main()
{
  uint16_t window_width = 512;
  uint16_t window_height = 512;

  uint16_t image_width = 256;
  uint16_t image_height = 256;
  uint8_t image_channels = 4;

  RGFW_window* p_window =
    RGFW_createWindow("trace",
                      RGFW_RECT(0, 0, window_width, window_height),
                      RGFW_windowCenter | RGFW_windowNoResize);

  while (RGFW_window_shouldClose(p_window) == RGFW_FALSE) {
    while (RGFW_window_checkEvent(p_window)) {
      if (p_window->event.type == RGFW_quit) {
        break;
      }

      if (RGFW_isMousePressed(p_window, RGFW_mouseLeft)) {
        std::vector<uint8_t> pixels;
        for (size_t j = 0; j < image_height; j++) {
          std::print(std::clog, "\rScanlines remaining: {} ", image_height - j);
          std::flush(std::clog);
          for (size_t i = 0; i < image_width; i++) {
            double r = i / double(image_width - 1);
            double g = j / double(image_height - 1);
            double b = 0.0;
            pixels.emplace_back(255.999 * r);
            pixels.emplace_back(255.999 * g);
            pixels.emplace_back(255.999 * b);
            pixels.emplace_back(255);
          }
        }

        stbir_resize(pixels.data(),
                     image_width,
                     image_height,
                     image_width * image_channels,
                     p_window->buffer,
                     window_width,
                     window_height,
                     window_width * image_channels,
                     STBIR_RGBA,
                     STBIR_TYPE_UINT8,
                     STBIR_EDGE_ZERO,
                     STBIR_FILTER_POINT_SAMPLE);

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
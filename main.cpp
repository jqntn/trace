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
#include <ray.hpp>
#include <utility>
#include <vec3.hpp>
#include <vector>

static void
emplace_color(std::vector<uint8_t>& pixels, const color& c)
{
  pixels.emplace_back(255.999 * c.r());
  pixels.emplace_back(255.999 * c.g());
  pixels.emplace_back(255.999 * c.b());
  pixels.emplace_back(255);
}

static color
ray_color(const ray& r)
{
  color white = color(1.0, 1.0, 1.0);
  color sky_blue = color(0.5, 0.7, 1.0);

  vec3 unit_direction = unit_vector(r.direction());
  double a = 0.5 * (unit_direction.y() + 1.0);
  color c = (1.0 - a) * white + a * sky_blue;

  return c;
}

int
main()
{
  /* === CONFIG === */

  /* window */
  double aspect_ratio = 16.0 / 9.0;

  uint16_t window_width = 1280;
  uint16_t window_height = window_width / aspect_ratio;

  /* image */
  uint16_t image_width = 256;
  uint16_t image_height = image_width / aspect_ratio;
  uint8_t image_channels = 4;

  /* camera */
  double focal_length = 1.0;
  point3 camera_center = point3(0.0, 0.0, 0.0);

  /* viewport */
  double viewport_height = 2.0;
  double viewport_width =
    viewport_height * ((double)image_width / image_height);

  vec3 viewport_u = vec3(viewport_width, 0.0, 0.0);
  vec3 viewport_v = vec3(0.0, -viewport_height, 0.0);

  vec3 pixel_delta_u = viewport_u / image_width;
  vec3 pixel_delta_v = viewport_v / image_height;

  vec3 viewport_upper_left = camera_center - vec3(0.0, 0.0, focal_length) -
                             viewport_u / 2.0 - viewport_v / 2.0;
  vec3 pixel00_loc =
    viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

  /* ====== */

  RGFW_window* p_window =
    RGFW_createWindow("trace",
                      RGFW_RECT(0, 0, window_width, window_height),
                      RGFW_windowCenter | RGFW_windowNoResize);

  goto render;

  while (RGFW_window_shouldClose(p_window) == RGFW_FALSE) {
    while (RGFW_window_checkEvent(p_window)) {
      if (p_window->event.type == RGFW_quit) {
        break;
      }

      if (RGFW_isMousePressed(p_window, RGFW_mouseLeft)) {
      render:
        std::vector<uint8_t> pixels;
        for (size_t j = 0; j < image_height; j++) {
          std::print(std::clog, "\rScanlines remaining: {} ", image_height - j);
          std::flush(std::clog);
          for (size_t i = 0; i < image_width; i++) {
            vec3 pixel_center =
              pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
            vec3 ray_direction = pixel_center - camera_center;

            ray r(camera_center, ray_direction);

            color pixel_color = ray_color(std::move(r));
            emplace_color(pixels, std::move(pixel_color));

            /*
            emplace_color(pixels,
                          color(i / double(image_width - 1),
                                j / double(image_height - 1),
                                0.0));
            */
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
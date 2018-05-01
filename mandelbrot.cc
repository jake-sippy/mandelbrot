#include <SFML/Graphics.hpp>
#include <iostream>
#include <complex>
#include <unistd.h>

#define W 600
#define H 400

#define R_START -2
#define R_END    1

#define I_START  1
#define I_END   -1

//#define MAX_ITER 7

typedef struct RgbColor {
    unsigned char r;
    unsigned char g;
    unsigned char b;
} RgbColor;

typedef struct HsvColor {
    unsigned char h;
    unsigned char s;
    unsigned char v;
} HsvColor;

sf::Uint8 mandelbrot(std::complex<float>, int);
RgbColor HsvToRgb(HsvColor);

int main() {
  sf::RenderWindow window(sf::VideoMode(W, H), "SFML");
  sf::Texture texture;
  sf::Uint8* pixels = new sf::Uint8[W*H*4];
  texture.create(W, H);
  texture.setSmooth(true);
  sf::Sprite sprite(texture);
  int MAX_ITER = 1;
  while (window.isOpen()) {
    for (int y = 0; y < H; y++) {
      for (int x = 0; x < W; x++) {
        // calculate the index
        int i = 4 * (y * W + x);
        float real = R_START + (1.0 * x / W) * (R_END - R_START);
        float im   = I_START + (1.0 * y / H) * (I_END - I_START);

        std::complex<float> c(real, im);

        sf::Uint8 iters = mandelbrot(c, MAX_ITER);
        sf::Uint8 hue = 255 - 255 * ((1.0 * iters) / MAX_ITER);

        // colored output
        HsvColor hsv;
        hsv.h = hue;
        hsv.s = 255;
        hsv.v = 255;

        RgbColor color = HsvToRgb(hsv);
        pixels[i]   = color.r;
        pixels[i+1] = color.g;
        pixels[i+2] = color.b;
        pixels[i+3] = 255;

        // grayscale output
        //pixels[i]= hue;
        //pixels[i+1] = hue;
        //pixels[i+2] = hue;
        //pixels[i+3] = 255;
      }
    }

    window.clear();
    texture.update(pixels);

    window.draw(sprite);
    window.display();

    MAX_ITER++;
  }
  sleep(1);
  return 0;
}

sf::Uint8 mandelbrot(std::complex<float> c, int MAX_ITER) {
  std::complex<float> z(0, 0);
  sf::Uint8 iters = 0;
  while (iters < MAX_ITER && std::abs(z) < 2) {
    z = z*z + c;
    iters++;
  }
  return iters;
}

RgbColor HsvToRgb(HsvColor hsv) {
    RgbColor rgb;
    unsigned char region, remainder, p, q, t;
    if (hsv.s == 0) {
        rgb.r = hsv.v;
        rgb.g = hsv.v;
        rgb.b = hsv.v;
        return rgb;
    }

    region = hsv.h / 43;
    remainder = (hsv.h - (region * 43)) * 6;

    p = (hsv.v * (255 - hsv.s)) >> 8;
    q = (hsv.v * (255 - ((hsv.s * remainder) >> 8))) >> 8;
    t = (hsv.v * (255 - ((hsv.s * (255 - remainder)) >> 8))) >> 8;

    switch (region) {
        case 0:
            rgb.r = 0; rgb.g = 0; rgb.b = 0; break;
        case 1:
            rgb.r = q; rgb.g = hsv.v; rgb.b = p; break;
        case 2:
            rgb.r = p; rgb.g = hsv.v; rgb.b = t; break;
        case 3:
            rgb.r = p; rgb.g = q; rgb.b = hsv.v; break;
        case 4:
            rgb.r = t; rgb.g = p; rgb.b = hsv.v; break;
        default:
            rgb.r = hsv.v; rgb.g = p; rgb.b = q; break;
    }

    return rgb;
}


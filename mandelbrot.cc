#include <SFML/Graphics.hpp>
#include <iostream>
#include <complex>

#define W 300
#define H 200

#define R_START -2
#define R_END    1

#define I_START  1
#define I_END   -1

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

// computes the escape time of a given complex
// number if it is less than max_iters
sf::Uint8 mandelbrot(std::complex<float>, int);

// Convert a greyscale 0-255 to a color
RgbColor HueToRgb(sf::Uint8);

int main() {
  sf::RenderWindow window(sf::VideoMode(W, H), "SFML");
  sf::Texture texture;
  sf::Uint8* pixels = new sf::Uint8[W*H*4];
  texture.create(W, H);
  texture.setSmooth(true);
  sf::Sprite sprite(texture);

  int curr_iter = 1;
  while (window.isOpen()) {
    for (int y = 0; y < H; y++) {
      for (int x = 0; x < W; x++) {
        // calculate the index
        int i = 4 * (y * W + x);

        // create the complex for this coord
        float real = R_START + (1.0 * x / W) * (R_END - R_START);
        float im   = I_START + (1.0 * y / H) * (I_END - I_START);
        std::complex<float> c(real, im);

        // find escape time of this complex
        sf::Uint8 iters = mandelbrot(c, curr_iter);
        sf::Uint8 hue = 255 - 255 * ((1.0 * iters) / curr_iter);

        // colorized output
        RgbColor rgb = HueToRgb(hue);
        pixels[i]   = rgb.r;
        pixels[i+1] = rgb.g;
        pixels[i+2] = rgb.b;
        pixels[i+3] = 255;

        // grayscale output
        //pixels[i]   = hue;
        //pixels[i+1] = hue;
        //pixels[i+2] = hue;
        //pixels[i+3] = 255;
      }
    }

    sf::Event event;
    while (window.pollEvent(event)) {
	if (event.type == sf::Event::Closed)
	    window.close();
    } 

    window.clear();
    texture.update(pixels);

    window.draw(sprite);
    window.display();

    curr_iter++;
  }
  return 0;
}

sf::Uint8 mandelbrot(std::complex<float> c, int max_iters) {
  std::complex<float> z(0, 0);
  sf::Uint8 iters = 0;
  while (iters < max_iters && std::abs(z) < 2) {
    z = z*z + c;
    iters++;
  }
  return iters;
}

RgbColor HueToRgb(sf::Uint8 hue) {
    RgbColor rgb;

    // saturation and value forced to be max
    int s = 255;
    int v = 255;
    unsigned char region, remainder, p, q, t;

    region = hue / 43;
    remainder = (hue - (region * 43)) * 6;

    p = (v * (255 - s)) >> 8;
    q = (v * (255 - ((s * remainder) >> 8))) >> 8;
    t = (v * (255 - ((s * (255 - remainder)) >> 8))) >> 8;

    switch (region) {
        case 0:     // we've made the lowest shade be black
            rgb.r = 0; rgb.g = 0; rgb.b = 0; break;
        case 1:
            rgb.r = q; rgb.g = v; rgb.b = p; break;
        case 2:
            rgb.r = p; rgb.g = v; rgb.b = t; break;
        case 3:
            rgb.r = p; rgb.g = q; rgb.b = v; break;
        case 4:
            rgb.r = t; rgb.g = p; rgb.b = v; break;
        default:
            rgb.r = v; rgb.g = p; rgb.b = q; break;
    }

    return rgb;
}


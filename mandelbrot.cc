#include <SFML/Graphics.hpp>
#include <iostream>
#include <complex>

#define W 720
#define H 480

#define R_START -2
#define R_END    1

#define I_START  1
#define I_END   -1

#define ITER_STEP 1

typedef struct RgbColor {
    unsigned char r;
    unsigned char g;
    unsigned char b;
} RgbColor;

// computes the escape time of a given complex
// number if it is less than max_iters
static sf::Uint8 mandelbrot(std::complex<double>, int);

// method to continue the mandelbrot iterations
// using arrays that save the current z and iteration
static void continueMandel(std::complex<double>,
    std::complex<double>*, int*, int);

// make an array of complexes
static std::complex<double>* makeComplexes();

// computes the index (given a coordinate point)
// for the arrays used
static int getIndex(int, int);

// Convert a greyscale 0-255 to a color
static RgbColor HueToRgb(sf::Uint8);


int main() {
  sf::RenderWindow window(sf::VideoMode(W, H), "SFML");
  sf::Texture texture;

  auto pixels = new sf::Uint8[W*H*4];
  auto iterations = new int[W*H*4];
  auto zs = new std::complex<double>[W*H*4];
  auto complexes = makeComplexes();

  texture.create(W, H);
  texture.setSmooth(true);
  sf::Sprite sprite(texture);

  int max_iters = 1;
  while (window.isOpen()) {
    int i;                    // index

    std::complex<double> c;   // the complex for each coord
    std::complex<double> z;   // the current z for each c
    int curr;                 // iterations current c has done

    for (int y = 0; y < H; y++) {
      for (int x = 0; x < W; x++) {
        // calculate the index
        i = getIndex(x, y);

        c = complexes[i];
        z = zs[i];
        curr = iterations[i];

        // find escape time of this complex
        continueMandel(c, &z, &curr, max_iters);

        sf::Uint8 hue = 255 - 255 * ((1.0 * curr) / max_iters);

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
	if (event.type == sf::Event::Closed) {
            delete[] complexes;
            delete[] iterations;
            delete[] zs;
            delete[] pixels;
            window.close();
            return 0;
        }
    }

    window.clear();
    texture.update(pixels);

    window.draw(sprite);
    window.display();

    max_iters += ITER_STEP;
  }
  delete[] complexes;
  delete[] iterations;
  delete[] zs;
  delete[] pixels;
  return 0;
}

static sf::Uint8 mandelbrot(std::complex<double> c, int max_iters) {
  std::complex<double> z(0, 0);
  sf::Uint8 iters = 0;
  while (iters < max_iters && std::abs(z) < 2) {
    z = z*z + c;
    iters++;
  }
  return iters;
}

static void continueMandel(std::complex<double> c,
    std::complex<double>* z, int* iters, int max_iters) {

  while (*iters < max_iters && std::abs(*z) < 2) {
    *z = (*z) * (*z) + c;
    (*iters)++;
  }
  return;
}

static std::complex<double>* makeComplexes() {
  auto result = new std::complex<double>[W*H*4];
  int i;
  double real, im;

  for (int y = 0; y < H; y++) {
    for (int x = 0; x < W; x++) {
      i = getIndex(x, y);
      real = R_START + ((double) x / W) * (R_END - R_START);
      im = I_START + ((double) y / H) * (I_END - I_START);
      std::complex<double> c(real, im);
      result[i] = c;
    }
  }
  return result;
}

static int getIndex(const int x, const int y) {
  return 4 * (y * W + x);
}

static RgbColor HueToRgb(sf::Uint8 hue) {
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


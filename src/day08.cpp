// Copyright (C) 2019 David Holmes <dholmes@dholmes.us>. All rights reserved.

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>

using namespace std;

const char * get_filename(int argc, char ** argv)
{
    if (argc < 2) {
        cerr << "USAGE: " << argv[0] << " filename.txt";
        exit(-1);
    }
    return argv[1];
}

enum pixel_values : char
{
    black = '0',
    white = '1',
    transparent = '2',
};
using Pixel = char;

struct dimensions
{
    int32_t width;
    int32_t height;
    int32_t pixel_count() const { return width * height; }
};

struct coordinates
{
    int32_t x;
    int32_t y;
};

struct image_layer
{
    dimensions dims;
    vector<Pixel> pixels;
};

struct image
{
    dimensions dims;
    vector<image_layer> layers;
};

// May fail and set error state on istream.
vector<Pixel> read_pixels(istream & in, const int32_t count)
{
    vector<Pixel> out;
    out.resize(count);
    in.read(out.data(), count);
    return out;
}

image read_image_from_file(const char * filename, const dimensions dims)
{
    int32_t pixels_per_layer = dims.pixel_count();
    ifstream input_file(filename);
    image out{dims, {}};

    vector<Pixel> pixels = read_pixels(input_file, pixels_per_layer);
    while (input_file) {
        out.layers.emplace_back();
        out.layers.back().dims = dims;
        out.layers.back().pixels = move(pixels);
        pixels = read_pixels(input_file, pixels_per_layer);
    }
    return out;
}

int32_t count_digit(const image_layer & layer, Pixel digit)
{
    return count(begin(layer.pixels), end(layer.pixels), digit);
}

int32_t count_zeroes(const image_layer & layer)
{
    return count_digit(layer, '0');
}

auto find_layer_fewest_zeroes(const image & im)
{
    vector<int32_t> zero_counts;
    zero_counts.resize(im.layers.size());
    transform(begin(im.layers), end(im.layers), begin(zero_counts),
              count_zeroes);
    return min_element(begin(zero_counts), end(zero_counts)) -
           begin(zero_counts);
}

image_layer transpose_layers(const image & im)
{
    image_layer out = im.layers.back();
    for (auto layer_it = im.layers.rbegin(); layer_it < im.layers.rend();
         layer_it++) {
        auto layer = *layer_it;
        for (auto i = 0UL; i < out.pixels.size(); i++) {
            Pixel top = layer.pixels[i];
            if (top != transparent) {
                out.pixels[i] = top;
            }
        }
    }
    return out;
}

void print_layer(ostream & out, const image_layer & layer)
{
    for (int row = 0; row < layer.dims.height; row++) {
        for (int col = 0; col < layer.dims.width; col++) {
            Pixel pixel = layer.pixels[row * layer.dims.width + col];
            out << ((pixel == white) ? "█" : " ");
        }
        out << '\n';
    }
}

int main(int argc, char ** argv)
{
    const image im = read_image_from_file(get_filename(argc, argv), {25, 6});
    const auto layer_index = find_layer_fewest_zeroes(im);
    const auto & layer = im.layers[layer_index];
    const auto ones = count_digit(layer, '1');
    const auto twos = count_digit(layer, '2');
    cout << "Layer " << layer_index << " has " << ones << " ones and " << twos
         << " twos (answer: " << (ones * twos) << ")\n";

    const auto output = transpose_layers(im);
    print_layer(cout, output);

    return 0;
}

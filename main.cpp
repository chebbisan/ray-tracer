#include <iostream>

int main() {
    int image_width = 256;
    int image_height = 256;

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int i = 0; i < image_height; ++i) {
        for (int j = 0; j < image_width; ++j) {
            auto r = (double)(j) / (image_width - 1);
            auto g = (double)(i) / (image_height - 1);
            auto b = 0.;

            int ir = int(r * 255.999);
            int ig = int(g * 255.999);
            int ib = int(b * 255.999);

            std::cout << ir << ' ' << ib << ' ' << ig << '\n';
        }
    }
    return 0;
}
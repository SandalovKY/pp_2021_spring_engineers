// Copiright 2021 Kustova Anastasiya
#include "filter_gaussa_block.h"
#include <iostream>
#include <math.h>
#include <vector>

int ** gaussianFilter(int ** img, int width, int height, int radius, float sigma) {
	int ** resultImage = new int*[height];
	for (int i = 0; i < height; i++) {
		resultImage[i] = new int[width];
	}
	int size = 2 * radius + 1;
	std::vector<float> kernel(size * size);
	kernel = createGaussianKernel(radius, sigma);
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			int color = calculateNewPixelColor(img, width, height, i, j, radius, kernel);
			resultImage[i][j] = color;
		}
	}
	return resultImage;
}
std::vector<float> createGaussianKernel(int radius, float sigma) {
	int size = 2 * radius + 1;
	std::vector<float> kernel(size * size);
	float norm = 0;
	for (int i = -radius; i <= radius; i++) {
		for (int j = -radius; j <= radius; j++) {
			kernel[(i + radius)* size + j + radius] = (float)(exp(-(i * i + j * j) / (2 * sigma * sigma)));
			norm = norm + kernel[(i + radius) * size + j + radius];
		}
	}
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			kernel[i * size + j] = kernel[i * size + j] / norm;
		}
	}
	return kernel;
}

int calculateNewPixelColor(int ** img, int width, int height, int x, int y, int radius, std::vector<float> kernel) {
	int size = radius * 2 + 1;
	double sumColor = 0;
	for (int l = -radius; l <= radius; l++) {
		for (int k = -radius; k <= radius; k++) {
			int idX = Clamp(x + k, 0, width - 1);
			int idY = Clamp(y + l, 0, height - 1);
			int neighborColor = img[idX][idY];
			sumColor += neighborColor * kernel[(k + radius) * size + l + radius];
		}
	}
	return Clamp((int)sumColor, 0, 255);
}
int Clamp(int value, int min, int max) {
	if (value < min) {
		return min;
	}
	if (value > max) {
		return max;
	}
	return value;
}


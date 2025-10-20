#include <algorithm>
#include <fstream>
#include <iostream>
void Project3DTriangleInto2D(float triangle[], float output[], float width,
                             float height) {
  for (int i = 0; i < 3; i++) {
    int index = i * 3;
    output[index] = (triangle[index] + 1) * (width / 2);
    output[index + 1] = (1.0f - triangle[index + 1]) * (height / 2);
    output[index + 2] = triangle[index + 2];
  }
}



void rastarization(float projection[]) {
  // Z axis not needed

  float x0 = projection[0];
  float y0 = projection[1];
  float x1 = projection[3];
  float y1 = projection[4];
  float x2 = projection[6];
  float y2 = projection[7];
}

bool isInsideTriangle(float px, float py, float x0, float y0, float x1,
                      float y1, float x2, float y2) {
  float denom = ((y1 - y2) * (x0 - x2) + (x2 - x1) * (y0 - y2));
  if (denom == 0)
    return false;

  float a = ((y1 - y2) * (px - x2) + (x2 - x1) * (py - y2)) / denom;
  float b = ((y2 - y0) * (px - x2) + (x0 - x2) * (py - y2)) / denom;
  float c = 1.0f - a - b;

  return (a >= 0 && b >= 0 && c >= 0);
}

void rasterize(float projection[], int width, int height,
               unsigned char *imageBuffer) {
  // Extract triangle vertices from projection
  float x0 = projection[0], y0 = projection[1];
  float x1 = projection[3], y1 = projection[4];
  float x2 = projection[6], y2 = projection[7];

  // Find bounding box
  int minX = (int)std::max(0.0f, std::min({x0, x1, x2}));
  int maxX = (int)std::min((float)width, std::max({x0, x1, x2}));
  int minY = (int)std::max(0.0f, std::min({y0, y1, y2}));
  int maxY = (int)std::min((float)height, std::max({y0, y1, y2}));

  // Loop through bounding box
  for (int y = minY; y < maxY; y++) {
    for (int x = minX; x < maxX; x++) {
      if (isInsideTriangle(x, y, x0, y0, x1, y1, x2, y2)) {
        // Pixel is inside triangle, write color
        int pixelIndex = (y * width + x) * 3;
        imageBuffer[pixelIndex] = 29;     // R
        imageBuffer[pixelIndex + 1] = 25; // G
        imageBuffer[pixelIndex + 2] = 23; // B
      }
    }
  }
}

void rasterize(float projection[]) {}

int main() {

  static float ImageWidth = 520;
  static float ImageHeight = 520;
  // I have a triangle
  float triangle[] = {
      -0.5f, -0.5f, 0.0f, //
      0.5f,  -0.5f, 0.0f, //
      0.0f,  0.5f,  0.0f  //
  };
  unsigned char *imageBuffer =
      new unsigned char[(int)ImageWidth * (int)ImageHeight * 3];
  float projection[9];
  Project3DTriangleInto2D(triangle, projection, ImageWidth, ImageHeight);
  rasterize(projection, (int)ImageWidth, (int)ImageHeight, imageBuffer);

  // Need a Image Buffer, A z buffer , the triangle
  // Put the triangle into 2D projection and rasteriza and then the final output
  // Let's Just write How a Simple PPM file will be
  std::ofstream PPMfile;
  PPMfile.open("test.ppm", std::ios::binary);
  if (!PPMfile.is_open()) {
    std::cout << "Failed to Open PPM file";
  }
  PPMfile << "P6\n" << ImageWidth << " " << ImageHeight << "\n255\n";
  PPMfile.write((char *)imageBuffer, (int)ImageWidth * (int)ImageHeight * 3);
  // Now here need to write the actual pixel data
  PPMfile.close();

  delete[] imageBuffer;
}

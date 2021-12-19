#ifndef FINAL_CPP_PERLIN_NOISE_H
#define FINAL_CPP_PERLIN_NOISE_H

class Perlin {
public:
    float persistence;
    int Number_Of_Octaves;

    Perlin() {
        persistence = 0.1;
        Number_Of_Octaves = 2;
    }

    double Noise(int x, int y);

    double SmoothedNoise(int x, int y);

    double Cosine_Interpolate(double a, double b, double x);

    double Linear_Interpolate(double a, double b, double x);

    double InterpolatedNoise(float x, float y);

    double PerlinNoise(float x, float y);
};

#endif

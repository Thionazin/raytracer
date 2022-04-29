# Raytracer
A simple image renderer that uses the ray tracing algorithm to draw each pixel.

# Requirements
GLM with a proper environment variable set up is required.

An OpenMP package must be installed.

CMake and Make are needed to build.

# Usage
To avoid creating a mess, create a new subdirectory. Enter it and run:
```
cmake ..
```

To build in release mode (recommended):
```
ccmake ..
```
Then change CMAKE_BUILD_TYPE to release.

Run generated makefile with
```
make <optional flags>
```

Target is an executable called A6
```
./A6 <scene no> <width and height> <output file name>
```

# Credits
The mesh loader is an obj loader from http://github.com/syoyo/tinyobjloader, and the image writer is from http://github.com/nothings/stb.

Algorithm for generating camera rays based off of https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-generating-camera-rays.

Credits to Dr. Shinjiro Sueda from Texas A&M University for the matrix stack and image classes.

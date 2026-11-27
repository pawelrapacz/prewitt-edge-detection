# Prewitt edge detection
A university project for implementing the Prewitt edge detection algorithm in C and Assembly.

<img width="2762" height="1502" alt="image" src="https://github.com/user-attachments/assets/ba51ae1a-0879-4ca7-aead-cc579eec89c2" />

## About
This project supports both x64 Windows and Linux systems and provides three variants of the Prewitt edge detection algorithm:

- [ASM x64 Scalar implementation](src/asmimpl/linux/asmDetectEdgesWorker.s)
- [ASM x64 SIMD implementation](src/asmimpl/linux/asmDetectEdgesSIMDWorker.s)
- [C implementation](src/cimpl/cimpl.c)

The main objective of having three different implementations is to compare their performance.
Because of that this project also includes a benchmarking utility that measures the execution time of each implementation across different input sizes and thread counts.
The results are automatically saved to a bench.csv file for further analysis.

## Building
Qt6 is required to build the application.

First configure the build system using CMake. On Windows you may need to also specify the path to Qt installation  
i.e. `-DCMAKE_PREFIX_PATH=C:\Qt\6.12.0\msvc2022_64`.

```
cmake --preset default-no-tests
```

Then, build the project using one of the available configurations: `debug`, `release` or `minsizerel`.

```
cmake --build --preset release-no-tests
```

Once the build is complete, run the application:

```
./build/Release/prewitt-edge-detection
```

### Testing

The project also includes a test suite. Running the tests requires Python 3 interpreter (with `opencv-python` installed), which is used to generate reference images.

First, configure the project with testing enabled:

```
cmake --preset default
```

Build all targets, including the tests:

```
cmake --build --preset all
```

Finally, run the test suite:

```
ctest --preset all
```

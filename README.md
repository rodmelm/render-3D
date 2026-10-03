# 3D Ray Tracer Engine
A 3D rendering engine built in C++, designed to generate photorealistic images using ray tracing techniques. Developed as an advanced university project, it focuses on memory layout optimization and multi-core parallelization.

# About The Project
This project implements a complete ray tracing engine without relying on external graphics APIs (like OpenGL or Vulkan). It applies advanced software engineering concepts, including Object-Oriented Design for scene objects, strict testing methodologies (GoogleTest), and performance optimization through memory access patterns (AoS vs. SoA) and multithreading using Threading Building Blocks (TBB).

# Built With:

- Language: Modern C++ (C++23 Standard)
- Parallelism: Intel TBB (Threading Building Blocks)
- Testing: GoogleTest (GTest)
- Build System: CMake & Ninja
- Static Analysis & Quality: Clang-Tidy, Gcovr (Code Coverage)

# Key Architecture & Features:

Custom Ray Tracing Physics: Implements accurate ray-geometry intersection algorithms for spheres and cylinders, supporting advanced optical material properties like matte (diffuse), metal (reflective with fuzziness), and refractive (glass-like, implementing Snell's law and total internal reflection).

Parallel Rendering: The rendering loop is parallelized using Intel TBB, significantly accelerating image generation by distributing pixel calculations across all available CPU cores.

Thread-Safe Random Number Generation: Implements a highly optimized, thread-local random number generation system (tbb::enumerable_thread_specific) to ensure thread safety without lock contention during the highly concurrent path-tracing process.

Memory Layout Optimization (AoS vs. SoA): Explores the performance impact of data structures by implementing both Array of Structures (AoS) and Structure of Arrays (SoA) memory layouts for the output image buffer, optimizing for cache locality and potential SIMD vectorization.

Custom Configuration & Scene Parsers: Features robust, custom-built parsers to load render settings (resolution, FOV, samples, depth) and complex scene definitions (materials, object placement) directly from text files.
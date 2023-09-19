A minimal project to build a simulation with [PhysX 5.2](https://github.com/NVIDIA-Omniverse/PhysX).

This is a CMake project that specifies PhysX as a dependency (cf [CMakeLists.txt](CMakeLists.txt)).

Note that the upstream PhysX dependency is not the official PhysX 5.2
distribution but a [fork](https://github.com/olitheolix/PhysX/tree/clang16).
However, the only differences are trivial cosmetic changes to make PhysX
compile with CLang 16.

# Usage

    cmake -S . -B build/
    cmake --build build/ -j8
    build/helloworld

## Docker
This repository also contains a reference [Dockerfile](Dockerfile) to build and
run the projects in isolation from the host:

    docker build -t physx-helloworld --progress=plain .
    docker run --rm physx-helloworld

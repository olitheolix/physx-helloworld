# ----------------------------------------------------------------------
# Build the Hello World application for Ubuntu 22.04.
#
# Usage:
#   $ docker build -t physx-helloworld . --progress=plain
#
# ----------------------------------------------------------------------
FROM ubuntu:22.04

# ----------------------------------------------------------------------
# Install Dependencies.
# ----------------------------------------------------------------------
RUN apt update && apt install -y \
    build-essential \
    cmake \
    curl \
    git \
    gnupg \
    lsb-release \
    software-properties-common \
    wget \
    wget

# ----------------------------------------------------------------------
# Install CLang 16.
# ----------------------------------------------------------------------
WORKDIR /tmp
RUN wget https://apt.llvm.org/llvm.sh \
    && chmod +x llvm.sh \
    && ./llvm.sh 16 all

# Make CLang 16 the default.
RUN update-alternatives --install /usr/bin/clang        clang        /usr/bin/clang-16        100 \
 && update-alternatives --install /usr/bin/clangd       clangd       /usr/bin/clangd-16       100 \
 && update-alternatives --install /usr/bin/clang++      clang++      /usr/bin/clang++-16      100 \
 && update-alternatives --install /usr/bin/clang-format clang-format /usr/bin/clang-format-16 100

# ----------------------------------------------------------------------
# Add Application Code.
# ----------------------------------------------------------------------
WORKDIR src
COPY CMakeLists.txt helloworld.cpp ./

# ----------------------------------------------------------------------
# Compile And Run Hello World Example.
# ----------------------------------------------------------------------
RUN cmake -S . -B build/ && cmake --build build/ -j8
RUN build/helloworld

CMD ["build/helloworld"]

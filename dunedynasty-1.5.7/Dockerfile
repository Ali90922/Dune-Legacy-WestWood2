# Use an official Ubuntu as a parent image, enforcing x86_64 architecture
FROM --platform=linux/amd64 ubuntu:latest

# Install cross-compilation tools and dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    libsdl2-dev \
    libsdl2-image-dev \
    libsdl2-mixer-dev \
    libsdl2-ttf-dev \
    cmake \
    git \
    fluidsynth \
    pkg-config \
    liballegro5-dev \
    gcc-multilib \
    g++-multilib \
    gcc-x86-64-linux-gnu \
    g++-x86-64-linux-gnu \
    libmad0-dev \
    libgl1-mesa-dev \
    libglu1-mesa-dev \
    && rm -rf /var/lib/apt/lists/*

# Set the working directory
WORKDIR /dunelegacy

# Copy the current directory contents into the container at /dunelegacy
COPY . /dunelegacy

# Copy the toolchain file
COPY x86_64-toolchain.cmake /dunelegacy

# Enforce cross-compilation for x86_64 using the toolchain file
RUN cmake . -DCMAKE_TOOLCHAIN_FILE=/dunelegacy/x86_64-toolchain.cmake -DCMAKE_C_COMPILER=x86_64-linux-gnu-gcc -DCMAKE_CXX_COMPILER=x86_64-linux-gnu-g++ -DOPENGL_INCLUDE_DIR=/usr/include -DOPENGL_gl_LIBRARY=/usr/lib/x86_64-linux-gnu/libGL.so && make VERBOSE=1

# Output the built executable path
CMD ["echo", "Build completed. Check the output in /dunelegacy/build/dist/"]

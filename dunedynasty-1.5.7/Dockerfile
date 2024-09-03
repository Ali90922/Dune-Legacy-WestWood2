# Use an official Ubuntu as a parent image
FROM ubuntu:latest

# Install dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    libsdl2-dev \
    libsdl2-image-dev \
    libsdl2-mixer-dev \
    libsdl2-ttf-dev \
    cmake \
    git \
    fluidsynth \
    && rm -rf /var/lib/apt/lists/*

# Set the working directory
WORKDIR /dunelegacy

# Copy the current directory contents into the container at /dunelegacy
COPY . /dunelegacy

# Build the game
RUN cmake . && make

# Run the game
CMD ["./dunelegacy"

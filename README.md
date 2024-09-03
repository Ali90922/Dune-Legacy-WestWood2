# Dune Legacy

Welcome to Dune Dynasty ReMastered, a modernized clone of the classic Dune 2 game.


### Using Markdown
![Dune Dynasty Banner](https://raw.githubusercontent.com/gameflorist/dunedynasty/master/docs/banner.jpg)




# Dune II: The Building of a Dynasty - Enhanced Edition

This project is a modern enhancement of the open-source version of **Dune II: The Building of a Dynasty**, a 1992 real-time strategy game originally developed by Westwood Studios and published by Virgin Games. As a sequel to the original Dune adventure strategy game, Dune II set the foundation for the RTS genre, introducing key gameplay mechanics that are still used today.

## Overview

The enhanced edition of Dune II focuses on making the game more accessible and enjoyable by:

- **New Maps**: Added new maps to enrich gameplay and provide fresh challenges.
- **Refactored Code**: Cleaned and updated legacy code to improve performance and maintainability.
- **Executable Creation**: Developed an executable that seamlessly integrates old game files with the modern version of Dune II, ensuring compatibility and ease of use.
- **Docker Integration**: Built a Docker image that bundles all necessary dependencies and libraries, allowing anyone to run the game without the complexity of manual setup or finding old .pak game files.

## Importance of the Docker Image and Executable Codebase

Unlike other open-source projects like DuneOpen and DuneLegacy, which require the original 1992 game files and various outdated libraries to function, this project simplifies the entire process:

- **Unified Setup**: The Docker image includes all necessary dependencies and libraries, eliminating the need to manually hunt down old game files or configure legacy software environments. This unified setup ensures that the game runs seamlessly on modern systems without compatibility issues.
- **Ease of Use**: With the Docker image, anyone can get started with the game from scratch without the technical hurdles typically associated with setting up legacy games. This is particularly valuable for players and developers who want to experience or modify the game without dealing with complex setups.
- **Cross-Platform Consistency**: By using Docker, the game can be run consistently across different platforms, ensuring that everyone has the same experience regardless of their operating system.
- **Enhanced Accessibility**: The executable codebase is updated to work directly with the modern enhancements, making it far easier to play and develop further modifications compared to other projects that still rely on outdated files and configurations.

## Getting Started

To get started with this enhanced version of Dune II, follow these steps:

1. **Clone the Repository**:

   ```bash
   git clone https://github.com/aliiii907/Dune-Legacy-WestWood2.git
   cd Dune-Legacy-WestWood2
   ```

2. **Run the Docker Image**:

   Ensure Docker is installed on your system. Then, build and run the Docker image:

   ```bash
   docker build -t dune-legacy-builder .
   docker run --rm -it dune-legacy-builder /bin/bash
   ```

3. **Launch the Game**:

   Inside the Docker container, navigate to the build directory and run the executable:

   ```bash
   cd /dunelegacy/build/dist
   ./dunedynasty
   ```

## Why This Project?

This project aims to simplify the process of setting up and running Dune II by combining the old game files with the modern version and automating the setup through Docker. Previously, players needed to manually locate and configure legacy files, making it challenging to get the game running. Now, with everything bundled in a Docker image, getting started is quick and easy.

## Contributing

Contributions are welcome! Whether you want to add new maps, improve the codebase, or enhance the Docker setup, feel free to fork the repository and submit a pull request.

## License

This project is open-source and available under the [MIT License](LICENSE).


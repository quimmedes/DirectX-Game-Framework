# DirectX Game Framework

A minimal DirectX 11-based game framework written in C++. Provides core subsystems for window and device management, rendering, input handling, camera control, model/textures loading, and basic lighting with HLSL shaders.

## Table of Contents

1. [Project Overview](#project-overview)  
2. [Features](#features)  
3. [Prerequisites](#prerequisites)  
4. [Building & Running](#building--running)  
5. [Project Structure](#project-structure)  
6. [Subsystems & Classes](#subsystems--classes)  
7. [Assets & Shaders](#assets--shaders)  
8. [Usage Example](#usage-example)  
9. [Contributing](#contributing)  
10. [License](#license)

## Project Overview

This framework encapsulates common boilerplate needed to start a DirectX 11-powered application on Windows:

- Creates and manages the application window and message loop  
- Initializes the Direct3D 11 device, swap chain, render target, and depth/stencil buffers  
- Loads and renders static meshes and textures  
- Supports camera movement and basic keyboard input  
- Implements simple directional lighting and texture shaders  

It is intended as a foundation for learning, experimentation, or rapid prototyping of small DirectX demos and games.

## Features

- **Window & System Management** (`SystemClass`)  
- **Direct3D Initialization** (`D3DClass`)  
- **Model Loading & Rendering** (`ModelClass`)  
- **Texture Loading** (`TextureClass`)  
- **Camera Control** (`CameraClass`)  
- **Keyboard Input Handling** (`InputClass`)  
- **Basic Lighting** (`LightClass`)  
- **HLSL Shaders** (`Engine/light.vs/.ps`, `Engine/texture.vs/.ps`)  
- **Configurable via C++ headers and HLSL files**

## Prerequisites

- Windows 10 or later (x64)  
- Visual Studio 2019 or 2022 (Desktop Development with C++ workload)  
- Windows 10 SDK (DirectX headers included) or legacy DirectX SDK (June 2010)  
- Git (to clone the repository)

## Building & Running

1. Clone the repository:  
   ```sh
   git clone https://github.com/quimmedes/DirectX-Game-Framework.git
   cd DirectX-Game-Framework
   ```
2. Open `DirectX.sln` in Visual Studio.  
3. Select **x64** and **Debug** (or **Release**) configuration.  
4. Build the solution (Ctrl+Shift+B).  
5. Run the `DirectX` project (F5).

A window will open rendering a textured 3D object with simple lighting.

## Project Structure

```
/ (root)
├─ applicationclass.{h,cpp}   # Entry point, window & message loop
├─ systemclass.{h,cpp}        # Wraps Win32 initialization & shutdown
├─ d3dclass.{h,cpp}           # Direct3D 11 device, context, swap chain
├─ cameraclass.{h,cpp}        # View/projection matrix setup & movement
├─ inputclass.{h,cpp}         # Keyboard state tracking
├─ modelclass.{h,cpp}         # Mesh data loading & drawing
├─ textureclass.{h,cpp}       # Texture loading
├─ lightclass.{h,cpp}         # Lighting parameters
├─ lightshaderclass.{h,cpp}   # HLSL shader compilation & rendering
├─ main.cpp                   # Application entry
├─ Engine/
│  ├─ *.vs, *.ps               # HLSL vertex/pixel shaders
│  └─ data/                    # Texture and model assets
└─ x64/Debug/                  # Build artifacts
```

## Subsystems & Classes

- **ApplicationClass**  
  Orchestrates initialization, the main loop, rendering, and cleanup.

- **SystemClass**  
  Manages Win32 window creation, resizing, and message pump.

- **D3DClass**  
  Wraps Direct3D 11 initialization: device, swap chain, render target, depth buffer, and viewport.

- **ModelClass**  
  Loads `.txt` mesh data (vertex positions, UVs, normals) into vertex/index buffers.

- **TextureClass**  
  Loads texture assets (`.tga`) into shader resource views.

- **CameraClass**  
  Generates view and projection matrices; offers position and rotation controls.

- **InputClass**  
  Tracks keyboard state for real-time input queries.

- **LightClass & LightShaderClass**  
  Encapsulate directional/point light properties and compile HLSL shaders for basic per-pixel lighting.

## Assets & Shaders

Located under `Engine/`:

- **Shaders** (`*.vs`, `*.ps`):  
  - `texture.vs/.ps`: Basic textured rendering  
  - `light.vs/.ps`: Textured rendering with lighting  

- **Data**:  
  - `.tga` textures  
  - `.txt` mesh definitions (cube, sphere, brick, stone)

## Usage Example

After starting the application:

- Use **W/A/S/D** or arrow keys to move the camera.  
- Observe textured 3D geometry lit by a directional light source.  

Extend or replace HLSL shaders in `Engine/` to experiment with custom effects.

## Contributing

1. Fork the repository.  
2. Create a feature branch.  
3. Commit your changes with clear messages.  
4. Open a pull request.

## License

This project is released under the MIT License. See [LICENSE](LICENSE) for details.

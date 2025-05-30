# OpenGL 3D Interactive Scene

This project is a 3D graphics assignment developed using **OpenGL** and **FreeGLUT** in **Code::Blocks (Windows)**. It features a fully interactive 3D environment with camera control, object transformations (translation, rotation, scaling), and auto-rotation for a **cube**. Users can select between a cube, pyramid, or sphere and manipulate them in real time using keyboard controls.

## Features

- Interactive camera movement (WASDQE)
- Object selection (cube, pyramid, sphere)
- Translation in 3D space
- Rotation along X and Y axes
- Object scaling
- Auto-rotation toggle for cube
- Lighting and shading
- Textures and color rendering
- Clean UI with keyboard-only interaction
- Exit the application with `ESC`

## Keyboard Controls

| Key              | Function                                     |
|------------------|----------------------------------------------|
| **ESC**          | Exit the application                         |
| **W / S**        | Move camera forward / backward               |
| **A / D**        | Move camera left / right                     |
| **Q / E**        | Move camera up / down                        |
| **1**            | Select Cube                                  |
| **2**            | Select Pyramid                               |
| **3**            | Select Sphere                                |
| **↑ / ↓**        | Move object forward/backward (Z-axis)        |
| **← / →**        | Move object left/right (X-axis)              |
| **Z / X**        | Move object up/down (Y-axis)                 |
| **R / T**        | Rotate object clockwise/counter-clockwise (Y-axis) |
| **F / G**        | Tilt object upward/downward (X-axis)         |
| **+ / -**        | Scale object up/down                         |
| **G**            | Toggle auto-rotation for cube                |

## Requirements

- OpenGL
- FreeGLUT
- C++ Compiler (tested on Code::Blocks for Windows)

## How to Run

1. Open the project in **Code::Blocks**
2. Ensure OpenGL and FreeGLUT are correctly linked in project build settings
3. Build and run the project


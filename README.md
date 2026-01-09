# Pekan Engine

Pekan is a lightweight 2D game engine written in modern C++20 focused on modular architecture, ECS-driven design, and high-performance rendering.
Its subsystem-based structure allows features to be enabled, replaced, or extended without modifying the core runtime,
making the engine suitable for both experimentation and larger structured projects.

**Key Features:**
- **Modular Subsystem Architecture**: Compile-time configurable subsystems with explicit dependencies
- **Entity-Component System**: EnTT-based ECS with transform hierarchies and data-oriented system design
- **Event System**: Type-safe event dispatch with layering and priority-based handling
- **Scene & Layer Management**: Scene abstraction with a layer stack for composing logic, rendering, and UI
- **2D Batch Renderer**: Custom OpenGL-based renderer with automatic batching to reduce draw calls
- **Developer Tools**: Built-in camera controllers, real-time ImGui integration, and an FPS widget

## How to build

**Requirements:**
- CMake 3.6+
- OpenGL 3.3+
- C++20 compiler

**On Windows (with Visual Studio):**

Run this batch script.
```powershell
gen.bat
```
It will configure Pekan and generate a Visual Studio solution using CMake

<details>
<summary>Alternative to gen.bat</summary>
gen.bat automatically detects the latest Visual Studio installed, and uses that, together with the CMake that ships with Visual Studio.
Alternatively, you can use any CMake (v3.6+) and any Visual Studio (2019+) to generate a Visual Studio solution.
For example, if you're using Visual Studio 2022 you can run this command:

```powershell
cmake -G "Visual Studio 2022" -A x64 -B build
```
</details>

Open `build/Pekan.sln` and build the solution.

After building, you can run any of the demo applications (`Demo00` through `Demo10`).

## Technical Highlights

**Game Framework & Architecture:**
- Application lifecycle management with init/update/render phases
- Layer stack for composing any number of independent layers (scenes, UI, debug overlays) and structuring application stages (menus, levels) with transitions between them.
- Scene abstraction built on EnTT registries
- Transform hierarchies with parent–child relationships

**Entity-Component System:**
- EnTT integration for high-performance component storage and iteration
- Specialized systems: TransformSystem2D, RenderSystem2D, SpriteSystem, CircleGeometrySystem, etc.
- Component-based rendering with automatic batch submission

**Rendering:**
- Custom batch renderer supporting sprites and shapes (circles, rectangles, triangles, polygons, lines)
- Camera system with flexible projection and transformation matrices

**Shaders:**
- Full support for custom shaders with programmable vertex and fragment stages
- Post-processing support for screen-space effects (blur, distortion, color grading)
- Easy-to-use shader management system for loading and binding custom GLSL programs

**Development Environment:**
- CMake-based build system with modular target configuration
- 11 demo applications showcasing different engine capabilities
- Integrated third-party libraries: OpenGL (GLAD), GLM, ImGui, EnTT, STB

## Demo Applications

**Gleam House**

The biggest demo built with Pekan so far is **Gleam House**, a 2D top-down labyrinth game
that showcases Pekan's capability for building complex maps and implementing dynamic lighting.

<img width="929" height="725" alt="Demo_of_Gleam_House_1" src="https://github.com/user-attachments/assets/75636145-9165-4750-aed5-46149dcf1855" />
<img width="1357" height="898" alt="Demo_of_Gleam_House_2" src="https://github.com/user-attachments/assets/775179c3-a6ec-4a6c-808f-dd5e7a87d22c" />
<img width="1492" height="895" alt="Demo_of_Gleam_House_3" src="https://github.com/user-attachments/assets/81facddb-9625-4b6e-818f-7b71722b5a06" />

https://github.com/user-attachments/assets/1a1fdcd0-d776-4202-861a-376f484ca623

**Other demos**

There are also 11 smaller demos showcasing individual engine features.

Here are a few examples:

- Rendering 2D shapes: 60k shapes all move around and change their transforms at random.
All of this is handled and rendered at ~48 FPS.

https://github.com/user-attachments/assets/8e6215c2-b3fb-4f1d-abbf-89c2470dbbe2

- Rendering 2D sprites: 10k sprites all move around and are gravitationally repelled by the mouse.
All of this is handled and rendered at ~60 FPS

https://github.com/user-attachments/assets/4ee2b3ac-7061-40fa-8ca1-58ca98bad551

- Post-processing shaders: A few different types of post-processing shaders are applied on a scene with many shapes.

https://github.com/user-attachments/assets/f9c37a2e-3a96-47ba-80c1-fca1c52a7dc5

- Transform hierarchies: The character's arms, legs and head are parented by the body, and each limb is additionally parented by a joint
so that it rotates around the point where it connects to the body.

https://github.com/user-attachments/assets/785abfa4-d5e0-4d06-a560-6f923bd6d9d7

- Custom shaders and textures: A few different shaders are shown, all of which combine 2 textures together based on some geometric animation

https://github.com/user-attachments/assets/66fb385b-ad0d-4a3d-9eea-2d06f59cfaa2

## Project Structure

```
src/
├── Core/           # Engine foundation, event system, window management
├── Graphics/       # Low-level graphics subsystem, OpenGL context
├── Renderer2D/     # 2D rendering pipeline, batching, cameras, shapes
├── GUI/            # ImGui integration and widget system
└── Tools/          # Developer utilities and camera controllers

demos/              # 11 sample applications demonstrating engine features
dep/                # Third-party dependencies (GLFW, GLAD, ImGui, EnTT, GLM, STB)
```

## Architecture Philosophy

Pekan is designed around modularity and explicit structure. Core engine functionality is split into subsystems (Graphics, Renderer2D, GUI, Tools) that can be
enabled or disabled at compile time. The `SubsystemManager` resolves dependencies and guarantees a deterministic initialization order.

Applications inherit from `PekanApplication` and follow a clear init/update/render lifecycle. On top of this, a layer stack is used to organize
game logic, rendering, UI, and debug tools into independent layers with well-defined update and event propagation order.

Game logic is built on an EnTT-based ECS, keeping data and systems decoupled and allowing features to scale without coupling gameplay code to the engine core.

## Dependencies

- **GLFW**: Cross-platform window and input handling
- **GLAD**: OpenGL loader
- **GLM**: Mathematics library for graphics
- **ImGui**: Immediate mode GUI
- **EnTT**: Entity-component system
- **STB**: Image loading

## License

See [LICENSE](LICENSE) for details.

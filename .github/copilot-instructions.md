<!--
Guidance for AI coding agents working on the Pekan repository.
Keep this file short and directly actionable. Refer to specific files and patterns.
-->

# Pekan — Copilot instructions (concise)

Quick orientation
- Pekan is a small C++ 3D/2D engine using OpenGL, GLFW, GLAD, ImGui and glm (see `CMakeLists.txt` and `dep/`).
- Core runtime lives in `src/Core` (engine bootstrap, `PekanEngine`, `PekanApplication`, `SubsystemManager`, event system, layer stack).
- Subsystems (Graphics, GUI, Renderer2D, Tools) live under `src/` and register via `SubsystemManager` (see `src/Core/ISubsystem.h` and `src/Core/SubsystemManager.*`).
- Demos are standalone applications under `demos/` which show common usage patterns (e.g. `demos/Demo00/main.cpp`).

What to do first when making code changes
- Read `CMakeLists.txt` at repo root to understand how targets are composed (Core static lib + demo executables + third-party deps under `dep/`).
- Inspect a demo (e.g. `demos/Demo00`) to see subsystem macros like `PEKAN_INCLUDE_SUBSYSTEM_GRAPHICS` and the application lifecycle (`init()`, `run()`, `exit()`).

Architecture & patterns to preserve
- Subsystem pattern: implementations inherit `ISubsystem` and call `SubsystemManager::registerSubsystem()` (see `src/Graphics/GraphicsSystem.h`). New subsystems should follow the same registration + `getParent()` semantics.
- Layer stack & events: application uses a LayerStack and dispatches events first to layers then to registered `EventListener`s; unhandled events are queued (`src/Core/PekanApplication.*`, `src/Core/Events/`). Keep event ownership and move-semantics consistent (events are moved into dispatch helpers).
- Single global window/engine: `PekanEngine` holds a single global `Window` instance and single application. Avoid creating multiple PekanApplication instances.

Build / run / debug notes (Windows / Visual Studio)
- Project uses CMake. Common workflow on Windows (Visual Studio generator):
  1) Create `build/` and run cmake; this repo includes a pre-generated `build/` with solution files but you can regenerate:

```powershell
mkdir build; cd build; cmake .. -G "Visual Studio 16 2019" -A x64
```

  2) Open `Pekan.sln` inside `build/` and build Debug/Release. The demos compile to executables under the `build/` tree.
- The demos set `Demo08` as default startup in CMake when `WITH_DEMO_PROJECTS` is ON.

Key files to reference when implementing changes
- Engine lifecycle: `src/Core/PekanEngine.*`, `src/Core/PekanApplication.*`
- Subsystems: `src/Core/ISubsystem.h`, `src/Core/SubsystemManager.*`, examples under `src/Graphics/`, `src/GUI/`.
- Event system: `src/Core/Events/*` and `src/Core/EventListener.*`.
- Rendering: `src/Graphics/Render*`, `src/Graphics/Shaders/` and `src/Renderer2D/`.

Code-style and conventions (observable)
- C++17, header-first include guards/pragmas are used. Example:

```cpp
#pragma once
```
- Logging and assertions use engine macros (`PekanLogger.h`, `PK_ASSERT*`, `PK_LOG_ERROR`). Use these for consistency.
- Prefer RAII and smart pointers for listeners and subsystems (`std::shared_ptr`/`std::weak_ptr` used in `PekanApplication`).

Integration & external dependencies
- Third-party libs are included as subprojects in `dep/` (glfw, glad, imgui, glm). Do not replace those with external package managers without updating `CMakeLists.txt`.

Examples to copy from
- Registering subsystem: `src/Graphics/GraphicsSystem.h` and `demos/Demo00/main.cpp` (uses `PEKAN_INCLUDE_SUBSYSTEM_GRAPHICS`).
- Event dispatch pattern: `src/Core/PekanApplication::handle*Event` and `_dispatchEvent` template helper.

If unsure
- Prefer minimal, local changes and run a demo to verify startup. Use the existing `build/` solution if you don't want to regenerate CMake.
- Ask for clarification and point to the demo that fails when you need a repro.

End of file — leave this short and update with new concrete guidance if structure changes.

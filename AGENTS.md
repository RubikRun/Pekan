# AGENTS.md

> Guidance for AI coding agents working on **Pekan v0.2** (Instagram demos branch).
> When in doubt, match the conventions already present in the surrounding code and existing demos.

---

## 1. What is this branch

**Pekan** is a 2D/3D game engine written in C++ with OpenGL and ImGui.
This branch (`v0.2_ig`) is pinned to **Pekan v0.2** — an older, stable snapshot used specifically for **Instagram demos**.

Current engine development happens on `master`, which has diverged significantly (ECS, Editor, more modules, etc.). **Do not assume master APIs exist here.** Trust the code in this repo.

### Purpose of work on this branch

Create visually appealing demos suitable for Instagram (games, shaders, math visuals, particle-like effects, etc.). Typical requests:

- A mini-game (Flappy Bird, Snake, Chess, …)
- A custom shader effect
- A cool looping graphic / math visualization
- Sprite-based interactive scenes

### Priorities for Instagram demos

1. **Visual appeal first** — the final product should look good on camera / in a reel.
2. **Performance matters** when it affects the look (e.g. 10k trees at 30 FPS vs 2k). Prefer batching, reuse textures, avoid per-object draw calls.
3. **Code readability and cleanliness are not a priority.** Skip polished abstractions, extensive comments, and journal notes unless they help you ship faster or fix a performance issue.
4. **Ignore** `build/` and `notes/`. Do not create or update development journal entries for demo work.

---

## 2. Repository layout

```
Pekan/
├── src/
│   ├── Core/              # App lifecycle, layers, window, events, subsystems, logger, time, utils
│   │   ├── Events/        # Event base + Key/Mouse/Window events and enums, EventListener
│   │   ├── Logger/        # PekanLogger (PK_LOG_* / PK_ASSERT macros)
│   │   ├── Time/          # DeltaTimer, FpsLimiter
│   │   └── Utils/         # File/Math/Randomization utils, stb
│   ├── Graphics/          # Low-level OpenGL wrappers
│   │   └── RenderComponents/  # Shader, VertexArray, VertexBuffer, IndexBuffer, Texture1D/2D
│   ├── Renderer2D/        # 2D batch renderer: Camera2D, Transformable2D, Sprite, Shapes, shaders
│   │   ├── Shapes/        # Rectangle, Circle, Triangle, Polygon, LineShape
│   │   ├── Sprite/        # Textured quads
│   │   └── Shaders/       # Built-in batch / line shaders (.glsl / .pkshad)
│   ├── GUI/               # ImGui integration: GUIWindow + Widget library
│   └── Tools/             # Helpers (currently Camera2D pan/zoom controller)
├── demos/                 # Demo00 … Demo08 — standalone executables (best reference material)
├── dep/                   # Third-party: glfw, glad, imgui, glm, stb
├── build/                 # CMake output (generated — ignore)
├── notes/                 # Author's old journals (ignore for Instagram work)
├── CMakeLists.txt
├── gen.bat                # Windows: generate Visual Studio solution
└── README.md              # May be outdated; trust the code
```

**There is no ECS / entt / EntityID on this branch.** Game objects are plain C++ classes or members on a `Layer`. A "scene" is just a demo-local class deriving from `Pekan::Layer` — not an engine type.

**Static library targets:** `Core`, `Graphics`, `Renderer2D`, `GUI`, `Tools`.

---

## 3. Building

- **Requirements:** CMake 3.6+, OpenGL 4.3+, C++17 compiler.
- **Windows (primary):**
  ```powershell
  gen.bat
  ```
  Auto-detects Visual Studio, generates `build/Pekan.sln`, and a Ninja build under `build/build-ninja/` for `compile_commands.json` / clangd. Forwards `-D` CMake options.
- Default VS startup project: **Demo08**.
- `WITH_DEMO_PROJECTS` (ON by default) controls whether demos are built.
- Each demo sets `VS_DEBUGGER_WORKING_DIRECTORY` to its source folder so `"resources/..."` paths resolve when run from Visual Studio.

### CMake conventions

- Source files are listed **explicitly** in each `CMakeLists.txt` (no globbing). When you add a `.h`/`.cpp`, add it to the relevant CMakeLists.
- New demos need:
  1. `demos/DemoXX/CMakeLists.txt`
  2. `add_subdirectory(demos/DemoXX)` + `set_target_properties(DemoXX PROPERTIES FOLDER "Demos")` in the root `CMakeLists.txt`

---

## 4. Architecture

### Subsystems

A subsystem implements `ISubsystem` (`init()`, `exit()`, `getSubsystemName()`, optional `getParent()`).
`SubsystemManager` inits parents before children.

Register subsystems in `main()` **before** `application.init()`:

```cpp
PEKAN_INCLUDE_SUBSYSTEM_GRAPHICS;     // Pekan::Graphics::GraphicsSystem  (loads GLAD)
PEKAN_INCLUDE_SUBSYSTEM_RENDERER2D;   // Pekan::Renderer2D::Renderer2DSystem  (parent: Graphics)
PEKAN_INCLUDE_SUBSYSTEM_GUI;          // Pekan::GUI::GUISystem  (ImGui)
```

There is **no** include macro for Tools — link the `Tools` library and `#include "PekanTools.h"`.

### Application → LayerStack → Layers

1. App derives from `Pekan::PekanApplication`.
2. Implement `_fillLayerStack(LayerStack&)` (required) and optionally `_init()`, `getName()`, `getProperties()`.
3. Lifecycle: `init()` → `run()` → `exit()`.
4. Main loop each frame: poll events → update all layers → render all layers → swap buffers → optional FPS limit.

**Layers** (`Pekan::Layer`):

| Method | Role |
|--------|------|
| `bool init()` | Create GPU resources (private; called by LayerStack) |
| `void update(double deltaTime)` | Logic — **deltaTime is in seconds** |
| `void render() const` | Draw |
| `void exit()` | Destroy resources |
| `getLayerName()` | Identifier string |

- Render order: **forward** (first pushed = bottom / background).
- Event order: **reverse** (top layer first). Return `true` from an event handler to stop propagation.
- Layers are `std::shared_ptr` (`Layer_Ptr`). Use `setParent()` when one layer must `init()` before another (e.g. scene needs GUI default values).

### Events & input

**Event-driven** (override on `Layer` / `EventListener`):

```cpp
bool onKeyPressed(const KeyPressedEvent& event) override;
bool onKeyReleased(const KeyReleasedEvent& event) override;
bool onMouseMoved(const MouseMovedEvent& event) override;
bool onMouseScrolled(const MouseScrolledEvent& event) override;
bool onMouseButtonPressed(const MouseButtonPressedEvent& event) override;
bool onMouseButtonReleased(const MouseButtonReleasedEvent& event) override;
bool onWindowResized(const WindowResizedEvent& event) override;
```

**Polling** (great for games — see Demo03):

```cpp
PekanEngine::isKeyPressed(KeyCode::KEY_W);
PekanEngine::isMouseButtonPressed(MouseButton::...);
PekanEngine::getMousePosition();           // window pixels, Y down
PekanEngine::getMousePositionNdc();        // NDC, Y up
Renderer2DSystem::getMousePosition();      // world space via active Camera2D
```

Quit from a layer: `m_application->stopRunning();`

### ApplicationProperties knobs

```cpp
ApplicationProperties props;
props.fps = 60.0;                          // >0 = hard FPS cap; <=0 uses VSync if useVSync
props.useVSync = true;
props.numberOfSamples = 16;                // MSAA samples
props.windowProperties.width = 1080;
props.windowProperties.height = 1080;      // square = good for Instagram
props.windowProperties.title = getName();
props.windowProperties.fullScreen = false;
props.windowProperties.hideCursor = false;
props.windowProperties.initialPosition = {30, 40};
```

---

## 5. Rendering

Two complementary paths:

| Path | Use when |
|------|----------|
| **Renderer2D** (`Sprite`, shapes) | Colored primitives, textured sprites, many objects (auto-batched) |
| **Graphics::RenderObject** | Custom GLSL, raw geometry, 3D, pixel-space games |

You can mix both in one frame (Demo04 does).

### Renderer2D frame (required for shapes/sprites)

```cpp
void render() const override
{
    using namespace Pekan::Renderer2D;
    using namespace Pekan::Graphics;

    Renderer2DSystem::beginFrame();
    RenderCommands::clear();
    // shape.render() / sprite.render() ...
    Renderer2DSystem::endFrame();   // flushes the batch
}
```

`render()` on shapes/sprites only **submits** to the batch; the actual GL draw happens in `endFrame()`.

### Camera2D

```cpp
auto camera = std::make_shared<Pekan::Renderer2D::Camera2D>();
camera->create(10.0f);   // or create(width, height) in world units
Pekan::Renderer2D::Renderer2DSystem::setCamera(camera);
Pekan::Tools::PekanTools::enableCameraController2D(camera);  // drag pan + scroll zoom
```

World space is Y-up. Camera is owned by the demo (`shared_ptr`); the system holds a `weak_ptr`.

### Transformable2D (base of Shape and Sprite)

```cpp
setPosition({x, y});
setRotation(radians);
setScale({sx, sy});
move(delta); rotate(delta); scale(factor);   // incremental
```

Transform order: **translation × rotation × scale**. Local origin is the center of the shape/sprite.

### Shapes (`Pekan::Renderer2D`)

All: `create(...)` → configure → `setColor(glm::vec4)` → `render()` → `destroy()` in `exit()`.

| Class | Create args |
|-------|-------------|
| `RectangleShape` | `create(width, height)` |
| `CircleShape` | `create(radius)` + optional `setSegmentsCount(n)` |
| `CircleShapeStatic<N>` | Fixed segment count (faster; prefer for many circles) |
| `TriangleShape` | `create(a, b, c)` as `glm::vec2` |
| `PolygonShape` | `create(std::vector<glm::vec2>)` — auto-triangulated |
| `LineShape` | `create(pointA, pointB, thickness)` — **batched** thick line |

`Line` (not a Shape) is a **debug 1px line with its own draw call**. Prefer `LineShape` for many lines. Call `line.update()` before `line.render()` each frame (syncs camera matrix).

### Sprites

```cpp
Pekan::Graphics::Image image("resources/foo.png");
auto texture = std::make_shared<Pekan::Graphics::Texture2D>();
texture->create(image);

Pekan::Renderer2D::Sprite sprite;
sprite.create(texture, width, height);
sprite.setPosition({x, y});
sprite.render();
```

**No built-in sprite-sheet API.** Animate by swapping textures: `sprite.setTexture(frames[i])` on a timer (Demo08).

For transparent PNGs, enable blending in `init()`:

```cpp
RenderState::enableBlending();
RenderState::setBlendFunction(BlendFactor::SrcAlpha, BlendFactor::OneMinusSrcAlpha);
RenderState::enableMultisampleAntiAliasing();  // needs numberOfSamples > 1
RenderState::setBackgroundColor(r, g, b, a);
```

### Custom shaders via RenderObject

```cpp
using Pekan::Graphics::RenderObject;
using Pekan::FileUtils;

m_renderObject.create(
    vertices, sizeof(vertices),
    { { ShaderDataType::Float2, "position" }, { ShaderDataType::Float2, "texCoord" } },
    BufferDataUsage::StaticDraw,
    FileUtils::readTextFileToString("resources/vs.glsl").c_str(),
    FileUtils::readTextFileToString("resources/fs.glsl").c_str()
);
m_renderObject.setTextureImage(image, "uTex0", 0);

// each frame:
Shader& shader = m_renderObject.getShader();
shader.bind();
shader.setUniform1f("uTime", t);
shader.setUniform2fv("uPosition", pos);
shader.unbind();
m_renderObject.render();
```

Useful Shader uniforms: `setUniform1f/1i/1iv/2fv/3fv/4fv/Matrix4fv`.
Hot-swap shaders at runtime with `setShaderSource(vs, fs)` (Demo04).
Update dynamic geometry with `setVertexData` / `setVertexSubData` / `setIndexData`.

Demo shaders live in `demos/DemoXX/resources/*.glsl`. Engine batch shaders live in `src/Renderer2D/Shaders/` (`.pkshad` files are preprocessed templates — usually leave them alone).

**No post-processing / FBO pipeline** in v0.2. Full-screen effects = a fullscreen `RenderObject` quad with a custom fragment shader.

### Performance tips

- Many shapes/sprites between one `beginFrame`/`endFrame` → typically **one batched draw**.
- Reuse the same `Texture2D` across sprites (unique textures per batch are limited by GPU texture slots; overflow auto-flushes).
- Prefer `CircleShapeStatic<N>` over `CircleShape` when segment count is fixed.
- Prefer `LineShape` over `Line` for bulk lines.
- Don't rebuild geometry every frame unless the shape actually changes.
- For thousands of moving objects, follow Demo06 (preallocate vectors, toggle how many you `render()`).

---

## 6. GUI

Optional ImGui layer for tweakable parameters while developing a demo. For a pure Instagram capture you can omit GUI entirely (Demo03 style).

```cpp
class MyGUIWindow : public Pekan::GUI::GUIWindow {
public:
    MyGUIWindow(Pekan::PekanApplication* app) : GUIWindow(app) {}
    bool init() override;
private:
    Pekan::GUI::GUIWindowProperties getProperties() const override;
};
```

Widgets must be `std::shared_ptr`. Call `widget->create(this, ...)` — that registers them on the window.

| Widget | Notes |
|--------|-------|
| `TextWidget`, `SeparatorWidget`, `NewLineWidget` | Layout |
| `ButtonWidget` | `isClicked()` |
| `CheckboxWidget` | `isChecked()` |
| `ComboBoxWidget` | `getIndex()` |
| `SliderFloatWidget`, `SliderFloat2Widget`, `SliderIntWidget` | `getValue()` |
| `DragFloat2Widget` | `getValue()` |
| `ColorEdit3Widget`, `ColorEdit4Widget` | `getValue()` |
| `FPSDisplayWidget` | Auto FPS |

ImGui captures mouse/keyboard when hovered — keep the GUI layer **above** the scene in the stack.

---

## 7. Creating a new Instagram demo

### Recommended starting points

| Goal | Copy from |
|------|-----------|
| Full-screen game (Flappy Bird, Snake, …) | **Demo03** (Snake) — keyboard polling, entity classes, no GUI |
| Custom GLSL / texture effects | **Demo04** — shader hot-swap, `uTime`, textures + shapes |
| Math / hypnotic looping visuals | **Demo05** — Mandelbrot times-table lines |
| Many animated shapes (particle feel) | **Demo06** — hundreds of bouncing shapes |
| Sprite field / mouse interaction / frame anim | **Demo08** — textures, repulsion, anim frames |
| Minimal shader + uniform | **Demo00** |
| Procedural dynamic geometry | **Demo01** |
| Transform playground / shape reference | **Demo07** |
| Square window for IG | **Demo04** (`960×960`) |

### File checklist

```
demos/DemoXX/
├── CMakeLists.txt
├── main.cpp
├── DemoXX_Application.h / .cpp
├── DemoXX_Scene.h / .cpp
├── DemoXX_GUIWindow.h / .cpp     # optional
├── [GameObject].h / .cpp         # optional entity classes
└── resources/
    ├── *.glsl
    └── *.png
```

Plus root `CMakeLists.txt`: `add_subdirectory` + FOLDER property.

### `main.cpp` template

```cpp
#include "GraphicsSystem.h"
#include "Renderer2DSystem.h"   // if using Renderer2D
#include "GUISystem.h"          // if using GUI

#include "PekanLogger.h"
#include "DemoXX_Application.h"
using Demo::DemoXX_Application;

int main(void)
{
    PEKAN_INCLUDE_SUBSYSTEM_GRAPHICS;
    PEKAN_INCLUDE_SUBSYSTEM_RENDERER2D;
    // PEKAN_INCLUDE_SUBSYSTEM_GUI;

    DemoXX_Application application;
    if (!application.init())
    {
        PK_LOG_ERROR("Application failed to initialize.", "Pekan");
        return -1;
    }
    application.run();
    return 0;
}
```

### Application template

```cpp
namespace Demo {

bool DemoXX_Application::_fillLayerStack(Pekan::LayerStack& layerStack)
{
    auto scene = std::make_shared<DemoXX_Scene>(this);
    // Optional GUI:
    // auto gui = std::make_shared<DemoXX_GUIWindow>(this);
    // scene->attachGUIWindow(gui);
    layerStack.pushLayer(scene);
    // layerStack.pushLayer(gui);
    // scene->setParent(gui);  // only if scene init() needs GUI defaults
    return true;
}

Pekan::ApplicationProperties DemoXX_Application::getProperties() const
{
    Pekan::ApplicationProperties props;
    props.fps = 60.0;
    props.numberOfSamples = 16;
    props.windowProperties.title = getName();
    props.windowProperties.width = 1080;
    props.windowProperties.height = 1080;  // square for Instagram
    return props;
}

} // namespace Demo
```

### Scene template

```cpp
namespace Demo {

class DemoXX_Scene : public Pekan::Layer {
public:
    DemoXX_Scene(Pekan::PekanApplication* application) : Layer(application) {}
    bool init() override;
    void update(double deltaTime) override;
    void render() const override;
    void exit() override;
    std::string getLayerName() const override { return "scene_layer"; }
};

} // namespace Demo
```

### CMakeLists template

```cmake
project(DemoXX)
add_executable(DemoXX
    main.cpp
    DemoXX_Application.h DemoXX_Application.cpp
    DemoXX_Scene.h DemoXX_Scene.cpp
)
target_link_libraries(DemoXX PRIVATE Core Renderer2D Tools)  # + GUI if needed
set_target_properties(DemoXX PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}")
```

### Resource paths

Always relative to the demo folder: `"resources/foo.png"`, `"resources/bar.glsl"`.
Load text with `Pekan::FileUtils::readTextFileToString(...)`.

### Useful utilities

```cpp
Pekan::Utils::getRandomInt(min, max);
Pekan::Utils::getRandomFloat(min, max);
Pekan::Utils::getRandomVec2(xRange, yRange);
Pekan::Utils::getRandomColor(/* randomizeAlpha */ false);
```

Common animation helper (used in several demos):

```cpp
static float osc(float x) { return (cos(x) + 1.0f) / 2.0f; }
static float osc(float x, float a, float b) { return a + (b - a) * osc(x); }
```

### Namespaces

| Code | Namespace |
|------|-----------|
| Engine | `Pekan`, `Pekan::Graphics`, `Pekan::Renderer2D`, `Pekan::GUI`, `Pekan::Tools`, `Pekan::Utils`, … |
| Demos | `Demo` |

---

## 8. Code style (light)

Match existing files; don't over-engineer for demos.

- **Indentation:** tabs.
- **Headers:** `#pragma once`.
- **Naming:** `PascalCase` types; `camelCase` functions/locals; `m_` members; `s_` statics; `g_` globals; `PK_` / `PEKAN_` macros.
- **Private hooks:** leading underscore (`_init`, `_fillLayerStack`, `_render`).
- **Smart-pointer aliases:** `Layer_Ptr`, `Texture2D_ConstPtr`, `Camera2D_Ptr`, etc.
- **Logging:** `PK_LOG_ERROR/WARNING/INFO/DEBUG(msg, sender)` and `PK_ASSERT` — not `std::cout`.
- Prefer `glm` types (`glm::vec2`, `glm::mat4`, …).

For Instagram demos, skip elaborate documentation and engine-wide refactors. Touch the engine only when a demo truly needs a capability that doesn't exist (and keep the change minimal).

---

## 9. What does NOT exist in v0.2

Do not invent or import these from master / other engines:

- ECS / `entt` / components / systems / `EntityID`
- Editor application
- Scene serializer / `.pksc` files
- Built-in post-processing, framebuffer API, particle system, audio, networking
- Sprite-sheet / animation clip helpers
- Scene graph / parent-child transforms between sprites
- Recurring callbacks on the application (master-era feature)

Implement what you need locally inside the demo (a simple particle struct array, a chess board class, etc.).

---

## 10. Quick checklist for agents

- [ ] New demo registered in root `CMakeLists.txt` and has its own `CMakeLists.txt` with all sources listed.
- [ ] Subsystem macros match linked libraries (`GRAPHICS` / `RENDERER2D` / `GUI`).
- [ ] Working directory / resource paths use `"resources/..."`.
- [ ] Renderer2D draws are wrapped in `beginFrame()` / `endFrame()`.
- [ ] GPU resources `create()`'d in `init()` and `destroy()`'d in `exit()`.
- [ ] Window size / aspect chosen for the Instagram format (often square).
- [ ] Blending + MSAA enabled when using transparent sprites.
- [ ] Prefer batched shapes/sprites; avoid one `RenderObject`/`Line` per tiny object when counts get large.
- [ ] Demo code lives in `namespace Demo`.
- [ ] Visually verify the result — polish the look, not the abstractions.

# [AGENTS.md](http://AGENTS.md)

> Guidance for AI coding agents working on **Pekan**.
> When in doubt, match the conventions already present in the surrounding code.

---

## 1. What is Pekan

Pekan is a **2D game engine**. It focuses on:

- **Modular, subsystem-based architecture** — features are split into subsystems that can be enabled/disabled at compile time.
- **ECS-driven design** — game logic is built on an ECS registry, keeping data (components) and logic (systems) decoupled.
- **High-performance 2D rendering** — a custom OpenGL-based batch renderer for sprites and shapes.

The author has been building Pekan from first principles.  
**Prefer designing things "the Pekan way" over copying patterns from elsewhere.**
Architecture and clarity matter here.

---

## 2. Repository layout

```
Pekan/
├── src/                 # Engine source, one folder (and CMake target) per module
│   ├── Core/            # Engine foundation: app lifecycle, layers, scenes, events,
│   │   │                #     subsystem manager, logger, time, entity infra, utils
│   │   ├── Entity/      #   EntityID, EntityIDComponent, NameComponent, DisabledComponent
│   │   ├── Events/      #   Event base + Key/Mouse/Window events and enums, EventListener
│   │   ├── Logger/      #   PekanLogger (PK_LOG_* / PK_ASSERT macros)
│   │   ├── Time/        #   DeltaTimer, FpsLimiter, RecurringCallback
│   │   └── Utils/       #   File/Math/Randomization utils, stb
│   ├── Graphics/        # Low-level OpenGL wrappers (GpuResources: Shader, VertexArray,
│   │                    #   buffers, textures, framebuffers), RenderCommands, GLCall
│   ├── Renderer2D/      # 2D rendering pipeline: cameras, transforms, sprites, shapes,
│   │                    #   materials, the ECS render systems, shaders (.glsl)
│   ├── GUI/             # ImGui integration: GUIWindow + a Widget library
│   └── Editor/          # The Pekan Editor application (scene/entities inspector)
├── demos/               # Demo00 … Demo10, each a standalone executable
├── dep/                 # Third-party deps (git submodules / vendored): glfw, glad,
│                        #   imgui, glm, entt, stb, json (nlohmann)
├── notes/               # The author's development notes (see §8) — READ THESE
│   ├── dev/             #   Dev journal (dev_NNNN.txt) + todo.txt (numbered tasks)
│   ├── plan/            #   Design docs for upcoming features (e.g. .pksc file format)
│   └── general/         #   High-level notes
├── build/               # CMake build output (generated, not committed)
├── CMakeLists.txt       # Top-level build
├── gen.bat              # Windows script to generate a Visual Studio solution
└── README.md            # Public-facing overview
```

> Note: The README might be outdated and is not necessarily accurate on technical details. Trust the actual code over the README.

---

## 3. Building

- **Requirements:** CMake 3.6+, an OpenGL 3.3+ capable system, a C++20 compiler.
- **Windows (primary dev environment, PowerShell):**
  ```powershell
  gen.bat
  ```
  This script auto-detects the latest installed Visual Studio and generates `build/Pekan.sln` using the CMake executable that ships with Visual Studio (not needed to have "cmake" in PATH). Open the generated solution and build. The script also forwards `-D` CMake options.
- The default startup project in the VS solution is `**Editor**`.
- `CMAKE_EXPORT_COMPILE_COMMANDS` is on - **clangd** is configured via `.clangd` to read `build/build-ninja`.
- Demos copy their `resources/` folder next to the built `.exe` via a post-build step, so executables can be run directly.
- `WITH_DEMO_PROJECTS` (ON by default) controls whether demos are built. Pekan can also be consumed as a dependency (CMake uses `CMAKE_CURRENT_SOURCE_DIR`, not `CMAKE_SOURCE_DIR`).

### CMake conventions

- Each module under `src/` is its own **static library** target (`Core`, `Graphics`, `Renderer2D`, `GUI`, `Editor`). Each demo is an **executable** target.
- Source files are listed **explicitly** in `CMakeLists.txt` (no globbing). When you add a `.h`/`.cpp`, add it to the relevant `CMakeLists.txt`.
- Headers are listed alongside sources, and grouped with `SOURCE_GROUP(...)` into virtual folders for the VS solution.
- A per-module root-dir macro is defined (e.g. `PEKAN_ROOT_DIR`, `PEKAN_RENDERER2D_ROOT_DIR`) and used to build absolute resource/shader paths.

---

## 4. Architecture & design philosophy

### Subsystems

- A subsystem implements `ISubsystem` (`init()`, `exit()`, `getSubsystemName()`, optional `getParent()`).
- Subsystems declare dependencies by returning a parent from `getParent()`. `SubsystemManager` resolves them and guarantees a **deterministic init order (parents before children)**. A `nullptr` parent means the parent is `Core`.
- Which subsystems an application uses is known at compile time from an application's `main()` via include macros, e.g.:
  ```cpp
  PEKAN_INCLUDE_SUBSYSTEM_GRAPHICS;
  PEKAN_INCLUDE_SUBSYSTEM_RENDERER2D;
  ```
- **Naming:** engine subsystem classes are suffixed `Subsystem` (e.g. `Renderer2DSubsystem`, `GraphicsSubsystem`, `GUISubsystem`) to distinguish them from ECS systems.

### Application-Layer-Scene model

- An app derives from `PekanApplication` and implements `_fillLayerStack()` (and optionally `_init()`, `getName()`, `getProperties()`).
- Lifecycle is **init → run → exit**; `PekanApplication` owns the main loop, delta timing, recurring callbacks, and frame-begin/frame-end callbacks.
- A `LayerStack` composes any number of `Layer`s (e.g. game world, UI, debug overlays). Layers are rendered bottom-to-top.
- **Event propagation is the reverse of render order** — the top-most (last-rendered) layer gets events first, and a layer can mark an event handled to stop propagation. Unhandled events fall through to an event queue.
- `Scene` **is a `Layer`** that owns an `entt::registry`. Scenes are the home of the ECS world.

### ECS conventions

- **Components are data.** They are plain `struct`s (e.g. `TransformComponent2D`, `SpriteComponent`). They may carry small, pure helper methods operating on their own data (e.g. `move()`, `rotate()`, `scale()`), but no engine logic.
- **Systems are logic.** They are classes exposing **static** functions that operate on a `registry` (e.g. `RenderSystem2D`, `TransformSystem2D`, `CameraSystem2D`, `SpriteSystem`). Systems hold no per-entity state.
- Iterate with views, and exclude as needed: `registry.view<SpriteComponent, TransformComponent2D>(entt::exclude<DisabledComponent>)`.
- **Naming:** components → `XComponent` / `XComponent2D`; ECS systems → `XSystem` / `XSystem2D`.
- **Entity identity:** `entt::entity` is an internal, recyclable handle. For anything stable/persistent (save/load, cross-entity references like parent links) use `**EntityID`** (`uint32_t` alias, `INVALID_ENTITY_ID == 0`), stored in an auto-attached `EntityIDComponent`. Always use the `EntityID` alias at call sites, never `uint32_t` directly.

---

## 5. Code style

Follow what the existing files do. Key conventions observed throughout:

- **Indentation: tabs**.
- **Headers:** `#pragma once` at the top. Order includes in groups, from most local to most external:
  1. The file's own header (e.g. `Foo.cpp` includes `Foo.h` first).
  2. Headers from the same subsystem/module.
  3. Headers from other subsystems/modules.
  4. Third-party headers.
  5. Standard library headers.
- **Namespaces:** Engine code lives in `Pekan` with nested namespaces per module (`Pekan::Renderer2D`, `Pekan::GUI`, `Pekan::Graphics`). Demo code lives in `Demo`. Namespace braces (even nested ones) sit at column 0, and closing braces carry a comment: `} // namespace Pekan`.
- **Naming:**
  - Types (classes, structs, enums): `PascalCase`.
  - Functions / methods / local variables / parameters: `camelCase`.
  - Member variables: `m_camelCase`. Static class members: `s_`. File-scope statics / globals: `g_`.
  - Macros and compile-time constants: `UPPER_SNAKE_CASE`, prefixed `PK_` or `PEKAN_`.
  - Smart-pointer aliases: suffix the type with `_Ptr` for a `shared_ptr` and `_ConstPtr` for a `shared_ptr` to const (e.g. `typedef std::shared_ptr<Widget> Widget_Ptr;` and `typedef std::shared_ptr<const Widget> Widget_ConstPtr;`).
  - Leading underscore (`_init`, `_render`, `_logErrorToConsole`, …): a function not meant to be called by users of the class/module — it exists for internal purposes only. Usually falls into one of these categories:
    - A virtual hook invoked by a public non-virtual counterpart (e.g. `init()` → `_init()`, `render()` → `_render()`).
    - An internal helper function (e.g. the `Pekan::Logger` functions behind the `PK_LOG_`* macros).
- **Class layout:** sections are explicitly labeled, e.g. `public: /* functions */`, `private: /* variables */`, `private: /* functions */`. The `init/exit/update/render` (and their `_`-prefixed overrides) are declared **at the top of the class**.
- **Comments are expected.** Most classes, members, and non-trivial functions have a short explanatory comment of what the thing is/does and why (sometimes including edge cases and future considerations). This is a well documented and commented codebase — keep it that way.
  - That said, follow the global rule: do **not** add redundant comments that merely restate the code.
- **C++20 features** are welcome and used (e.g. designated initializers for struct init, `std::string_view`, `constexpr`).
- Prefer `glm` types (`glm::vec2`, `glm::mat3`, …) for math.

### Logging & assertions

Use the logger macros (defined in `Core/Logger/PekanLogger.h`) rather than `printf`/`std::cout`:

- `PK_LOG_ERROR(msg, sender)`, `PK_LOG_WARNING(...)`, `PK_LOG_INFO(...)`, `PK_LOG_DEBUG(...)`.
- The `sender` is a short source tag — engine-level messages use `"Pekan"`. The `msg` argument supports stream syntax (`"x = " << x`).
- Assertions: `PK_ASSERT(condition, msg, sender)` and the shorter `PK_ASSERT_QUICK(condition)`. Asserts are compiled only in debug mode.
- Logging is heavily configurable via compile-time `PK_LOGGER_`* macros and runtime `PEKAN_LOGGER_`* environment variables. Keep new logging consistent with this system.

---

## 6. GUI

- GUI is built on **ImGui**, wrapped so engine code doesn't touch ImGui directly (we may write some direct ImGui code from time to time during development, but later abstract it away in the GUI module).
- A `GUIWindow` (a `Layer`) hosts `Widget`s. GUI is built using the existing typed widgets (`ButtonWidget`, `CheckboxWidget`, `SliderFloatWidget`, `ColorEdit4Widget`, `ComboBoxWidget`, `TextWidget`, `SelectableListWidget`, `ContextMenuWidget`, …).
- Create one ImGui frame **per frame**, not per window. User-facing messages go through `PekanUserMessageBox` (error/warning/info popups).
- Widgets support `hide()`/`unhide()`/`isHidden()`; hidden widgets are not rendered and skip `_render()`.

---

## 7. Demos

- `demos/Demo00` … `Demo10` are standalone executables, each demonstrating a feature or multiple features (e.g. shapes, sprites, post-processing, transform hierarchies, custom shaders, a Snake game in Demo03, etc.).
- Standard demo structure:
  - `main.cpp` — includes the needed subsystem macros, creates the app, `init()` + `run()`.
  - `DemoNN_Application.{h,cpp}` — derives `PekanApplication`.
  - `DemoNN_Scene.{h,cpp}` — derives `Scene` and fills the ECS world.
  - Gameplay-specific classes and `resources/` (shaders, textures).
- Demos live in namespace `Demo` and link against the relevant engine targets (`Core`, `Graphics`, …).
- When changing engine APIs, **keep the demos compiling** — they are used as integration tests as well.

---

## 8. The `notes/` directory (important context)

The author keeps a detailed development journal and design docs. These are the best source of *why* decisions were made.

- `notes/dev/dev_NNNN.txt` — chronological dev journal. The latest (`dev_0006.txt`) is actively appended to. **Most commits add a corresponding journal entry.** Entries are dated and often wrapped in `{ ... }` blocks describing the task, the reasoning, and the outcome.
- `notes/dev/todo.txt` — a numbered task list. Format: `NNNN DD.MM.YYYY (DONE): description`. Tasks are referenced from journal entries (e.g. "Let's look at TO-DO task 0079").
- `notes/plan/` — design docs for larger features (e.g. `plan_0006_pkscFileFormat.md` specifies the `.pksc` JSON scene format). Read the relevant plan before implementing a planned feature.

If you do any non-trivial work it's usually a good idea to record it in the current dev journal (what was done and why, what the thought process was, reasoning behind design decisions, even low-level ones).

Stay focused on the task at hand. If you spot an unrelated bug or an opportunity for improvement along the way, note it down in `todo.txt` rather than fixing it as part of the current change. This keeps each diff clean and scoped to the single task it's solving.

## 9. Git conventions

- **Scope:** commits are **small and single-concern**, typically touching a handful of files (often 2–7). Large additions (e.g. importing a dependency or creating a skeleton for a new demo) are isolated in their own commit.
- **Subject style:** a single line, **capitalized, imperative-ish**, no trailing period. Leading verbs seen: `Add`, `Fix`, `Implement`, `Rename`, `Use`, `Make`, `Move`, `Remove`, `Replace`, `Integrate`, `Setup`, `Update`.
  - Examples: `Implement SceneSerializer::serialize()`, `Fix bug in Snake game (Demo03) where snake grows twice when eating an apple`, `Rename class RenderObject to DrawObject; Rename directory RenderComponents to GpuResources`.
- Multiple closely-related changes in one commit are joined with `;`  in the subject.
- No PR/issue trailers or multi-paragraph bodies are used; context lives in `notes/` instead.
- **Only create commits when explicitly asked.**

---

## 10. Quick checklist for agents

- New `.h`/`.cpp` files are added to the correct module's `CMakeLists.txt` (and `SOURCE_GROUP` if appropriate).
- Code uses tabs, the `Pekan`(`::Module`) namespace, `m_`/`s_`/`g_` member prefixes, and the `_`-prefixed pattern for internals.
- New components are plain data structs. New logic lives in `static` system functions. ECS iteration uses views with proper `entt::exclude`.
- Stable references use `EntityID`, not raw `entt::entity` or `uint32_t`.
- Logging/asserts use `PK_LOG_`* / `PK_ASSERT`*, not the standard library.
- Demos must always compile. If a change in engine API breaks them, demo code needs to change accordingly.
- Non-obvious decisions are explained in intent-focused comments.

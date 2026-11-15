# Pekan codebase guide

This guide maps Pekan's source code and records its architecture and development
state as of 15.11.2026, at commit `37fa11c`. It is a dated reference, not a new
implementation plan. Read [AGENTS.md](../../AGENTS.md) for working conventions, and
recheck the current code and relevant notes before reviewing or changing anything.

The author owns the design and implementation. Assistance is mainly for reviews,
discussion, and requested mechanical work.

## Where to start

| Topic | Entry points | Useful context |
| --- | --- | --- |
| Application startup, frame order, input | [PekanApplication.cpp](../../src/Core/PekanApplication.cpp), [PekanEngine.cpp](../../src/Core/PekanEngine.cpp), [Window.cpp](../../src/Core/Window.cpp) | [LayerStack.cpp](../../src/Core/LayerStack.cpp), [EventListener.h](../../src/Core/Events/EventListener.h) |
| Subsystem registration and dependencies | [ISubsystem.h](../../src/Core/ISubsystem.h), [SubsystemManager.cpp](../../src/Core/SubsystemManager.cpp) | Subsystem headers and each application's `main.cpp` |
| Entity ownership and stable IDs | [Scene.h](../../src/Core/Scene.h), [Scene.cpp](../../src/Core/Scene.cpp), [EntityID.h](../../src/Core/Entity/EntityID.h) | `.pksc` plan, Editor selection |
| Scene serialization | [SceneSerializer.cpp](../../src/Core/SceneSerializer.cpp), [Scene2DSerializer.cpp](../../src/Renderer2D/Scene2DSerializer.cpp), [SerializationUtils.h](../../src/Core/Utils/SerializationUtils.h) | [Scene format specification and plan](../../notes/plan/plan_0006_pkscFileFormat.md) |
| ECS rendering | [Scene2D.cpp](../../src/Renderer2D/Scene2D.cpp), [RenderSystem2D.cpp](../../src/Renderer2D/RenderSystem2D.cpp), [SpriteSystem.cpp](../../src/Renderer2D/Sprite/SpriteSystem.cpp) | Demo09 covers transformed and untransformed entities |
| Geometry and transforms | [TransformSystem2D.cpp](../../src/Renderer2D/TransformSystem2D.cpp), [Shapes](../../src/Renderer2D/Shapes), [MathUtils.cpp](../../src/Core/Utils/MathUtils.cpp) | Demo05, Demo07, Demo10 |
| Cameras and coordinate conversion | [CameraComponent2D.cpp](../../src/Renderer2D/CameraComponent2D.cpp), [CameraSystem2D.cpp](../../src/Renderer2D/CameraSystem2D.cpp), [CameraController2D.cpp](../../src/Renderer2D/CameraController2D.cpp) | Demo06, Demo07, Editor |
| OpenGL resources and draw calls | [DrawObject.h](../../src/Graphics/DrawObject.h), [GpuResources](../../src/Graphics/GpuResources), [RenderState.h](../../src/Graphics/RenderState.h), [RenderCommands.cpp](../../src/Graphics/RenderCommands.cpp) | Demo00–Demo04 |
| Post-processing and shader generation | [PostProcessor.cpp](../../src/Graphics/PostProcessor.cpp), [ShaderPreprocessor.cpp](../../src/Graphics/ShaderPreprocessor.cpp) | Demo06 and Demo09 |
| GUI ownership, frames, input capture | [GUISubsystem.cpp](../../src/GUI/GUISubsystem.cpp), [GUIWindow.cpp](../../src/GUI/GUIWindow.cpp), [Widget.cpp](../../src/GUI/Widgets/Widget.cpp) | [PekanUserMessageBox.h](../../src/GUI/PekanUserMessageBox.h) |
| Editor component controls | [EditorScene.cpp](../../src/Editor/EditorScene.cpp), [EntityPropertiesGUIWindow.cpp](../../src/Editor/EntityPropertiesGUIWindow.cpp), [ComponentWidgetsTypes.cpp](../../src/Editor/ComponentWidgetsTypes.cpp) | [EntitiesGUIWindow.cpp](../../src/Editor/EntitiesGUIWindow.cpp) |
| Logging and timing | [PekanLogger.h](../../src/Core/Logger/PekanLogger.h), [Time](../../src/Core/Time) | Logger environment switches and application frame callbacks |

## Architecture details that matter in reviews

### Startup, frames, and shutdown

The include macros in `main()` register subsystem instances before
`application.init()`. The window and OpenGL context are created before subsystem
initialization. `Renderer2DSubsystem` depends on `GraphicsSubsystem`; a subsystem
with no explicit parent depends on Core. Registration order and parent recursion
determine initialization; do not assume a general dependency graph or reverse
shutdown order that the implementation does not provide.

Application initialization then calls `_init()`, `_fillLayerStack()`, and layer
initialization. A layer's parent is an initialization dependency, separate from
its position in the rendering stack.

`PekanApplication::run()` currently does this each frame:

1. Poll GLFW events, then drain the remaining application event queue.
2. Update the viewport and get delta time.
3. Call frame-begin callbacks, then update recurring callbacks.
4. Update all layers, then render all layers in stack order.
5. Call frame-end callbacks, swap buffers, and apply the FPS limit if configured.

Events go through layers in reverse stack order. A handled layer event stops
there. Otherwise, all registered application listeners receive it; only if none
handles it does it reach the queue. Listeners are stored as weak pointers, so the
owner must retain them. `Scene2D` owns its camera controller, which registers as
one of these listeners. `GUIWindow` uses ImGui's capture flags to keep input from
reaching the scene/controller when the GUI consumes it.

Graphics registers automatic clearing through a frame-begin callback. GUI owns
one ImGui frame for the whole application through begin/end callbacks. Creating
an ImGui frame for each window would violate that arrangement.

The existing `main()` functions call `init()` and `run()`; the application
destructor calls `exit()`. Layer cleanup precedes subsystem cleanup and window
destruction. GPU lifetime matters: many GPU wrappers destroy valid resources in
their destructors, while `FrameBuffer` expects an explicit `destroy()`. Check the
individual class before changing ownership or assuming automatic cleanup.

### Scenes, entities, and transforms

`Scene` owns three connected pieces of state: the EnTT registry, an ordered vector
of entity handles, and the next stable EntityID. Creating/destroying entities
through the scene maintains these together. The public registry is for component
access; calling `registry.create()` directly bypasses scene bookkeeping.

Entity mutation methods are deliberately protected. Derived scenes expose only
the operations they need, as in `EditorScene` and `Demo10_Scene`. The serializer
is a friend of `Scene` and uses private helpers for explicit IDs. `clear()` and
`adoptFrom()` operate on scene contents, leaving layer/application state in place.

Stable IDs belong to scene persistence and long-lived selections. Runtime
`TransformComponent2D::parent` is still an `entt::entity` in the same registry.
Serialization translates it to an EntityID; loading is intended to resolve it
back after all entities exist. That conversion does not imply a planned change
to the runtime parent member's type.

World transforms are computed recursively from local translation, rotation, and
scale. Dirty flags and cached world matrices are deferred in TO-DO item 0072.
Do not infer their presence from the old `Transformable2D` journal entries.

`DisabledComponent` is a marker used to filter rendering and camera views. A
system must choose the appropriate view; disabling an entity is not a registry
operation that automatically prevents arbitrary code from accessing it.

### Rendering and cameras

`Scene2D::render()` wraps `RenderSystem2D::render()` and the derived `_render()`
hook in `PostProcessor::beginFrame()` / `endFrame()`. The custom hook runs after
the ECS draw calls. Demo04 deliberately mixes a low-level `DrawObject` with ECS
content; its ordering questions already have TO-DO entries 0097–0099.

Shapes combine a geometry component with `SolidColorMaterialComponent`.
Transforms are optional: the renderer has separate views for transformed and
untransformed shapes and sprites. `LineComponent` is a two-vertex line primitive
with its own color; `LineGeometryComponent` supplies a thick rectangle-shaped
line and needs a material. These are distinct supported paths.

Geometry systems generate positions into vertex attribute views. Convex polygons
use triangle fans; concave polygons use the ear-clipping implementation in
`MathUtils`. Check the geometry system and winding assumptions together when
changing triangulation or transforms.

The current ECS renderer constructs draw resources per entity. The batch renderer
and associated optimizations were deliberately set aside during the ECS migration
(journal entries from 25.10.2025 onward; TO-DO items 0074–0076). Old batch classes
are no longer part of the source tree. The cached camera pointers in the rendering
systems are also a deliberate temporary design; the journal explicitly notes that
this rendering code is not thread-safe.

`CameraSystem2D` finds the first enabled camera with the relevant flag. With no
primary camera it supplies a default camera spanning [-1, 1] in both dimensions;
with no controllable camera it returns null. Camera position/rotation/zoom live
in `CameraComponent2D` itself, independently of `TransformComponent2D`.
`CameraController2D` queries the current registry on input, pans by dragging,
zooms at the cursor, and rotates with Alt + scroll.

Post-processing uses global Graphics resources: optionally an MSAA framebuffer,
then a resolved texture, then a full-screen draw with the chosen fragment shader.
`ShaderPreprocessor` replaces `{{placeholders}}` in `.pkshad` files and writes the
corresponding `.glsl` file. Running Demo06 or Demo09 can therefore write generated
shader files; inspect the diff after a runtime check.

### GUI and Editor

Widgets created with a window register themselves through `shared_from_this()`;
they must be owned by a shared pointer for that path. Widgets created with a null
window can instead be rendered manually. `GUIWindow` renders registered widgets
in insertion order and then calls its `_render()` hook.

Editable widgets expose a user-change flag. Value getters and programmatic setters
clear it. The Editor first pushes user edits to the ECS, then refreshes the widgets
from component data. This ordering was introduced so selecting a camera in the
inspector does not overwrite changes made by the camera controller.

The component name list and index switch in `EditorScene` must agree. A new
editable/serializable component can also require changes in
`ComponentWidgetsTypes`, inspector visibility and synchronization, rendering, and
`Scene2DSerializer`. Follow the requested increment; this list is for checking
completeness within that increment.

The properties window stores an EntityID and resolves it against the registry.
The entities list still tracks a selected list index and translates it to an ID
when updating the properties window. Do not describe the whole list-selection
implementation as already using stable IDs.

The Editor can create entities, add the ten supported component types, edit many
fields, and enable/disable entities. Polygon vertex count is currently read-only;
texture picking, parent editing, name editing, and scene file controls are not
implemented. User-facing messages use `PK_SHOW_ERROR`, `PK_SHOW_WARNING`, and
`PK_SHOW_INFO`; these queue messages for GUI rendering.

## Current `.pksc` work

Read [plan_0006_pkscFileFormat.md](../../notes/plan/plan_0006_pkscFileFormat.md) and the
latest entries in [dev_0006.txt](../../notes/dev/dev_0006.txt) before continuing this work.
The plan contains both implemented contracts and future steps.

As of 15.11.2026:

- The JSON dependency, stable IDs, scene state helpers, generic serializer, and
  all ten component serializers/deserializers exist.
- `SceneSerializer` handles common metadata and entities; Core has no knowledge
  of Renderer2D component types. Loading creates all entities and builds the name
  map before deserializing components, so forward name references can resolve.
- Duplicate non-empty names map to `INVALID_ENTITY_ID` as an ambiguity marker.
  Duplicate names alone are allowed; using an ambiguous name reference fails.
  Missing/null names mean unnamed, while empty strings are invalid.
- `nlohmann::ordered_json`, a fixed component order, scene entity-vector order,
  and four-space indentation provide deterministic output. GLM vector adapters
  in `SerializationUtils.h` specifically support `ordered_json`; component field
  validation happens in the component deserializers, not in those adapters.
- Component helpers distinguish missing components, valid components, and invalid
  components. Missing fields retain struct defaults. Present values are checked
  for type, vector shape, and numeric representability. Semantic invariants such
  as valid dimensions/segment counts remain TO-DO item 0125.
- Unknown top-level scene fields, component types, and fields inside known
  components warn and are skipped. Do not assume every nesting level has the
  same validation merely because the plan describes a general policy.
- Loading mutates its destination directly; failure can leave partial contents.
  Transactional loading belongs in the future Editor integration using a staging
  scene and `adoptFrom()`, not in a speculative rewrite of the base serializer.
- Parent IDs are carried in the internal
  `UnresolvedTransformParentComponent2D_`. `postDeserialize()` is still empty, so
  loaded parent links are not yet restored. Resolution, missing/self-parent
  handling, and cycle breaking are the remaining planned work in Step 4.
- Sprite `texturePath` is written as null. Loading a string currently warns and
  ignores it; actual texture loading is deferred (TO-DO item 0127).
- Editor Save/Load and name-editing UI (Step 5), followed by the planned round-trip
  and edge-case checks (Step 6), remain pending.

The recent commits include numeric float-range validation (`18a0fd0`) and plan
clarifications (`37fa11c`). Read their actual diffs when reviewing them; their
temporary subject lines do not explain the scope.

## Demo map

| Demo | What the current source exercises |
| --- | --- |
| Demo00 | Basic indexed draw object, shader uniform updates, GUI position/background controls |
| Demo01 | Multiple squares, dynamic vertex data and subdata, blending, GUI controls |
| Demo02 | Low-level 3D cube, model/view/projection matrices, depth testing and face culling; not a Renderer3D subsystem |
| Demo03 | Snake game, keyboard input, frame-based movement, application without GUI |
| Demo04 | Custom shaders/textures and a low-level draw object mixed with ECS shapes |
| Demo05 | Mandelbrot times-table patterns comparing line primitives with thick line geometry |
| Demo06 | Many animated shapes, enabled/disabled entities, camera switching, post-processing |
| Demo07 | Shape transforms controlled through GUI, coordinate grid, camera conversion/control |
| Demo08 | Many textured sprites and frame-by-frame texture animation |
| Demo09 | ECS geometry/material/sprite paths with and without transforms, camera input, post-processing |
| Demo10 | Articulated player composed of parented transforms, shapes, and sprites |

Demo00–Demo03 inherit directly from `Layer`; Demo04–Demo10 use `Scene2D`.
Their historical names and the README should not be used to infer their current
inheritance or rendering implementation.

## Build and validation

`Core` is declared in the root [CMakeLists.txt](../../CMakeLists.txt). Graphics,
Renderer2D, and GUI are separate static libraries; Editor is an executable.
Renderer2D publicly links Graphics, which publicly links Core. GUI publicly links
Core and privately links ImGui. Core publicly exposes GLM, JSON, and the EnTT
include directory. Graphics explicitly links `opengl32.lib`, so the checked-in
build is Windows-oriented.

GLM and EnTT are Git submodules; the remaining dependencies are vendored. Inspect
the checked-in dependency APIs when needed and preserve local integration changes.
Configuring a fresh checkout requires initialized submodules.

From the repository root, `.\gen.bat` generates the Visual Studio build and a
separate Ninja compilation database under `build/build-ninja` for `.clangd`.
`gen.bat --clean` currently removes all of `build`, including the Ninja directory;
changing that layout is already TO-DO item 0121. Avoid regenerating or cleaning
an existing build merely to inspect the project.

For an already generated Visual Studio build, CMake can be found without changing
PATH:

```powershell
$pekanVsPath = & "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe" -latest -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath
$pekanCmake = Join-Path $pekanVsPath 'Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe'
& $pekanCmake --build build --config Debug --parallel 1 -- /nr:false
```

Omitting `--target` builds the solution's default targets, including demos when
`WITH_DEMO_PROJECTS=ON`. Add `--target Renderer2D` or `--target Editor` for a
focused build. Use `--config Release` for Release. The exact configured CMake
path and generator can also be read from `build/CMakeCache.txt`.

The environment checked on 15.11.2026 uses Visual Studio 18 2026 and a `Pekan.slnx` solution;
do not hardcode that toolchain path for other checkouts. `Window.cpp` requests an
OpenGL 4.3 core context, even though the shader sources commonly use GLSL 330.

Demo resources use working-directory-relative paths. Visual Studio configures
each demo's source directory as its working directory; post-build copies also
allow running from the executable directory. Engine shader paths instead use
compile-time source-directory macros such as `PEKAN_RENDERER2D_ROOT_DIR` and
`PEKAN_GRAPHICS_ROOT_DIR`. A successful build is not a visual or interaction test.
No first-party automated test target or CTest registration was found; the demos
and Editor are the existing integration exercises.

Build validation on 15.11.2026:

- The full Release build passed: Core, Graphics, Renderer2D, GUI, Editor, and
  Demo00–Demo10. No application was launched for a visual or interaction check.
- The Debug build attempt failed with MSVC C1902, "Program database manager
  mismatch", while compiling Core. The cause was not diagnosed or repaired as
  during that check; this is not a successful Debug validation.
- Root-wide Git status/diff commands encountered a permission error when reading
  EnTT's submodule metadata in this session. File-scoped diffs and
  `git status --short --ignore-submodules=all` worked. The latter reports the root
  repository only and does not establish that submodules are clean.

## Reading the development history

- [general_0000.txt](../../notes/general/general_0000.txt): initial motivation behind Pekan, including the importance of the author's own architecture and learning.
- [plan_0005.txt](../../notes/plan/plan_0005.txt): v0.2 architecture and Gleam House
  context. It describes old OO rendering classes that have since been removed.
- [plan_0006.txt](../../notes/plan/plan_0006.txt): the broader v0.3 feature list. ECS and
  scene files come before later ideas such as layer transitions, collisions,
  lighting, and a debug overlay; this is not authorization to build them.
- [dev_0004.txt](../../notes/dev/dev_0004.txt) and
  [dev_0005.txt](../../notes/dev/dev_0005.txt): events/layers, the earlier renderer,
  post-processing, and the original transform hierarchy work.
- [dev_0006.txt](../../notes/dev/dev_0006.txt): ECS transition, removal of the old
  rendering classes, Editor development, protected entity APIs, and scene files.
  Later entries can supersede earlier behavior within the same file.
- [todo.txt](../../notes/dev/todo.txt): deliberate deferrals and small follow-up tasks.
  Check whether an issue already has a number before adding another entry.

Keep future research notes separate from the author's journal. Update this guide
only when a confirmed change makes it materially more useful; it should not grow
into a transcript of every session.

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

- `notes/dev/dev_NNNN.txt` — chronological **dev journal**. The latest file (`dev_0006.txt` at the time of writing) is actively appended to. **Most commits add a corresponding journal entry.** Never rewrite old entries; only append. Do not start a new `dev_NNNN.txt` unless the author asks.
- `notes/dev/todo.txt` — a numbered task list. Format: `NNNN DD.MM.YYYY (DONE): description` or `(TODO)` while open. Continuation lines are indented. Tasks are referenced from the journal as **"TO-DO task 0079"** / **"TO-DO item 0071"** (always "TO-DO", not "TODO"). Entries can be tasks, questions, or "think about X".
- `notes/plan/` — design docs for larger features. Some are short plain-text lists (`plan_0000.txt`), some are structured specs (`plan_0006_pkscFileFormat.md`). Read the relevant plan before implementing a planned feature. **Do not write plan docs in journal voice**, and **do not write journal entries like a plan/spec**.
- `notes/general/` — rare high-level notes (e.g. why the project exists). Leave these alone unless asked.

If you do any non-trivial work, record it in the current journal: what was done and why, the thought process, reasoning behind design decisions, even low-level ones. Stay focused on the task at hand. If you spot an unrelated bug or an opportunity for improvement, note it down in `todo.txt` rather than fixing it as part of the current change.

When asked to write a journal entry, **read the last stretch of the current `dev_NNNN.txt` first** and continue in that voice. The rest of this section is how that voice works.

### Journal mechanics

Date headers look like this, with a blank line after:

```
----------
24.08.2026
----------
```

Use `DD.MM.YYYY`. A day's work is usually wrapped in `{ ... }` with the braces on their own lines.

- **New task, new `{`.** Several `{` blocks can sit under one date (separate tasks that same day).
- **Same task, same `{`.** If work continues across days, put the next date header *inside* the still-open block. Do not open a second `{` for the same thought. Close the block when that task/thought is actually finished.
- `{` is usually right after the date. A continuation date inside an already-open block has no extra `{`.
- A date can also have a one-liner outside braces (rare: "Started developing Gleam House...").
- Tiny work can be tiny: "Nothing to say here. Done." is a valid whole entry.

The journal is **plain text, not Markdown.** No `#` headings, no `**bold**`, no \`backticks\`, no fenced code blocks, no tables.

Put names, paths, signatures, and snippets on their own indented line (4 spaces). Short names can stay in the sentence. Multi-line C++ is indented the same way, as real code, not as a fenced listing:

```
Let's create a new function in PekanApplication:
    void registerRecurringCallback(std::function<void()> callback, float interval);
```

Wrap lines around a natural phrase boundary (~80–100 characters, not strict). Blank lines separate thoughts, not every sentence.

Named asides use `--begin LABEL` / `--end LABEL` (e.g. `SIDE NOTE`). Longer recaps sometimes use `====begin SUM UP ... ====end SUM UP`. In-progress placeholders exist: `... class PekanEngine, in progress ...`. You will almost never need those unless the author is mid-thought and stopping for the day.

### Voice

Write **as the author, first person**, as if he typed it that evening. The journal is him talking to himself / his future self — not a commit message, not a PR, not an agent report.

- Mix **"I"** and **"we"**. "I" = a personal choice or what he did that day. "We" = Pekan / the code / the current work. Never "the agent", "we implemented" as a team of AIs, or "this commit".
- Present tense while working through the thought ("Let's create...", "Now we can..."). Past tense for what already happened ("I copied...", "It worked!").
- When introducing the problem, describe the code's **current, pre-change state in the present tense**, because that is what the author is looking at in that moment. Write "unknown fields are silently ignored", not "unknown fields were being silently ignored". The past tense can make it sound like the problem existed only in some earlier revision. Switch to past tense only after the entry has narrated the change, or when referring to something that genuinely happened earlier.
- Casual, spoken, slightly messy English. Contractions are normal (`let's`, `that's`, `don't`, `I'll`). Fillers he actually uses: "Okay,", "So,", "Now,", "Next,", "Alright,". Closers he actually uses: **"That's it."**, **"Done."**, **"That's about it."**, **"Okay, it works!"**, **"Looks good."**, **"Done. Easy."**
- Occasional real personality is fine when it fits the moment: "which is stupid tbh", "it's a bit annoying", "Neat!", "Pretty nice.", "gotta admit", "I feel like...". Do not force slang into every paragraph, and do not invent a jokey narrator.
- Ordinary words for reasoning: "the idea is", "the bottomline is", "pretty straightforward", "a bit weird", "that's not good enough", "this makes no sense". **Do not** use blog/AI diction: "leverage", "robust", "canonical", "idiomatic", "this ensures", "in order to facilitate", "separation of concerns", "hydration", "pipeline", "going forward".
- Technical terms are the real C++ / OpenGL / Pekan names (`emplace`, `entt::entity`, `texel`, `EntityID`). Don't invent extra jargon, and don't explain what ECS or JSON is — he already knows.
- Honest about uncertainty: "I guess we'll see", "Can't really decide", "Idk", "there are quite a few details that I'm missing but they are not that important."
- He asks himself questions and answers them in the next line: "Why?", "How will it handle it?", "What should these functions do?", "Is this an okay behavior?" That question/answer rhythm is the reasoning, not decoration.

### What an entry actually contains

An entry is a **walkthrough of the thought**, not a summary of the diff.

Typical shape:

1. **Where we are / what we're doing** — "Let's implement `deserialize()`.", "Let's look at TO-DO task 0079.", "Time to move on to step 4 of the .pksc plan."
2. **Why, if it isn't obvious** — "Why do we need this?", the problem with the current code, why the plan's wording was unclear.
3. **The approach**, often by talking through options. If there are two ways, say both and pick one with a reason ("I will go with option 1 because I feel like it will be better for our future architecture."). Mention temporary/PoC work when that's what it is, and that it will be done properly later.
4. **The actual change**, at the level of new files, class/function names, member variables, and the important `if`s. Show the snippet. Explain why that check exists, not only that it exists.
5. **Close** — "That's it." Then whether it works / what was tested. "Done." If this finishes a larger stretch, say what comes next ("Now we can proceed with implementing `deserializeComponents()`.").

Match **length to the size of the thought**:

- One-line CMake / rename / obvious cleanup → a few sentences.
- A new function or a small restructure → a short walkthrough (problem → what we changed → the key snippet → done).
- A new system (events, post-processing, `deserialize()`, a gnarly optimization) → long. Numbered steps, helper by helper, the exact loop, the edge case. This is normal. Do not compress a design-heavy day into a bullet summary.

Skip details he would skip: surrounding architecture that hasn't changed, and "those details are not important, and quite straightforward, so leaving it at that." Do **not** skip the decision that was actually made, even if it looks small (why `enabled` defaults to true, why the map is filled before the second loop, why a function is `protected` not `public`).

He often records a **failed or rejected approach** first, then the one that landed. Keep that when it happened. He also notes leftover work as a new TO-DO item instead of doing it now.

Trivial tasks still get an entry if they were a TO-DO, but it can be "Nothing to say here. Done."

### How this sounds vs how it must not sound

He writes like this (from `dev_0006.txt`, 16.08.2026):

```
There were a few places where we had code like this:
    json sceneData;
Default-constructing a JSON object and then filling its properties with data like this:
    sceneData["sceneType"] = getSceneType();
This works technically, but what happens behind the scenes is that default-constructing a JSON object
actually constructs it as null. Then at the moment of the first property assignment that's when
it becomes non-null. This is a bit weird and introduces a window where the JSON object is null,
which might be dangerous, so we better construct our JSON objects at the moment of declaration properly:
    json sceneData = json::object();
Did this change in a few places.
```

Not like this:

```
## Summary
Initialized JSON objects with `json::object()` instead of default construction
to avoid a transient null state. This is more robust and prevents subtle bugs.
```

The first one is a person thinking. The second one is a changelog. Always write the first kind.

### Don't

- Don't rewrite or restyle old journal text.
- Don't open with "This commit...", "Changes:", "Summary:", or a bullet dump of files touched.
- Don't use Markdown inside `dev_*.txt` / `todo.txt`.
- Don't explain Pekan, ECS, or C++ basics.
- Don't add "best practice" tips, future-proofing lectures, or extra advice he didn't think.
- Don't be stiff, polished, or literary. Don't invent metaphors.
- Don't mimic typos on purpose (the journals have some; write cleanly in the same register).
- Don't copy the tone of `plan_0006_pkscFileFormat.md` into the journal. Plans are specs. The journal is a session diary.

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
- Journal entries (when you write them) go at the end of the latest `notes/dev/dev_NNNN.txt`, in the author's first-person voice — see §8. Read the end of that file before writing.

---

## 11. Working with the author

- Development proceeds in deliberately small, independently verifiable commits. Treat the requested increment as a hard scope boundary; do not implement later plan points, adjacent refactors, or extra cleanup unless explicitly asked.
- When asked only for scaffolding, signatures, or TODO markers, make the smallest compile-safe change and leave the actual implementation to the author.
- Discuss and get confirmation before changing an agreed architecture, API, or implementation plan beyond what the current request requires.
- Preserve the author's existing comments and prose. Only extend them or minimally edit wording that became inaccurate; do not replace their structure or rewrite them in a different voice.
- Prefer the simplest solution needed by Pekan today. Avoid generic abstractions, extra template flexibility, or additional data structures unless they solve a concrete current requirement.

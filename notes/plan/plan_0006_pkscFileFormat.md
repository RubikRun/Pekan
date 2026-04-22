# Pekan Scene File Format (.pksc)

## Overview

The `.pksc` (Pekan Scene) format is a JSON-based file format for saving and loading Pekan scenes. It captures an ECS snapshot - what entities exist, which components each entity has, and all component data.

**Goals:**

- Human-readable and hand-editable (plain JSON, meaningful field names, 4-space indentation)
- Versioned (`formatVersion` field for future migration)
- Support for different scene types (`sceneType` field — `"scene2d"` now, `"scene3d"` later, etc.)
- Stable entity identity via a Pekan-specific EntityID, not entt internals

---

## EntityID

`entt::entity` values are internal handles — they get recycled, don't survive save/load, and are meaningless outside their registry. We need a stable identifier that persists in files and is usable for cross-entity references (e.g., parent-child relationships).

**EntityID type** — a typed alias in `src/Core/Entity/EntityID.h`:

```cpp
namespace Pekan
{
    using EntityID = uint32_t;
    constexpr EntityID INVALID_ENTITY_ID = 0;
}
```

Use `EntityID` (not `uint32_t`) everywhere so the underlying type can change later without touching call sites.

**Two separate components in Core** — `id` is infrastructure, `name` is user-facing; they have different lifetimes and access patterns:

```cpp
// Core/Entity/EntityIDComponent.h
struct EntityIDComponent
{
    EntityID id = INVALID_ENTITY_ID;
};

// Core/Entity/NameComponent.h
struct NameComponent
{
    std::string name;
};
```

`EntityIDComponent` is attached automatically to every entity. `NameComponent` is optional — only attached when an entity has a non-empty name. Names have no engine-side behavior, don't need to be unique, and exist purely for readability in `.pksc` files and in the Editor.

`Scene` gains a counter member `EntityID m_nextEntityId = 1`. Every call to `createEntity()` emplaces `EntityIDComponent` with the current counter value and increments it. `INVALID_ENTITY_ID` is never assigned.

`Scene` also gets a **private** `createEntity(EntityID entityId)` overload for deserialization. This overload emplaces `EntityIDComponent` with the given ID (instead of auto-incrementing). During deserialization, `SceneSerializer` sets `m_nextEntityId` via a private `setNextEntityId()` helper.

`createEntity()` and `disableEntity()` remain **protected**. Deserialization-only helpers (`createEntity(EntityID)` and `setNextEntityId(EntityID)`) are **private**. `Scene` declares `SceneSerializer` as a friend class so the serializer can call these scene-management methods without exposing entity-mutation methods to arbitrary callers.

---

## File Format Specification

### Top-level structure

```json
{
    "formatVersion": { "major": 1, "minor": 0 },
    "sceneType": "scene2d",
    "settings": { },
    "entities": [ ... ]
}
```

- `formatVersion` — `{ "major": int, "minor": int }`, starts at `{1, 0}`.
  - **Major** bump = breaking change. Loader refuses if file's major > supported.
  - **Minor** bump = additive change (e.g. new optional fields/components). Older engines log warnings on unknowns and still load.
- `sceneType` — string identifying the scene type. Determines which component types are valid. Currently support only `"scene2d"`.
- `settings` — optional object for scene-global state (e.g. clear color, physics gravity, post-processing stack). Keys are scene-type-specific. Missing or empty is valid. No settings are defined for `"scene2d"` in the MVP — the field exists to reserve the slot.
- `entities` — JSON array. Each element is one entity. Order does not matter — entities are identified by their `id`, not by index.

### Entity structure

```json
{
    "id": 1,
    "name": "red_rectangle",
    "enabled": true,
    "components": {
        "Transform2D": { ... },
        "RectangleGeometry": { ... },
        "SolidColorMaterial": { ... }
    }
}
```

- `id` — the entity's EntityID. Must be unique within the file. Must be > 0.
- `name` — optional string. When non-empty, a `NameComponent` is attached to the entity. Omitted or empty means no `NameComponent`. Not required to be unique.
- `enabled` — optional, defaults to `true`. When `false`, the entity gets a `DisabledComponent`.
- `components` — object whose keys are component type names. Which keys are valid depends on `sceneType`.

### Entity references

When a component references another entity (e.g., `TransformComponent2D::parent`), the value may be:

- a number — the target's `EntityID`
- a string — the target's `name` (resolved in a pre-pass to exactly one `EntityID`; if not found or ambiguous due to duplicate names, that's an error and load fails)
- `null` — no entity

```json
"parent": 1               // by ID — what the Editor always writes
"parent": "main_camera"   // by name — convenience for hand-authoring
"parent": null
```

The Editor always writes numeric IDs on save. Name references exist purely as an authoring convenience. On deserialization, a lookup table (EntityID → entt::entity) is built and used to resolve these references.

### Scene2D component mappings

The 10 component types valid for `"scene2d"`, matching the Editor's current component list:


| JSON key             | C++ type                      | JSON fields                                                                                                     |
| -------------------- | ----------------------------- | --------------------------------------------------------------------------------------------------------------- |
| `Transform2D`        | `TransformComponent2D`        | `position` [x,y], `rotation`, `scaleFactor` [x,y], `parent` (EntityID, name, or null)                           |
| `Sprite`             | `SpriteComponent`             | `width`, `height`, `texturePath` (string or null), `textureCoordinatesMin` [x,y], `textureCoordinatesMax` [x,y] |
| `RectangleGeometry`  | `RectangleGeometryComponent`  | `width`, `height`                                                                                               |
| `CircleGeometry`     | `CircleGeometryComponent`     | `radius`, `segmentsCount`                                                                                       |
| `TriangleGeometry`   | `TriangleGeometryComponent`   | `pointA` [x,y], `pointB` [x,y], `pointC` [x,y]                                                                  |
| `PolygonGeometry`    | `PolygonGeometryComponent`    | `vertexPositions` (array of [x,y])                                                                              |
| `LineGeometry`       | `LineGeometryComponent`       | `pointA` [x,y], `pointB` [x,y], `thickness`                                                                     |
| `SolidColorMaterial` | `SolidColorMaterialComponent` | `color` [r,g,b,a]                                                                                               |
| `Line`               | `LineComponent`               | `pointA` [x,y], `pointB` [x,y], `color` [r,g,b,a]                                                               |
| `Camera2D`           | `CameraComponent2D`           | `size` [x,y], `position` [x,y], `rotation`, `zoomLevel`, `isPrimary`, `isControllable`                          |


**Conventions:**

- `glm::vec2` → JSON array `[x, y]`
- `glm::vec4` / color → JSON array `[r, g, b, a]`

**Sprite textures:** We want an MVP for now so `texturePath` will always be `null` because the Editor doesn't have a texture picker and `Texture2D` doesn't store its source path. Real texture support will come later. Dimensions and UVs can be saved and loaded normally.

### Full example

```json
{
    "formatVersion": { "major": 1, "minor": 0 },
    "sceneType": "scene2d",
    "settings": {},
    "entities": [
        {
            "id": 1,
            "name": "red_rectangle",
            "components": {
                "Transform2D": {
                    "position": [0.0, 5.0],
                    "rotation": 0.0,
                    "scaleFactor": [1.0, 1.0],
                    "parent": null
                },
                "RectangleGeometry": {
                    "width": 2.0,
                    "height": 1.0
                },
                "SolidColorMaterial": {
                    "color": [1.0, 0.0, 0.0, 1.0]
                }
            }
        },
        {
            "id": 2,
            "name": "blue_circle",
            "enabled": false,
            "components": {
                "Transform2D": {
                    "position": [3.0, 2.0],
                    "rotation": 0.0,
                    "scaleFactor": [0.5, 0.5],
                    "parent": 1
                },
                "CircleGeometry": {
                    "radius": 0.5,
                    "segmentsCount": 32
                },
                "SolidColorMaterial": {
                    "color": [0.2, 0.6, 1.0, 1.0]
                }
            }
        },
        {
            "id": 3,
            "name": "main_camera",
            "components": {
                "Camera2D": {
                    "size": [16.0, 9.0],
                    "position": [0.0, 0.0],
                    "rotation": 0.0,
                    "zoomLevel": 1.0,
                    "isPrimary": true,
                    "isControllable": true
                }
            }
        }
    ]
}
```

---

## Serializer Architecture

Two layers — a generic base and scene-type-specific subclasses:

**SceneSerializer (base class, in Core)**
Handles everything common to all scene types:

- Reading/writing the top structure (`formatVersion`, `sceneType`, `entities`)
- Creating entities via `scene.createEntity(entityId)`, disabling via `scene.disableEntity()`
- Reading entity data (`id`, enabled status) through `scene.getRegistry()` and `scene.getEntities()`
- Delegating component (de)serialization to virtual hooks

`Scene` declares `SceneSerializer` as a friend so the serializer can use non-public scene-management methods. It does not modify internal data like `m_entities` or `m_nextEntityId` directly. Component data is handled through `scene.getRegistry()`, which is normal ECS usage.

Lives in Core alongside `Scene`. Core gains the JSON library as a dependency. Has no knowledge of Renderer2D.

**Scene2DSerializer (subclass, in Renderer2D)**
Handles `"scene2d"` specifics:

- (De)serializes all 10 Renderer2D component types
- Resolves Transform2D parent references in a post-deserialization pass (two-pass: create all entities first, then resolve EntityID → entt::entity for parent fields)

This extends naturally — a future `Scene3DSerializer` goes in a Renderer3D module with its own component types. The base format and Editor infrastructure stay the same.

**Error handling on load:**

- Unknown `sceneType` → error, refuse to load
- Unknown component keys → warning, skip
- Invalid parent EntityID (numeric not found / self-reference) → warning, set to null.
- Invalid parent name reference (not found / ambiguous because of duplicate names) → error, refuse to load.
- Parent cycle → detected via a single DFS over the parent graph in `postDeserialize()` (visited + in-stack sets). Break the cycle by setting the offending entity's parent to null and log the chain (e.g. `"cycle: 7 → 12 → 4 → 7"`).
- Missing component fields → use C++ struct defaults
- `formatVersion.major` higher than supported → error, refuse to load. `formatVersion.minor` higher than supported → load normally (unknown components/fields already warn and skip).
- Duplicate EntityIDs → error. Do not silently generate replacement IDs, because references to that duplicated ID would become ambiguous.

---

## Development Plan

### Step 1 — Add nlohmann/json dependency

Download `json.hpp` from the nlohmann/json GitHub release into `dep/json/json.hpp`. Create a `dep/json/CMakeLists.txt` with an INTERFACE library target. Wire it in the root `CMakeLists.txt` with `add_subdirectory(dep/json)`. This matches how other deps (glfw, glad, glm, imgui) are already vendored.

### Step 2 — EntityID types + Scene changes

Create:

- `src/Core/Entity/EntityID.h` — `using EntityID = uint32_t;` and `constexpr EntityID INVALID_ENTITY_ID = 0;`
- `src/Core/Entity/EntityIDComponent.h` — `struct EntityIDComponent { EntityID id = INVALID_ENTITY_ID; };`
- `src/Core/Entity/NameComponent.h` — `struct NameComponent { std::string name; };`

Modify `Scene`:

- Add `EntityID m_nextEntityId = 1` member
- In `createEntity()`, emplace `EntityIDComponent{m_nextEntityId++}` on the new entity
- Add a private `createEntity(EntityID entityId)` overload that emplaces `EntityIDComponent` with the given ID and appends to `m_entities` — but does **not** touch `m_nextEntityId`. Used by deserialization.
- Add `friend class SceneSerializer;` so the serializer can call private/protected scene-management methods (`createEntity(EntityID)`, `setNextEntityId(EntityID)`, `createEntity()`, `disableEntity()`) without making them public.
- Add a protected `clear()` method — clears the registry, clears `m_entities`, and resets `m_nextEntityId = 1`. Single entry point for wiping a scene.
- Add a private `setNextEntityId(EntityID id)` method — used only by `SceneSerializer` after deserialization to resume the counter from `max(loaded IDs) + 1`.
- Add a protected `adoptFrom(Scene&& other)` method that move-assigns `other.m_registry`, `other.m_entities`, and `other.m_nextEntityId` into `*this`. Used by transactional load in Step 5.
- Update the Editor to use `EntityID` (not raw `entt::entity`) for any persistent selection state, and display `EntityIDComponent::id` + `NameComponent::name` (when present) in the entity list.

After this step, every entity automatically has a stable ID. The Editor works exactly as before.

### Step 3 — SceneSerializer base class (in Core)

Create `src/Core/SceneSerializer.h` and `.cpp`. Add them to Core's source list in the root `CMakeLists.txt` and link the JSON library to Core.

The base class must provide:

- `serialize(const Scene&)` → JSON string. Builds the top-level structure (including `formatVersion` as `{ "major", "minor" }` and an empty `settings` object). Iterates `scene.getEntities()`, reads `EntityIDComponent`, `NameComponent` (when present), and `DisabledComponent` from `scene.getRegistry()`, calls virtual `serializeComponents(scene.getRegistry(), entity)` for each entity. Writes `name` only when a `NameComponent` is present; omits `enabled` when true (default).
- `deserialize(Scene&, json)` → populates scene. Parses JSON, validates `sceneType` and `formatVersion.major` (refuses on major > supported; accepts higher minor with a warning). For each entity: calls `scene.createEntity(entityId)`, emplaces `NameComponent` if `"name"` is present and non-empty, calls `scene.disableEntity()` if `enabled` is false, calls virtual `deserializeComponents(scene.getRegistry(), entity)`. After all entities are created, calls virtual `postDeserialize(scene)`, then `scene.setNextEntityId(maxLoadedEntityId + 1)`. Logs errors and warnings directly via `PK_LOG_ERROR` / `PK_LOG_WARNING`. Returns `bool` — `false` on fatal errors (unknown sceneType, unsupported major, duplicate EntityIDs, malformed JSON, invalid parent name references), `true` on success.
- Pure virtual hooks: `getSceneType()`, `serializeComponents(registry, entity)`, `deserializeComponents(registry, entity, json)`. Optional virtual: `postDeserialize(scene)`.

### Step 4 — Scene2DSerializer

Create `src/Renderer2D/Scene2DSerializer.h` and `.cpp`. Add them to `src/Renderer2D/CMakeLists.txt`. Renderer2D already depends on Core (through Graphics), so `SceneSerializer` and the JSON library are available — no extra link needed.

Implement:

- `serializeComponents(registry, entity)` — for each of the 10 types (in the fixed order from the component mappings table), call `registry.try_get<T>(entity)`, write JSON if present. For Transform2D parent, look up the parent entity's `EntityIDComponent` to get the ID. Entities are serialized in `scene.getEntities()` vector order. This fixed ordering for both entities and component keys ensures deterministic output — re-saving an unchanged scene produces identical JSON.
- Before component deserialization, build a `name → EntityID` map from the file. If duplicate non-empty names exist and any component uses string-based references, treat that as an error and fail load.
- `deserializeComponents(registry, entity, json)` — for each key in the `components` object, default-construct the component, override fields from JSON, emplace on entity. For Transform2D, accept `parent` as number, string (resolved via the name map), or null; store the raw parent EntityID temporarily (set the component's `parent` to `entt::null`).
- `postDeserialize(scene)` — build an EntityID→entt::entity map from `scene.getEntities()` + `scene.getRegistry()`, iterate all `TransformComponent2D` instances and resolve parent EntityIDs to actual `entt::entity` handles. Then run a single DFS over the parent graph (visited + in-stack sets); on cycle, break it by nulling the offending parent and log the chain.

### Step 5 — Editor integration

**EditorScene** — add two methods:

- `saveToFile(path)` — create `Scene2DSerializer`, call `serialize(*this)`, write the result via `FileUtils::writeStringToTextFile()`.
- `loadFromFile(path)` — **transactional**: the live scene is never mutated until the file is fully parsed.
  1. Read the file via `FileUtils::readTextFileToString()`.
  2. Construct a staging `EditorScene` locally.
  3. Call `deserialize(staging, json)`. On `false`, log and return — the live scene is untouched.
  4. On success: call `this->clear()`, then `this->adoptFrom(std::move(staging))`.

**Entity selection** uses `EntityID`, not `entt::entity`. Any stored selection must be an `EntityID`; `entt::entity` is resolved from the current registry each frame. This removes the dangling-handle footgun across clear/load boundaries.

**EntitiesGUIWindow** — add a text input for the file path and Save/Load buttons. Stores the current selection as `EntityID`. The entity list displays each entity's name when present (e.g. `"red_rectangle (1)"`), falling back to just the ID (e.g. `"Entity 1"`) for unnamed entities.

**EntityPropertiesGUIWindow** — shows the selected entity's `EntityID` and a text input for editing its name (adds/removes `NameComponent` as the string becomes non-empty/empty).

**CameraController2D** — if it caches entity handles to camera entities, it must re-find the primary camera after a successful load. Hook this off the successful-load path in `EditorScene::loadFromFile`.

### Step 6 — Test and polish

- Build a scene in the Editor → save → inspect the `.pksc` file
- Hand-edit the `.pksc` file → load it → verify it looks right
- Test edge cases: empty scene, disabled entities, parent-child chains, missing component fields
- Re-save a loaded scene and compare output — should be identical

---

## Files Summary

**New:**

- `dep/json/json.hpp` + `CMakeLists.txt`
- `src/Core/Entity/EntityID.h`
- `src/Core/Entity/EntityIDComponent.h`
- `src/Core/Entity/NameComponent.h`
- `src/Core/SceneSerializer.h` + `SceneSerializer.cpp`
- `src/Renderer2D/Scene2DSerializer.h` + `Scene2DSerializer.cpp`

**Modified:**

- `CMakeLists.txt` (root) — add dep/json subdir, add Core sources (`EntityID.h`, `EntityIDComponent.h`, `NameComponent.h`, `SceneSerializer.h`/`.cpp`), link JSON library to Core
- `src/Core/Scene.h` / `.cpp` — add `m_nextEntityId`, emplace `EntityIDComponent` in `createEntity()`, add private `createEntity(EntityID)` overload, add private `setNextEntityId(EntityID)`, add `friend class SceneSerializer`, add protected `clear()` and `adoptFrom(Scene&&)`
- `src/Renderer2D/CMakeLists.txt` — add Scene2DSerializer sources
- `src/Editor/EditorScene.h` / `.cpp` — add `saveToFile()` and transactional `loadFromFile()`
- `src/Editor/EntitiesGUIWindow.h` / `.cpp` — add Save/Load UI; store selection as `EntityID`; show names (with ID fallback) in the entity list
- `src/Editor/EntityPropertiesGUIWindow.h` / `.cpp` — show the selected `EntityID` and add a text input for editing the name (adds/removes `NameComponent`)

---

## Future Extensions (out of scope)

- Sprite texture path tracking and loading
- Native OS file dialogs
- Scene metadata (name, author, etc.)
- Serializer registry/factory for runtime scene type dispatch
- Undo/redo integration
- Harden the JSON boundary — wrap all `nlohmann::json` access in try/catch, use `value()`/`contains()` instead of `operator[]` for reads, configure float precision for round-trip stability, enable `ignore_comments`, enforce UTF-8-no-BOM + LF + trailing newline on write, and keep `nlohmann::json` out of public headers (pimpl or opaque `Writer`/`Reader` interface) so a binary transport can be added later.
- Component registration system — each component module registers its JSON key, C++ type, and (de)serialize functions with a global registry; `Scene2DSerializer` iterates the registry in registration order instead of hardcoding the 10 types. Adding a component becomes a one-file change colocated with the component itself.


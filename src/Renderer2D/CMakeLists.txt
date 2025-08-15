# Create project Renderer2D
project(Renderer2D)

option(PEKAN_USE_1D_TEXTURE_FOR_2D_SHAPES_BATCH
    "Use a 1D texture to hold the colors of different shapes in a 2D shapes batch. Otherwise, a vertex attribute will be used. Using a 1D texture is more stable, it's less likely to crash or work incorrectly, while a vertex attribute is generally faster, especially on newer GPUs."
    ON
)
option(PEKAN_ENABLE_2D_SHAPES_ORIENTATION_CHECKING "Enable orientation checking for 2D shapes" OFF)

# Add a static library Renderer2D, compiling the following source files
add_library(Renderer2D STATIC
    Renderer2DSystem.h
    Renderer2DSystem.cpp
    RenderBatch2D.h
    RenderBatch2D.cpp
    Camera2D.h
    Camera2D.cpp
    Line.h
    Line.cpp
    Transformable2D.h
    Transformable2D.cpp
    Vertex2D.h
    Shapes/Shape.h
    Shapes/Shape.cpp
    Shapes/TriangleShape.h
    Shapes/TriangleShape.cpp
    Shapes/RectangleShape.h
    Shapes/RectangleShape.cpp
    Shapes/CircleShape.h
    Shapes/CircleShape.cpp
    Shapes/CircleShapeStatic.h
    Shapes/CircleShapeStatic_impl.h
    Shapes/PolygonShape.h
    Shapes/PolygonShape.cpp
    Shapes/LineShape.h
    Shapes/LineShape.cpp
    Sprite/Sprite.h
    Sprite/Sprite.cpp
)

# Group Shapes files under a virtual folder called "Shapes"
SOURCE_GROUP("Source Files\\Shapes" FILES
    Shapes/Shape.cpp
    Shapes/TriangleShape.cpp
    Shapes/RectangleShape.cpp
    Shapes/LineShape.cpp
    Shapes/CircleShape.cpp
    Shapes/PolygonShape.cpp
)
SOURCE_GROUP("Header Files\\Shapes" FILES
    Shapes/Shape.h
    Shapes/TriangleShape.h
    Shapes/RectangleShape.h
    Shapes/LineShape.h
    Shapes/CircleShape.h
    Shapes/CircleShapeStatic.h
    Shapes/CircleShapeStatic_impl.h
    Shapes/PolygonShape.h
)
# Group Sprite files under a virtual folder called "Sprite"
SOURCE_GROUP("Source Files\\Sprite" FILES
    Sprite/Sprite.cpp
)
SOURCE_GROUP("Header Files\\Sprite" FILES
    Sprite/Sprite.h
)

# Set include directories for Renderer2D
target_include_directories(Renderer2D PUBLIC . Shapes Sprite)

# Set link libraries for Renderer2D
target_link_libraries(Renderer2D PUBLIC Graphics)

target_compile_definitions(Renderer2D PRIVATE
    # Set PEKAN_RENDERER2D_ROOT_DIR definition to be the path to current source directory
    PEKAN_RENDERER2D_ROOT_DIR="${CMAKE_CURRENT_SOURCE_DIR}"
)
target_compile_definitions(Renderer2D PUBLIC
    # Set PEKAN_ENABLE_2D_SHAPES_ORIENTATION_CHECKING definition to be 0 or 1 depending on the on/off state of the option
    PEKAN_ENABLE_2D_SHAPES_ORIENTATION_CHECKING=$<IF:$<BOOL:${PEKAN_ENABLE_2D_SHAPES_ORIENTATION_CHECKING}>,1,0>
    # Set PEKAN_USE_1D_TEXTURE_FOR_2D_SHAPES_BATCH definition to be 0 or 1 depending on the on/off state of the option
    PEKAN_USE_1D_TEXTURE_FOR_2D_SHAPES_BATCH=$<IF:$<BOOL:${PEKAN_USE_1D_TEXTURE_FOR_2D_SHAPES_BATCH}>,1,0>
)

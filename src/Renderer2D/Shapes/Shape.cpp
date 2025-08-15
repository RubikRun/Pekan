#include "Shape.h"

#include "PekanLogger.h"
#include "Renderer2DSystem.h"
#include "Utils/PekanUtils.h"

namespace Pekan
{
namespace Renderer2D
{

    void Shape::render() const
    {
        PK_ASSERT(m_isValid, "Trying to render a Shape that is not yet created.", "Pekan");

        Renderer2DSystem::submitForRendering(*this);
    }

    void Shape::setColor(glm::vec4 color)
    {
        PK_ASSERT(m_isValid, "Trying to set color of a Shape that is not yet created.", "Pekan");

        m_color = color;
#if !PEKAN_USE_1D_TEXTURE_FOR_2D_SHAPES_BATCH
        m_needUpdateVerticesWorld = true;
#endif
    }

    void Shape::_create()
    {
        PK_ASSERT(!isValid(), "Trying to create a Shape instance that is already created.", "Pekan");

        Transformable2D::_create();

        m_color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        m_isValid = true;
        m_needUpdateVerticesWorld = true;
    }

    void Shape::_destroy()
    {
        PK_ASSERT(isValid(), "Trying to destroy a Shape instance that is not yet created.", "Pekan");

        m_isValid = false;

        Transformable2D::_destroy();
    }

    void Shape::onTransformChanged()
    {
        m_needUpdateVerticesWorld = true;
    }

} // namespace Renderer2D
} // namespace Renderer2D
#include "Demo09_Scene.h"

#include "TransformComponent2D.h"
#include "TransformSystem2D.h"
#include "PekanLogger.h"

using namespace Pekan::Renderer2D;

namespace Demo
{

	constexpr glm::vec2 TURKEY_INITIAL_POSITION = glm::vec2(-0.5f, -0.2f);

	bool Demo09_Scene::init()
	{
		entt::registry& registry = getRegistry();

		m_turkey = createEntity();
		registry.emplace<TransformComponent2D>(m_turkey, TURKEY_INITIAL_POSITION);

		return true;
	}

	void Demo09_Scene::update(double deltaTime)
	{
		entt::registry& registry = getRegistry();

		// Move turkey
		TransformSystem2D::move(registry, m_turkey, glm::vec2(0.02f, 0.01f));
		// Log turkey matrix
		const glm::mat3 turkeyMatrix = TransformSystem2D::getWorldMatrix(registry, m_turkey);
		PK_LOG_INFO
		(
			"Turkey Matrix:\n" <<
			turkeyMatrix[0][0] << "    " << turkeyMatrix[0][1] << "    " << turkeyMatrix[0][2] << "\n" <<
			turkeyMatrix[1][0] << "    " << turkeyMatrix[1][1] << "    " << turkeyMatrix[1][2] << "\n" <<
			turkeyMatrix[2][0] << "    " << turkeyMatrix[2][1] << "    " << turkeyMatrix[2][2] << "\n\n\n",
			"Demo09"
		);
	}

	void Demo09_Scene::render() const
	{
	}

	void Demo09_Scene::exit()
	{
		destroyEntity(m_turkey);
	}

} // namespace Demo

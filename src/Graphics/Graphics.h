#pragma once

#include "ISubsystem.h"

namespace Pekan
{
namespace Graphics
{

	class Graphics : public ISubsystem
	{
	public:

		// Returns a pointer to the global Graphics instance
		static Graphics* getInstance();

		// Loads OpenGL function pointers
		static bool loadOpenGL();

	private: /* functions */

		void init() override;
		void exit() override;

		bool isInitialized() override { return m_isInitialized; }

	private: /* variables */

		bool m_isInitialized = false;
	};

} // namespace Graphics
} // namespace Pekan

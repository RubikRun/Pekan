#pragma once

#include <string>

namespace Pekan
{

	// Optional, user-facing name for an entity.
	//
	// Names are not required to be unique and have no engine-side behavior.
	// They exist purely for readability in scene files and the Editor.
	// Only attached to an entity when its name is non-empty.
	struct NameComponent
	{
		std::string name;
	};

} // namespace Pekan

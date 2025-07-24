#pragma once

#include "Widget.h"

namespace Pekan
{
namespace GUI
{

	// A widget used as a new line between widgets
	//
	// NOTE: Instances of this class MUST be owned by a NewLineWidget_Ptr
	class NewLineWidget : public Widget
	{
	public:

		void create(GUIWindow* guiWindow);
		void destroy();

	private: /* functions */

		void _render() const override;
	};

	typedef std::shared_ptr<NewLineWidget> NewLineWidget_Ptr;
	typedef std::shared_ptr<const NewLineWidget> NewLineWidget_ConstPtr;

} // namespace GUI
} // namespace Pekan

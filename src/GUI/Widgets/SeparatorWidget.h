#pragma once

#include "Widget.h"

namespace Pekan
{
namespace GUI
{

	// A widget used as a separator between widgets
	//
	// NOTE: Instances of this class MUST be owned by a SeparatorWidget_Ptr
	class SeparatorWidget : public Widget
	{
	public:

		void create(GUIWindow* guiWindow);
		void destroy();

	private: /* functions */

		void _render() const override;
	};

	typedef std::shared_ptr<SeparatorWidget> SeparatorWidget_Ptr;
	typedef std::shared_ptr<const SeparatorWidget> SeparatorWidget_ConstPtr;

} // namespace GUI
} // namespace Pekan

#include "GUI.h"

#include "PekanLogger.h"
#include "SubsystemManager.h"

namespace Pekan
{
namespace GUI
{

	static GUI g_gui;

	void GUI::registerSubsystem()
	{
		SubsystemManager::registerSubsystem(&g_gui);
	}

	GUI* GUI::getInstance()
	{
		return &g_gui;
	}

	void GUI::init()
	{
		m_isInitialized = true;
	}

	void GUI::exit()
	{
		m_isInitialized = false;
	}

} // namespace GUI
} // namespace Pekan
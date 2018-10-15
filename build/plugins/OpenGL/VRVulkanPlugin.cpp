#include "VRVulkanGraphicsToolkit.h"

#include <iostream>
#include <plugin/VRPlugin.h>

// special: include this only once in one .cpp file per plugin
#include <plugin/VRPluginVersion.h>

namespace MinVR {

	class VRVulkanPlugin : public VRPlugin {
	public:
		PLUGIN_API VRVulkanPlugin() {
			//std::cout << "VulkanPlugin created." << std::endl;
		}
		PLUGIN_API virtual ~VRVulkanPlugin() {
			//std::cout << "VulkanPlugin destroyed." << std::endl;
		}

		PLUGIN_API void registerWithMinVR(VRMainInterface *vrMain)
		{
			//std::cout << "Registering VRVulkanPlugin." << std::endl;
			vrMain->getFactory()->registerItemType<VRGraphicsToolkit, VRVulkanGraphicsToolkit>("VRVulkanGraphicsToolkit");
		}

		PLUGIN_API void unregisterWithMinVR(VRMainInterface *vrMain)
		{
			//std::cout << "Unregistering VRVulkanPlugin." << std::endl;
			// TODO
		}
	};
}	// end namespace MinVr

extern "C"
{
	PLUGIN_API MinVR::VRPlugin* createPlugin() {
		return new MinVR::VRVulkanPlugin();
	}
}
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "VRVulkanGraphicsToolkit.h"

namespace MinVR {

	VRVulkanGraphicsToolkit::VRVulkanGraphicsToolkit() {

	}

	VRVulkanGraphicsToolkit::~VRVulkanGraphicsToolkit() {

	}

	VRGraphicsToolkit*
		VRVulkanGraphicsToolkit::create(VRMainInterface *vrMain, VRDataIndex *config, const std::string &nameSpace) {
		VRGraphicsToolkit *gfxToolkit = new VRVulkanGraphicsToolkit();
		return gfxToolkit;
	}
	
}
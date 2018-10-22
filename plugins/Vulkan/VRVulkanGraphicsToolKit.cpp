#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "VRVulkanGraphicsToolkit.h"

namespace MinVR {

	/*
	* This section is all functions needed to make the API work with MinVR. 
	* The flush/finish graphics functions will have to do with Vulkan pipeline implementataions.
	*/
	VRVulkanGraphicsToolkit::VRVulkanGraphicsToolkit() {

	}

	VRVulkanGraphicsToolkit::~VRVulkanGraphicsToolkit() {

	}

	void VRVulkanGraphicsToolkit::setSubWindow(VRRect rect) {

	}

	void VRVulkanGraphicsToolkit::flushGraphics() {

	}

	void VRVulkanGraphicsToolkit::finishGraphics() {

	}

	VRGraphicsToolkit*
		VRVulkanGraphicsToolkit::create(VRMainInterface *vrMain, VRDataIndex *config, const std::string &nameSpace) {
		VRGraphicsToolkit *gfxToolkit = new VRVulkanGraphicsToolkit();
		return gfxToolkit;
	}
	


}
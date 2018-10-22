#pragma once
#ifndef VRVULKANGRAPHICSTOOLKIT_H
#define VRVULKANGRAPHICSTOOLKIT_H

#include <display/VRGraphicsToolkit.h>
#include <main/VRFactory.h>
#include <plugin/VRPlugin.h>
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <optional>

namespace MinVR {
	/** Vulkan Implementation for the MinVR VRGraphicsToolkit abstraction.
	*/

	class VRVulkanGraphicsToolkit : public VRGraphicsToolkit {
	public:
		PLUGIN_API VRVulkanGraphicsToolkit();
		PLUGIN_API virtual ~VRVulkanGraphicsToolkit();

		PLUGIN_API std::string getName() const { return "VRVulkanGraphicsToolkit"; }

		//Stuff needed to set up vulkan
		PLUGIN_API void initVulkan();
		PLUGIN_API void cleanUpVulkan();
		PLUGIN_API void recreateSwapChain();

		//Stuff needed by MinVR
		PLUGIN_API void setSubWindow(VRRect rect);
		PLUGIN_API void flushGraphics();
		PLUGIN_API void finishGraphics();

		PLUGIN_API static VRGraphicsToolkit* create(VRMainInterface *vrMain, VRDataIndex *config, const std::string &nameSpace);

		const std::vector<const char*> validationLayers = {
			"VK_LAYER_LUNARG_standard_validation"
		};

		const std::vector<const char*> deviceExtensions = {
			VK_KHR_SWAPCHAIN_EXTENSION_NAME
		};

#ifdef MinVR_DEBUG
		const bool enableValidationLayers = false;
#else
		const bool enableValidationLayers = true;
#endif

		//Might want to make these classes for multiple swap chains
		struct SwapChainSupportDetails {
			VkSurfaceCapabilitiesKHR capabilities;
			std::vector<VkSurfaceFormatKHR> formats;
			std::vector<VkPresentModeKHR> presentModes;
		};

		struct QueueFamilyIndices {
			std::optional<uint32_t> graphicsFamily;
			std::optional<uint32_t> presentFamily;

			bool isComplete() {
				return graphicsFamily.has_value() && presentFamily.has_value();
			}
		};

	private:
		// init stuff
		void createInstance();
		void setupDebugCallback();
		void createSurface();
		void pickPhysicalDevice();
		void createLogicalDevice();
		// swap chain stuff
		void createSwapChain();
		void createImageViews();
		void createRenderPass();
		void createGraphicsPipeline();
		void createFramebuffers();
		void createCommandPool();
		void createCommandBuffers();
		void createSyncObjects();

		void setUpValidationLayers();

		//swapChain recreation
		void cleanupSwapChain();
	};
}

#endif
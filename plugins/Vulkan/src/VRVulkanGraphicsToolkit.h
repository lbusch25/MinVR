#pragma once
#ifndef VRVULKANGRAPHICSTOOLKIT_H
#define VRVULKANGRAPHICSTOOLKIT_H

#include <display/VRGraphicsToolkit.h>
#include <main/VRFactory.h>
#include <plugin/VRPlugin.h>
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <optional>
#include <algorithm>

namespace MinVR {
	/** Vulkan Implementation for the MinVR VRGraphicsToolkit abstraction.
	*/

	class VRVulkanGraphicsToolkit : public VRGraphicsToolkit {
	public:
		PLUGIN_API VRVulkanGraphicsToolkit();
		PLUGIN_API virtual ~VRVulkanGraphicsToolkit();

		PLUGIN_API std::string getName() const { return "VRVulkanGraphicsToolkit"; }

		//Stuff needed to set up vulkan
		PLUGIN_API void initVulkan(GLFWwindow* window);
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

#ifdef MinVR_DEBUG
		const bool enableValidationLayers = false;
#else
		const bool enableValidationLayers = true;
#endif

	private:
		VRInt HEIGHT;
		VRInt WIDTH;

		VkInstance INSTANCE_DEFAULT;
		VkDebugUtilsMessengerEXT CALLBACK_DEFAULT;
		VkSurfaceKHR SURFACE_DEFAULT;

		VkPhysicalDevice PHYSICAL_DEVICE_DEFAULT = VK_NULL_HANDLE;
		VkDevice DEVICE_DEFAULT;

		VkQueue GRAPHICS_QUEUE_DEFAULT;
		VkQueue PRESENT_QUEUE_DEFAULT;

		VkSwapchainKHR SWAP_CHAIN_DEFAULT;
		std::vector<VkImage> SWAP_CHAIN_IMAGES;
		VkFormat SWAP_CHAIN_IMAGE_FORMAT_DEFAULT;
		VkExtent2D SWAP_CHAIN_EXTENT_DEFAULT;
		std::vector<VkImageView> SWAP_CHAIN_IMAGE_VIEWS;
		std::vector<VkFramebuffer> swapChainFramebuffers;

		VkRenderPass renderPass;
		VkPipelineLayout pipelineLayout;
		VkPipeline graphicsPipeline;

		VkCommandPool commandPool;
		std::vector<VkCommandBuffer> commandBuffers;

		std::vector<VkSemaphore> imageAvailableSemaphores;
		std::vector<VkSemaphore> renderFinishedSemaphores;
		std::vector<VkFence> inFlightFences;
		size_t currentFrame = 0;

		bool framebufferResized = false;

		// init stuff
		void createInstance();
		void setupDebugCallback();
		void createSurface(GLFWwindow* window);
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

		//Validation layer setup
		//void setUpValidationLayers();
		bool checkValidationLayerSupport();
		std::vector<const char*> getRequiredExtensions();
		void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT callback, const VkAllocationCallbacks* pAllocator);
		static VKAPI_ATTR VkBool32 VKAPI_CALL VRVulkanGraphicsToolkit::debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
			std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

			return VK_FALSE;
		}
		VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pCallback);

		//swapChain recreation
		void cleanUpSwapChain();

		//Various helper functions
		QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
		bool isDeviceSuitable(VkPhysicalDevice device);
		bool checkDeviceExtensionSupport(VkPhysicalDevice device);
		SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
		VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
		VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes);
		VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, VRInt width, VRInt height);

		void setWindowWidth(VRInt width);
		void setWindwoHeight(VRInt height);
	};
}

#endif
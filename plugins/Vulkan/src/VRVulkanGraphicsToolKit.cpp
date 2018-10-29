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
	

	/*
	* These functions will be my abstracted Vulkan construction (for now)
	*/

	void VRVulkanGraphicsToolkit::initVulkan() {
		createInstance();
		setupDebugCallback();
		createSurface();
		pickPhysicalDevice();
		createLogicalDevice();
		createSwapChain();
		createImageViews();
		createRenderPass();
		createGraphicsPipeline();
		createFramebuffers();
		createCommandPool();
		createCommandBuffers();
		createSyncObjects();
	}

	void VRVulkanGraphicsToolkit::cleanUpVulkan() {
		if (enableValidationLayers) {
			DestroyDebugUtilsMessengerEXT(INSTANCE_DEFAULT, CALLBACK_DEFAULT, nullptr);
		}


		vkDestroyInstance(INSTANCE_DEFAULT, nullptr);
	}

	void VRVulkanGraphicsToolkit::recreateSwapChain() {

	}

	/*
	* These functions will be my Vulkan helper functions (for now, many probably included in API later)
	*/

	void VRVulkanGraphicsToolkit::createInstance() {		//Could be engine, application_name, and version in constructor

		if (enableValidationLayers && !checkValidationLayerSupport()) {
			throw std::runtime_error("validation layers requested, but not available!");
		}

		VkApplicationInfo appInfo = {};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "MinVR";		//Change this to dynamically be the application name
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "No Engine";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_0;

		VkInstanceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;

		auto extensions = getRequiredExtensions();
		createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
		createInfo.ppEnabledExtensionNames = extensions.data();

		if (enableValidationLayers) {
			createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
			createInfo.ppEnabledLayerNames = validationLayers.data();
		}
		else {
			createInfo.enabledLayerCount = 0;
		}

		if (vkCreateInstance(&createInfo, nullptr, &INSTANCE_DEFAULT) != VK_SUCCESS) {
			throw std::runtime_error("failed to create instance!");
		}
	}

	void VRVulkanGraphicsToolkit::createSurface() {

	}

	void VRVulkanGraphicsToolkit::pickPhysicalDevice() {

	}

	void VRVulkanGraphicsToolkit::createLogicalDevice() {

	}

	void VRVulkanGraphicsToolkit::createSwapChain() {

	}

	void VRVulkanGraphicsToolkit::createImageViews() {

	}

	void VRVulkanGraphicsToolkit::createRenderPass() {

	}

	void VRVulkanGraphicsToolkit::createGraphicsPipeline() {

	}

	void VRVulkanGraphicsToolkit::createFramebuffers() {

	}

	void VRVulkanGraphicsToolkit::createCommandPool() {

	}

	void VRVulkanGraphicsToolkit::createCommandBuffers() {

	}

	void VRVulkanGraphicsToolkit::createSyncObjects() {

	}

	void VRVulkanGraphicsToolkit::cleanUpSwapChain() {

	}

	/*
	* Functions for setting up validation layers
	*/

	std::vector<const char*> VRVulkanGraphicsToolkit::getRequiredExtensions() {
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

		if (enableValidationLayers) {
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}

		return extensions;
	}

	bool VRVulkanGraphicsToolkit::checkValidationLayerSupport() {
		uint32_t layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

		std::vector<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

		for (const char* layerName : validationLayers) {
			bool layerFound = false;

			for (const auto& layerProperties : availableLayers) {
				if (strcmp(layerName, layerProperties.layerName) == 0) {
					layerFound = true;
					break;
				}
			}

			if (!layerFound) {
				return false;
			}
		}

		return true;
	}

	void VRVulkanGraphicsToolkit::setupDebugCallback() {
		if (!enableValidationLayers) return;

		VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		createInfo.pfnUserCallback = debugCallback;

		if (CreateDebugUtilsMessengerEXT(INSTANCE_DEFAULT, &createInfo, nullptr, &CALLBACK_DEFAULT) != VK_SUCCESS) {
			throw std::runtime_error("failed to set up debug callback!");
		}
	}

	VkResult VRVulkanGraphicsToolkit::CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pCallback) {
		auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
		if (func != nullptr) {
			return func(instance, pCreateInfo, pAllocator, pCallback);
		}
		else {
			return VK_ERROR_EXTENSION_NOT_PRESENT;
		}
	}

	void VRVulkanGraphicsToolkit::DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT callback, const VkAllocationCallbacks* pAllocator) {
		auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
		if (func != nullptr) {
			func(instance, callback, pAllocator);
		}
	}
}
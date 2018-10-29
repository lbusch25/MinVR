#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "VRVulkanGraphicsToolkit.h"
#include <algorithm>

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

	void VRVulkanGraphicsToolkit::initVulkan(GLFWwindow* window) {
		createInstance();
		setupDebugCallback();
		createSurface(window);
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
		for (auto imageView : SWAP_CHAIN_IMAGE_VIEWS) {
			vkDestroyImageView(DEVICE_DEFAULT, imageView, nullptr);
		}

		vkDestroySwapchainKHR(DEVICE_DEFAULT, SWAP_CHAIN_DEFAULT, nullptr);
		vkDestroyDevice(DEVICE_DEFAULT, nullptr);

		if (enableValidationLayers) {
			DestroyDebugUtilsMessengerEXT(INSTANCE_DEFAULT, CALLBACK_DEFAULT, nullptr);
		}

		vkDestroySurfaceKHR(INSTANCE_DEFAULT, SURFACE_DEFAULT, nullptr);
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

	void VRVulkanGraphicsToolkit::createSurface(GLFWwindow* window) {
		if (glfwCreateWindowSurface(INSTANCE_DEFAULT, window, nullptr, &SURFACE_DEFAULT) != VK_SUCCESS) {
			throw std::runtime_error("failed to create window surface!");
		}
	}

	void VRVulkanGraphicsToolkit::pickPhysicalDevice() {
		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(INSTANCE_DEFAULT, &deviceCount, nullptr);

		if (deviceCount == 0) {
			throw std::runtime_error("failed to find GPUs with Vulkan support!");
		}

		std::vector<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(INSTANCE_DEFAULT, &deviceCount, devices.data());

		for (const auto& device : devices) {
			if (isDeviceSuitable(device)) {
				PHYSICAL_DEVICE_DEFAULT = device;
				break;
			}
		}

		if (PHYSICAL_DEVICE_DEFAULT == VK_NULL_HANDLE) {
			throw std::runtime_error("failed to find a suitable GPU!");
		}
	}

	void VRVulkanGraphicsToolkit::createLogicalDevice() {
		QueueFamilyIndices indices = findQueueFamilies(PHYSICAL_DEVICE_DEFAULT);

		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };

		float queuePriority = 1.0f;
		for (uint32_t queueFamily : uniqueQueueFamilies) {
			VkDeviceQueueCreateInfo queueCreateInfo = {};
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = queueFamily;
			queueCreateInfo.queueCount = 1;
			queueCreateInfo.pQueuePriorities = &queuePriority;
			queueCreateInfos.push_back(queueCreateInfo);
		}

		VkPhysicalDeviceFeatures deviceFeatures = {};

		VkDeviceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

		createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
		createInfo.pQueueCreateInfos = queueCreateInfos.data();

		createInfo.pEnabledFeatures = &deviceFeatures;

		createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
		createInfo.ppEnabledExtensionNames = deviceExtensions.data();

		if (enableValidationLayers) {
			createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
			createInfo.ppEnabledLayerNames = validationLayers.data();
		}
		else {
			createInfo.enabledLayerCount = 0;
		}

		if (vkCreateDevice(PHYSICAL_DEVICE_DEFAULT, &createInfo, nullptr, &DEVICE_DEFAULT) != VK_SUCCESS) {
			throw std::runtime_error("failed to create logical device!");
		}

		vkGetDeviceQueue(DEVICE_DEFAULT, indices.graphicsFamily.value(), 0, &GRAPHICS_QUEUE_DEFAULT);
		vkGetDeviceQueue(DEVICE_DEFAULT, indices.presentFamily.value(), 0, &PRESENT_QUEUE_DEFAULT);

	}

	void VRVulkanGraphicsToolkit::createSwapChain() {
		SwapChainSupportDetails swapChainSupport = querySwapChainSupport(PHYSICAL_DEVICE_DEFAULT);

		VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
		VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
		//USING SETTERS FOR WIDTH/HEIGHT, ASK BRET
		VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities, WIDTH, HEIGHT);		//COME BACK HERE

		uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
		if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
			imageCount = swapChainSupport.capabilities.maxImageCount;
		}

		VkSwapchainCreateInfoKHR createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.surface = SURFACE_DEFAULT;

		createInfo.minImageCount = imageCount;
		createInfo.imageFormat = surfaceFormat.format;
		createInfo.imageColorSpace = surfaceFormat.colorSpace;
		createInfo.imageExtent = extent;
		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

		QueueFamilyIndices indices = findQueueFamilies(PHYSICAL_DEVICE_DEFAULT);
		uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

		if (indices.graphicsFamily != indices.presentFamily) {
			createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			createInfo.queueFamilyIndexCount = 2;
			createInfo.pQueueFamilyIndices = queueFamilyIndices;
		}
		else {
			createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		}

		createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		createInfo.presentMode = presentMode;
		createInfo.clipped = VK_TRUE;

		createInfo.oldSwapchain = VK_NULL_HANDLE;

		if (vkCreateSwapchainKHR(DEVICE_DEFAULT, &createInfo, nullptr, &SWAP_CHAIN_DEFAULT) != VK_SUCCESS) {
			throw std::runtime_error("failed to create swap chain!");
		}

		vkGetSwapchainImagesKHR(DEVICE_DEFAULT, SWAP_CHAIN_DEFAULT, &imageCount, nullptr);
		SWAP_CHAIN_IMAGES.resize(imageCount);
		vkGetSwapchainImagesKHR(DEVICE_DEFAULT, SWAP_CHAIN_DEFAULT, &imageCount, SWAP_CHAIN_IMAGES.data());

		SWAP_CHAIN_IMAGE_FORMAT_DEFAULT = surfaceFormat.format;
		SWAP_CHAIN_EXTENT_DEFAULT = extent;
	}

	void VRVulkanGraphicsToolkit::createImageViews() {
		SWAP_CHAIN_IMAGE_VIEWS.resize(SWAP_CHAIN_IMAGES.size());

		for (size_t i = 0; i < SWAP_CHAIN_IMAGES.size(); i++) {
			VkImageViewCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			createInfo.image = SWAP_CHAIN_IMAGES[i];
			createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			createInfo.format = SWAP_CHAIN_IMAGE_FORMAT_DEFAULT;
			createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			createInfo.subresourceRange.baseMipLevel = 0;
			createInfo.subresourceRange.levelCount = 1;
			createInfo.subresourceRange.baseArrayLayer = 0;
			createInfo.subresourceRange.layerCount = 1;

			if (vkCreateImageView(DEVICE_DEFAULT, &createInfo, nullptr, &SWAP_CHAIN_IMAGE_VIEWS[i]) != VK_SUCCESS) {
				throw std::runtime_error("failed to create image views!");
			}
		}
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

	/*
	*	Helper funtions
	*/

	bool VRVulkanGraphicsToolkit::isDeviceSuitable(VkPhysicalDevice device) {
		QueueFamilyIndices indices = findQueueFamilies(device);

		bool extensionsSupported = checkDeviceExtensionSupport(device);

		bool swapChainAdequate = false;
		if (extensionsSupported) {
			SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device);
			swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
		}

		return indices.isComplete() && extensionsSupported && swapChainAdequate;
	}

	VRVulkanGraphicsToolkit::QueueFamilyIndices VRVulkanGraphicsToolkit::findQueueFamilies(VkPhysicalDevice device) {
		QueueFamilyIndices indices;

		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

		int i = 0;
		for (const auto& queueFamily : queueFamilies) {
			if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
				indices.graphicsFamily = i;
			}

			VkBool32 presentSupport = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(device, i, SURFACE_DEFAULT, &presentSupport);

			if (queueFamily.queueCount > 0 && presentSupport) {
				indices.presentFamily = i;
			}

			if (indices.isComplete()) {
				break;
			}

			i++;
		}

		return indices;
	}

	bool VRVulkanGraphicsToolkit::checkDeviceExtensionSupport(VkPhysicalDevice device) {
		uint32_t extensionCount;
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

		std::vector<VkExtensionProperties> availableExtensions(extensionCount);
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

		std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

		for (const auto& extension : availableExtensions) {
			requiredExtensions.erase(extension.extensionName);
		}

		return requiredExtensions.empty();
	}

	VRVulkanGraphicsToolkit::SwapChainSupportDetails VRVulkanGraphicsToolkit::querySwapChainSupport(VkPhysicalDevice device) {
		SwapChainSupportDetails details;

		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, SURFACE_DEFAULT, &details.capabilities);

		uint32_t formatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, SURFACE_DEFAULT, &formatCount, nullptr);

		if (formatCount != 0) {
			details.formats.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(device, SURFACE_DEFAULT, &formatCount, details.formats.data());
		}

		uint32_t presentModeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, SURFACE_DEFAULT, &presentModeCount, nullptr);

		if (presentModeCount != 0) {
			details.presentModes.resize(presentModeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(device, SURFACE_DEFAULT, &presentModeCount, details.presentModes.data());
		}

		return details;
	}

	VkSurfaceFormatKHR VRVulkanGraphicsToolkit::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
		if (availableFormats.size() == 1 && availableFormats[0].format == VK_FORMAT_UNDEFINED) {
			return { VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };
		}

		for (const auto& availableFormat : availableFormats) {
			if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
				return availableFormat;
			}
		}

		return availableFormats[0];
	}

	VkPresentModeKHR VRVulkanGraphicsToolkit::chooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes) {
		VkPresentModeKHR bestMode = VK_PRESENT_MODE_FIFO_KHR;

		for (const auto& availablePresentMode : availablePresentModes) {
			if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
				return availablePresentMode;
			}
			else if (availablePresentMode == VK_PRESENT_MODE_IMMEDIATE_KHR) {
				bestMode = availablePresentMode;
			}
		}

		return bestMode;
	}

	VkExtent2D VRVulkanGraphicsToolkit::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, VRInt width, VRInt height) {
		if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
			return capabilities.currentExtent;
		}
		else {
			VkExtent2D actualExtent = { width, height };

			actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
			actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));

			return actualExtent;
		}
	}

	void VRVulkanGraphicsToolkit::setWindowWidth(VRInt width) {
		WIDTH = width;
	}
	void VRVulkanGraphicsToolkit::setWindwoHeight(VRInt height) {
		HEIGHT = height;
	}
}
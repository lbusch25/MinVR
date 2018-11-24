#include "../plugins/Vulkan/src/VRVulkanGraphicsToolkit.h"
#include "gameApp.h"

int main(int argc, char **argv) {
	//argc and argv are required so that
	//MinVR can parse command line arguments for config files

	GameApp app(argc, argv);

	//Returns when program shuts down.
	app.run();

	return 0;
}
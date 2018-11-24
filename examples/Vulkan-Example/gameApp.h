#ifndef GAMEAPP_H
#define GAMEAPP_H

#include <api/MinVR.h>

using namespace MinVR;

class GameApp : public VRApp {

	/* Passes argc, argv and config file on to VRApp.*/
	GameApp(int argc, char** argv);
	virtual ~GameApp();
};

#endif
/*
 * Copyright Regents of the University of Minnesota, 2016.  This software is released under the following license: http://opensource.org/licenses/
 * Source code originally developed at the University of Minnesota Interactive Visualization Lab (http://ivlab.cs.umn.edu).
 *
 * Code author(s):
 * 		Dan Orban (dtorban)
 */

#include <display/VRStereoNode.h>

namespace MinVR {

VRStereoNode::VRStereoNode(const std::string &name, VRGraphicsToolkit *gfxToolkit, VRStereoFormat format) :
	VRDisplayNode(name), _gfxToolkit(gfxToolkit), _format(format) {
}

VRStereoNode::~VRStereoNode() {
}


void VRStereoNode::render(VRDataIndex *renderState, VRRenderHandler *renderHandler) {
  //renderState->pushState();

	if (_format == VRSTEREOFORMAT_MONO) {
		renderState->addData("StereoFormat", "Mono");
		renderState->addData("Eye", "Cyclops");
		_gfxToolkit->setDrawBuffer(VRGraphicsToolkit::VRDRAWBUFFER_BACK);
		renderRestOfGraph(renderState, renderHandler);
	}
	else if (_format == VRSTEREOFORMAT_QUADBUFFERED) {
		renderState->addData("StereoFormat", "QuadBuffered");
		renderState->addData("Eye", "Left");
		_gfxToolkit->setDrawBuffer(VRGraphicsToolkit::VRDRAWBUFFER_BACKLEFT);
		renderRestOfGraph(renderState, renderHandler);

		renderState->addData("Eye", "Right");
		_gfxToolkit->setDrawBuffer(VRGraphicsToolkit::VRDRAWBUFFER_BACKRIGHT);
		renderRestOfGraph(renderState, renderHandler);
	}
	else if (_format == VRSTEREOFORMAT_SIDEBYSIDE) {
		renderState->addData("StereoFormat", "SideBySide");
		int w,h;
		if (renderState->exists("ViewportX", "/")) {
			w = renderState->getValue("ViewportWidth");
			h = renderState->getValue("ViewportHeight");
		}
		else {
			w = renderState->getValue("WindowWidth");
			h = renderState->getValue("WindowHeight");
		}
		renderState->addData("Eye", "Left");
		_gfxToolkit->setViewport(VRRect(0,0,w/2,h/2));
		renderRestOfGraph(renderState, renderHandler);

		renderState->addData("Eye", "Right");
		_gfxToolkit->setViewport(VRRect(w/2+1,h/2+1,w/2,h/2));
		renderRestOfGraph(renderState, renderHandler);
	}
	else if (_format == VRSTEREOFORMAT_COLUMNINTERLACED) {
		renderState->addData("StereoFormat", "ColumnInterlaced");
		renderState->addData("Eye", "Left");
		_gfxToolkit->disableDrawingOnEvenColumns();
		renderRestOfGraph(renderState, renderHandler);

		renderState->addData("Eye", "Right");
		_gfxToolkit->disableDrawingOnOddColumns();
		renderRestOfGraph(renderState, renderHandler);

		_gfxToolkit->enableDrawingOnAllColumns();
	}


  //renderState->pushState();
}


void VRStereoNode::renderRestOfGraph(VRDataIndex *renderState, VRRenderHandler *renderHandler) {
	if (_children.size() == 0) {
		renderHandler->onVRRenderScene(renderState, this);
	}
	else {
		VRDisplayNode::render(renderState, renderHandler);
	}	
}


VRDisplayNode* VRStereoNodeFactory::create(VRMainInterface *vrMain, VRDataIndex *config, const std::string &valName, const std::string &nameSpace)
{
	std::string nodeNameSpace = nameSpace + "/" + valName;

	std::string type = config->getValue("Type", nodeNameSpace);
	if (type != "VRStereoNode") {
		// This factory cannot create the type specified
		return NULL;
	}

	VRGraphicsToolkit *gfxToolkit = vrMain->getGraphicsToolkit(config->getValue("GraphicsToolkit", nodeNameSpace));

	std::string formatStr = config->getValue("StereoFormat", nodeNameSpace);
	VRStereoNode::VRStereoFormat format = VRStereoNode::VRSTEREOFORMAT_MONO;
	if (formatStr == "QuadBuffered") {
		format = VRStereoNode::VRSTEREOFORMAT_QUADBUFFERED;
	}
	else if (formatStr == "SideBySide") {
		format = VRStereoNode::VRSTEREOFORMAT_SIDEBYSIDE;
	}
	else if (formatStr == "ColumnInterlaced") {
		format = VRStereoNode::VRSTEREOFORMAT_COLUMNINTERLACED;
	}

	VRDisplayNode *node = new VRStereoNode(valName, gfxToolkit, format);

	std::vector<std::string> childrenNames = config->getValue("Children", nameSpace);
	for (std::vector<std::string>::iterator it = childrenNames.begin(); it < childrenNames.end(); ++it) {
		VRDisplayNode *child = vrMain->getFactory()->createDisplayNode(vrMain, config, *it, "/");
		if (child != NULL) {
			node->addChild(child);
		}
	}

	return node;
}

} /* namespace MinVR */
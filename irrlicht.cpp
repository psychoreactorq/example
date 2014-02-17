
#include <irrlicht.h>

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif

int main()
{
	IrrlichtDevice *device = createDevice( video::EDT_OPENGL, dimension2d<u32>(640, 480), 16, 0, 0, 1, 0);

	if (!device)
		return 1;

    core::stringw str;

	IVideoDriver* driver = device->getVideoDriver();
	ISceneManager* smgr = device->getSceneManager();
	IGUIEnvironment* guienv = device->getGUIEnvironment();

	guienv->addStaticText(L"Hello World! This is the Irrlicht Software renderer!", rect<s32>(10,10,260,22), 1);

	IAnimatedMesh* mesh = smgr->getMesh("../../media/sydney.md2");
	if (!mesh)
	{
		device->drop();
		return 1;
	}
	IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode( mesh );

	if (node)
	{
		node->setMaterialFlag(EMF_LIGHTING, 0);
		node->setMD2Animation(scene::EMAT_STAND);
		node->setMaterialTexture( 0, driver->getTexture("../../media/sydney.bmp") );
	}

	smgr->addCameraSceneNode(0, vector3df(0,30,-40), vector3df(0,5,0));

    int lastFPS = -1;

	while(device->run())
	{
        int fps = driver->getFPS();

        if (lastFPS != fps)
        {
            str = L"Irrlicht Engine example! Driver: ";
            str += driver->getName();
            str += " FPS: ";
            str += fps;
            device->setWindowCaption(str.c_str());
            lastFPS = fps;
        }
		driver->beginScene(1, 1, SColor(255,255,255,140));
		smgr->drawAll();
		guienv->drawAll();
		driver->draw2DRectangle(video::SColor(255,0,255,0), core::rect<s32>(40, 40, 80, 80));
        device->sleep(5);
		driver->endScene();
	}

	device->drop();

	return 0;
}

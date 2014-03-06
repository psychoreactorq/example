
#include <iostream>
#include <vector>
#include <irrlicht.h>
#include <Box2D/Box2D.h>
#define WIDTH 640
#define HEIGHT 480

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

const float M2P=20;
const float P2M=1/M2P;
const video::SColor white=video::SColor(255,255,255,255);
b2World* world;
IVideoDriver* driver;
bool execute=1;

class MyEventReceiver : public IEventReceiver
{
public:
	struct SMouseState
	{
		core::position2di Position;
		bool LeftButtonDown;
		SMouseState() : LeftButtonDown(0) { }
	} MouseState;

	virtual bool OnEvent(const SEvent& event)
	{
		if (event.EventType == irr::EET_MOUSE_INPUT_EVENT)
		{
			switch(event.MouseInput.Event)
			{
			case EMIE_LMOUSE_PRESSED_DOWN:
				MouseState.LeftButtonDown=1;
				break;
			case EMIE_LMOUSE_LEFT_UP:
				MouseState.LeftButtonDown=0;
				break;
			case EMIE_MOUSE_MOVED:
				MouseState.Position.X=event.MouseInput.X;
				MouseState.Position.Y=event.MouseInput.Y;
				break;
			default:
				break;
			}
		}
		else if (event.EventType == irr::EET_KEY_INPUT_EVENT)
		{
			switch(event.KeyInput.Key)
			{
			case KEY_ESCAPE:
				execute=0;
				break;
			default:
				break;
			}
		}
		return 0;
	}

	const SMouseState& GetMouseState(void) const
	{
		return MouseState;
	}


	MyEventReceiver()
	{
	}
};

b2Body* addRect(int x,int y,int w,int h,bool dyn=1)
{
	b2BodyDef bodydef;
	bodydef.position.Set(x*P2M,y*P2M);
	if(dyn) bodydef.type=b2_dynamicBody;
	b2Body* body=world->CreateBody(&bodydef);
	b2PolygonShape shape;
	shape.SetAsBox(P2M*w/2,P2M*h/2);
	b2FixtureDef fixturedef;
	fixturedef.shape=&shape;
	fixturedef.density=1.0;
	body->CreateFixture(&fixturedef);
	return body;
}

inline void drawWorld()
{
	b2Body* tmp=world->GetBodyList();
	b2Vec2 points[4];
	driver->beginScene(1,1,SColor(255,77,77,77));
	while(tmp)
	{
#define point(i) ((b2PolygonShape*)tmp->GetFixtureList()->GetShape())->GetVertex(i)
		const core::vector2di centerLoc(tmp->GetWorldCenter().x*M2P, tmp->GetWorldCenter().y*M2P);
		core::vector2di locVec[4];
		for(u32 i=0; i<4; i++)
		{
			locVec[i]=core::vector2di(centerLoc.X+point(i).x*M2P,centerLoc.Y+point(i).y*M2P);
			locVec[i].rotateBy(tmp->GetAngle()*180.0/M_PI,centerLoc);
		}
		for (u32 i=0; i<3; i++)
			driver->draw2DLine(locVec[i],locVec[i+1],white);
		driver->draw2DLine(locVec[3],locVec[0],white);

		tmp=tmp->GetNext();
	}
	driver->endScene();
}

int main()
{
	MyEventReceiver receiver;
	IrrlichtDevice *device=createDevice(video::EDT_OPENGL,dimension2d<u32>(WIDTH, HEIGHT),16,0,0,1,&receiver);
	if (!device) return 1;
	driver=device->getVideoDriver();
	world=new b2World(b2Vec2(0.0,9.81));
	addRect(WIDTH/2,HEIGHT-50,WIDTH,30,0);
	int lastFPS=-1;
	bool placed=0;
	while(device->run() && execute)
	{
		int fps=driver->getFPS();
		if (lastFPS!=fps)
		{
			core::stringw str=L"Irrlicht + Box2D libraries example! Driver: ";
			str+=driver->getName();
			str+=" FPS: ";
			str+=fps;
			device->setWindowCaption(str.c_str());
			lastFPS = fps;
		}
		if(receiver.GetMouseState().LeftButtonDown)
		{
			if(!placed)
			{
				addRect(receiver.GetMouseState().Position.X,receiver.GetMouseState().Position.Y,20,20);
				placed=!placed;
			}
		}
		else if(placed)placed=!placed;
		drawWorld();
		world->Step(1.0/30.0,8,3);
		device->sleep(5);
	}
	device->drop();
	return 0;
}

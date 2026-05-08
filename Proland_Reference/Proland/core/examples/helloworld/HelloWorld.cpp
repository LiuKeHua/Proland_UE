#include <stdlib.h>

#include "ork/core/FileLogger.h"
#include "ork/render/FrameBuffer.h"
#include "ork/resource/XMLResourceLoader.h"
#include "ork/scenegraph/SceneManager.h"
#include "ork/ui/GlutWindow.h"

#include "proland/util/TerrainViewController.h"

using namespace ork;
using namespace proland;

class HelloWorld : public GlutWindow
{
public:
	ptr<SceneManager> manager;
	ptr<TerrainViewController> controller;
	int mouseX, mouseY;
	bool rotate;

	HelloWorld() : GlutWindow(Window::Parameters().size(1024, 768))
	{
		FileLogger::File *out = new FileLogger::File("log.html");

		//Logger::DEBUG_LOGGER = new FileLogger("DEBUG", out, Logger::DEBUG_LOGGER);
		Logger::INFO_LOGGER = new FileLogger("INFO", out, Logger::INFO_LOGGER);
		Logger::WARNING_LOGGER = new FileLogger("WARNING", out, Logger::WARNING_LOGGER);
		Logger::ERROR_LOGGER = new FileLogger("ERROR", out, Logger::ERROR_LOGGER);

		ptr<XMLResourceLoader> resLoader = new XMLResourceLoader();
		resLoader->addPath(".");
		resLoader->addArchive("helloworld.xml");

		ptr<ResourceManager> resManager = new ResourceManager(resLoader, 8);

		manager = new SceneManager();
		manager->setResourceManager(resManager);

		manager->setScheduler(resManager->loadResource("defaultScheduler").cast<Scheduler>());
		manager->setRoot(resManager->loadResource("scene").cast<SceneNode>());
		manager->setCameraNode("camera");
		manager->setCameraMethod("draw");

		controller = new TerrainViewController(manager->getCameraNode(), 2500.0);
	}

	virtual ~HelloWorld()
	{
		FrameBuffer::getError();
	}

	virtual void redisplay(double t, double dt)
	{
		controller->update();
		controller->setProjection(0.f,0.f, vec4f(-1.0f, 1.0f, -1.0f, 1.0f));

		ptr<FrameBuffer> fb = FrameBuffer::getDefault();
		fb->clear(true, false, true);

		//fb->setPolygonMode(LINE,LINE);

		manager->update(t, dt);
		manager->draw();

		GlutWindow::redisplay(t, dt);

		if (Logger::ERROR_LOGGER != NULL)
		{
			Logger::ERROR_LOGGER->flush();
		}
	}

	virtual void reshape(int x, int y)
	{
		ptr<FrameBuffer> fb = FrameBuffer::getDefault();
		fb->setDepthTest(true, LESS);
		fb->setViewport(vec4<GLint>(0, 0, x, y));
		GlutWindow::reshape(x, y);
	}

	virtual void idle(bool damaged)
	{
		GlutWindow::idle(damaged);
		if (damaged)
		{
			manager->getResourceManager()->updateResources();
		}
	}

	virtual bool mouseClick(button b, state s, modifier m, int x, int y)
	{
		mouseX = x;
		mouseY = y;
		rotate = (m & CTRL) != 0;
		return true;
	}

	virtual bool mouseMotion(int x, int y)
	{
		if (rotate)
		{
			controller->phi += (mouseX - x) / 500.0;
			controller->theta += (mouseY - y) / 500.0;
		}
		else
		{
			vec3d oldp = manager->getWorldCoordinates(mouseX, mouseY);
			vec3d p = manager->getWorldCoordinates(x, y);
			if (valid(oldp) && valid(p))
			{
				controller->move(oldp, p);
			}
		}
		mouseX = x;
		mouseY = y;
		return true;
	}

	virtual bool mouseWheel(wheel b, modifier m, int x, int y)
	{
		if (b == WHEEL_DOWN) 
		{
			controller->d *= 1.1;
		}
		if (b == WHEEL_UP)
		{
			controller->d /= 1.1;
		}
		return true;
	}

	virtual bool keyTyped(unsigned char c, modifier m, int x, int y)
	{
		if (c == 27)
		{
			::exit(0);
		}

		if (c == ',')
		{
			static bool LINEMode = false;
			LINEMode = !LINEMode;
			ptr<FrameBuffer> fb = FrameBuffer::getDefault();
			fb->setPolygonMode(LINEMode ? LINE : FILL, CULL);
			return true;
		}


		return true;
	}

	virtual bool specialKey(key k, modifier m, int x, int y)
	{
		switch (k)
		{
		case KEY_F5:
			manager->getResourceManager()->updateResources();
			break;
		default:
			break;
		}
		return true;
	}

	bool valid(vec3d p) 
	{
		return abs(p.x) < 1000.0 && abs(p.y) < 1000.0 && abs(p.z) < 1000.0;
	}

	static static_ptr<Window> app;
};
 
static_ptr<Window> HelloWorld::app;
 
int main(int argc, char* argv[])
{
	atexit(Object::exit);
	HelloWorld::app = new HelloWorld();
	HelloWorld::app->start();
	return 0;
}

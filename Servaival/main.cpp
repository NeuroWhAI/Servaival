#include <CodeAdapter\EasyCA.h>
#include <CodeAdapterSFML\SFMLFactory.h>

#ifndef _DEBUG
#include <Windows.h>
#endif

#include "LoadScene.h"




int main()
{
#ifndef _DEBUG
	ShowWindow(GetConsoleWindow(), SW_HIDE);
#endif


	initializeCA<SFMLFactory>();


	auto window = caFactory->createWindow();
	window->create(caDraw::Size(1024, 768), { "Servaival" }, false);
	window->setFramerate(60);


	auto sceneManager = canew<caUtil::SceneManager>();
	sceneManager->replaceScene(*window, canew<LoadScene>());

	window->setSceneManager(sceneManager);


	while (window->isRunning())
	{
		caTouch->update();
		caKeyboard->update(*window);


		if (sceneManager->hasScene() == false)
		{
			window->exit();
		}


		window->update();
		window->draw(caDraw::Color(0, 0, 0));
	}


	return 0;
}


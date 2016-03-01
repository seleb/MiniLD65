#pragma once

#include <MY_Scene_GameOver.h>
#include <MY_Scene_Main.h>
#include <shader/ComponentShaderText.h>
#include <sweet/UI.h>
#include <OrthographicCamera.h>
#include <Sprite.h>
#include <RenderOptions.h>
#include <NumberUtils.h>

MY_Scene_GameOver::MY_Scene_GameOver(Game * _game, unsigned long int _score) :
	MY_Scene_Base(_game),
	fadeCount(0)
{
	// remove default camera
	removeCamera(activeCamera);
	
	OrthographicCamera * cam = new OrthographicCamera(0, 256, 0, 256, -10, 10);
	childTransform->addChild(cam);
	cameras.push_back(cam);
	activeCamera = cam;


	Sprite * bg = new Sprite(MY_ResourceManager::globalAssets->getTexture("GAMEOVER")->texture, baseShader);
	bg->mesh->setScaleMode(GL_NEAREST);
	childTransform->addChild(bg)->scale(256, false)->translate(256/2.f, 256/2.f, 0);

	uiLayer->addMouseIndicator();

	// Create a linear layout to contain all of our menu items
	// remember that these elements are all going to exist in the uiLayer, so we pass in its physics world in the constructors
	VerticalLinearLayout * layout = new VerticalLinearLayout(uiLayer->world);
	uiLayer->addChild(layout);
	layout->horizontalAlignment = kCENTER;
	layout->verticalAlignment = kBOTTOM;
	// set the layout's size to 100% of its parent
	layout->setRationalHeight(1.f, uiLayer);
	layout->setRationalWidth(1.f, uiLayer);
	
	TextArea * t = new TextArea(uiLayer->world, font, textShader);
	t->horizontalAlignment = kCENTER;
	t->verticalAlignment = kMIDDLE;

	t->setRenderMode(kTEXTURE);

	fade = new NodeUI(uiLayer->world);
	fade->setRationalHeight(1.f, uiLayer);
	fade->setRationalWidth(1.f, uiLayer);
	fade->setBackgroundColour(56/255.f,97/255.f,65/255.f,1.f);
	
	layout->addChild(t);
	uiLayer->addChild(fade);
	
	t->setRationalWidth(1.f, layout);
	t->setRationalHeight(1.f, layout);

	t->setMouseEnabled(true);
	t->eventManager.addEventListener("click", [_game](sweet::Event * _event){
		sweet::step.cycles = 0;
		_game->scenes["main"] = new MY_Scene_Main(_game);
		_game->switchScene("main", true);
	});

	
	std::stringstream ss;
	ss << "GAME OVER\n" << "You missed school!\n\n" << "Score: " << _score << "\n\n";
	if(_score <= 0){
		ss << "You didn't even try!";
	}else if(_score < 5){
		ss << "You should be ashamed of yourself!";
	}else if(_score < 10){
		ss << "You did okay I guess!";
	}else if(_score < 15){
		ss << "You did pretty good!";
	}else if(_score < 30){
		ss << "Your mother would be proud!";
	}else if(_score < 50){
		ss << "You're really dedicated!";
	}else if(_score < 100){
		ss << "You should stop playing!";
	}else if(_score < 1000){
		ss << "You're definitely cheating!";
	}else{
		ss << "You don't get to play anymore!\n:/";
		t->eventManager.listeners.clear();
		t->eventManager.addEventListener("click", [_game](sweet::Event * _event){
			_game->exit();
		});
	}
	ss << "\n\n*click to play again*";
	t->setText(ss.str());


	uiLayer->invalidateLayout();
}


void MY_Scene_GameOver::update(Step * _step){
	fadeCount += 1;
	if(fadeCount  < 50){
		fade->bgColour.a = 1.f - fadeCount /50.f;
	}
	MY_Scene_Base::update(_step);
}
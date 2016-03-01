#pragma once

#include <MY_Scene_Main.h>
#include <shader/ComponentShaderText.h>
#include <sweet/UI.h>
#include <OrthographicCamera.h>
#include <Sprite.h>

MY_Scene_Main::MY_Scene_Main(Game * _game) :
	MY_Scene_Base(_game)
{
	// remove default camera
	removeCamera(activeCamera);
	
	OrthographicCamera * cam = new OrthographicCamera(0, 256, 0, 256, -10, 10);
	childTransform->addChild(cam);
	cameras.push_back(cam);
	activeCamera = cam;


	Sprite * bg = new Sprite(MY_ResourceManager::globalAssets->getTexture("BG")->texture, baseShader);
	bg->mesh->setScaleMode(GL_NEAREST);
	childTransform->addChild(bg)->scale(256, false)->translate(256/2.f, 256/2.f, 0);


	Sprite * kid = new Sprite(MY_ResourceManager::globalAssets->getTexture("KID")->texture, baseShader);
	kid->mesh->setScaleMode(GL_NEAREST);
	childTransform->addChild(kid)->scale(29, false)->translate(29/2.f + 145, 29/2.f + 105, 0);

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


	uiLayer->invalidateLayout();
}

void MY_Scene_Main::update(Step * _step){
	if(mouse->leftDown()){
		awake += 1;
		sleepiness += 2;
	}else{
		awake -= 1;
		sleepiness -= awake > 50 ? 1 : 2;
	}


	if(sleepiness < 0){
		sleepiness = 0;
	}else if(sleepiness > 100){
		sleepiness = 100;
	}

	if(awake < 0){
		// fall asleep, lose game
	}else if(awake > 100){
		awake = 100;
	}
	
	MY_Scene_Base::update(_step);
}

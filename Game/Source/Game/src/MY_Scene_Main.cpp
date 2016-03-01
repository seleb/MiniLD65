#pragma once

#include <MY_Scene_Main.h>
#include <shader/ComponentShaderText.h>
#include <sweet/UI.h>
#include <OrthographicCamera.h>
#include <Sprite.h>
#include <RenderOptions.h>
#include <NumberUtils.h>

MY_Scene_Main::MY_Scene_Main(Game * _game) :
	MY_Scene_Base(_game),
	day(0),
	busDelay(0),
	busOpen(false)
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


	bus = new Sprite(MY_ResourceManager::globalAssets->getTexture("BUS")->texture, baseShader);
	bus->mesh->setScaleMode(GL_NEAREST);
	childTransform->addChild(bus)->scale(125, false)->translate(-125, 175, 0);


	kid = new Sprite(MY_ResourceManager::globalAssets->getTexture("KID")->texture, baseShader);
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
	
	daysLabel = new TextLabel(uiLayer->world, font, textShader);
	daysLabel->horizontalAlignment = kCENTER;
	daysLabel->verticalAlignment = kMIDDLE;
	updateDay();

	layout->addChild(daysLabel);
	
	daysLabel->setRationalWidth(1.f, layout);
	//daysLabel->setHeight(font->getLineHeight()*2.5f);


	uiLayer->invalidateLayout();
}

void MY_Scene_Main::update(Step * _step){
	int p = _step->cycles;
	p = p % 800 - busDelay;
	int t = 0;
	if(p < 250){
		// pulling up
		t = Easing::easeOutCubic(p, 0, 135, 250);
	}else if(p < 500){
		// stopped
		if(!busOpen){
			busOpen = true;
			bus->mesh->replaceTextures(MY_ResourceManager::globalAssets->getTexture("BUS-OPEN")->texture);
		}

		t = 135;
		if(mouse->leftJustPressed()){
			boardBus();
		}
	}else if(p < 750){
		// leaving
		if(busOpen){
			busOpen = false;
			bus->mesh->replaceTextures(MY_ResourceManager::globalAssets->getTexture("BUS")->texture);
		}
		t = Easing::easeInCubic(p-500, 135, 200, 250);
	}else if(p < 800){
		// waiting
		t = 335;
	}else{
		// left
		if(onBus){
			nextDay();
		}else{
			gameOver();
		}
	}
	bus->firstParent()->translate(t, bus->getWorldPos().y, 0, false);
	
	MY_Scene_Base::update(_step);
}

void MY_Scene_Main::boardBus(){
	onBus = true;
	childTransform->removeChild(kid->firstParent());
}

void MY_Scene_Main::gameOver(){
}

void MY_Scene_Main::nextDay(){
	day += 1;
	onBus = false;
	childTransform->addChild(kid->firstParent(), false);
	busDelay = sweet::NumberUtils::randomInt(50, 500);
	updateDay();
}

void MY_Scene_Main::updateDay(){
	daysLabel->setText("Days since you last missed school: " + std::to_string(day));
}

void MY_Scene_Main::render(sweet::MatrixStack * _matrixStack, RenderOptions * _renderOptions){
	_renderOptions->depthEnabled = false;
	MY_Scene_Base::render(_matrixStack, _renderOptions);
}
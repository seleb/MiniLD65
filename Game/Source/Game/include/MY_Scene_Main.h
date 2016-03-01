#pragma once

#include <MY_Scene_Base.h>

class TextLabel;

class MY_Scene_Main : public MY_Scene_Base{
public:
	// days since you last missed school
	unsigned long int day;
	unsigned long int busDelay;
	bool onBus;
	bool busOpen;

	TextLabel * daysLabel;
	Sprite * kid, * bus;
	NodeUI * fade;

	MY_Scene_Main(Game * _game);


	virtual void update(Step * _step) override;
	virtual void render(sweet::MatrixStack * _matrixStack, RenderOptions * _renderOptions) override;


	void boardBus();

	void nextDay();
	void gameOver();

	void updateDay();
};
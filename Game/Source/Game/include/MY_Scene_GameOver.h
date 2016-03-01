#pragma once

#include <MY_Scene_Base.h>

class TextLabel;

class MY_Scene_GameOver : public MY_Scene_Base{
public:
	NodeUI * fade;
	int fadeCount;
	MY_Scene_GameOver(Game * _game, unsigned long int _score);

	virtual void update(Step * _step) override;
};
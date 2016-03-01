#pragma once

#include <MY_Scene_Base.h>

class MY_Scene_Main : public MY_Scene_Base{
public:
	// how awake you are
	float awake;
	// how sleepy you are
	float sleepiness;
	// days since you last missed school
	unsigned long int day;

	MY_Scene_Main(Game * _game);


	virtual void update(Step * _step) override;
};
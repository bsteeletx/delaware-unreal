#pragma once
#include "Sprite.h"
#include "Defines.h"
#include "ParentImage.h"

class Checkbox
{
public:
	Checkbox(void);
	~Checkbox(void);

	void setData(const char filename[], Sprite OptionsParent);
	void Check(void);
	void UnCheck(void);
	void hide(void);
	void show(void);

	Sprite Checked;
	Sprite NotChecked;

private:
	bool isChecked;

    Sprite Parent;
};

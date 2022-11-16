#include "Checkbox.h"
#include "AGK.h"

Checkbox::Checkbox(void)
{
}


Checkbox::~Checkbox(void)
{
}

void Checkbox::setData(const char filename[], Sprite OptionsParent)
{
    char tempFileCheck[32] = {NULL};
	char tempFileNotCheck[32] = {NULL};
    //unsigned int imgNum;

    Parent = OptionsParent;

    if (strlen(filename) < 24)
	{
		strcpy(tempFileCheck, filename);
		strcpy(tempFileNotCheck, filename);

		strcat(tempFileCheck, "_on.png");
		strcat(tempFileNotCheck, "_off.png");
	}

	Checked = Sprite(Parent.getImageNumber(), Text(tempFileCheck));
	NotChecked = Sprite(Parent.getImageNumber(), Text(tempFileNotCheck));
}

void Checkbox::Check(void)
{
	Checked.setVisible(true);
	NotChecked.setVisible(false);

	isChecked = true;
}

void Checkbox::UnCheck(void)
{
	NotChecked.setVisible(true);
	Checked.setVisible(false);

	isChecked = false;
}

void Checkbox::hide(void)
{
	NotChecked.setVisible(false);
	Checked.setVisible(false);
}

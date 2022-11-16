
#pragma once

class ParentImage
{
public:
	ParentImage(void);
	~ParentImage(void);

    unsigned int getImageNumber(const char filename[]);
	unsigned int *getParentImage(void);

    bool isDifferent(const char filename[]);
    bool isParentImageSet(void);

    void setImage(const char filename[]);

private:

    unsigned int pImageNumber[4];
    char fileLoc[8][32];
};


#include "ParentImage.h"
#include "template.h"

ParentImage::ParentImage(void)
{
    for (int i = 0; i < 4; i++)
        pImageNumber[i] = 0;

}

ParentImage::~ParentImage(void)
{


}

bool ParentImage::isParentImageSet()
{
    int temp = pImageNumber[0];

    if (temp != 0)
        return true;
    else
        return false;
}

bool ParentImage::isDifferent(const char *filename)
{
    for (int i = 0; i < 4; i++)
    {
        if (!strcmp(fileLoc[i], filename)) //if it's the same
            return false;
    }

    return true;
}

void ParentImage::setImage(const char filename[])
{
    char temp[32] = {NULL};
    int tempImage;

    //if parent image is a single 1024x1024 image file
	if (agk::GetFileExists(filename))
	{
        tempImage = pImageNumber[0];

        pImageNumber[0] = agk::LoadImage(filename);

        if (filename[0] != 'd')
            addToQueue(pImageNumber[0], 0);
        else
            addToQueue(pImageNumber[0], 1);

        strcpy(fileLoc[0], filename);
    }
    else //if there are multiple 1024x1024 parent image files
    {
        char postfix[8] = {NULL};
        int counter = 0;

        while (counter != -1)
        {
            tempImage = pImageNumber[counter];
            strncpy(temp, filename, agk::Len(filename) - 4);
            sprintf(postfix, " %d.png", counter);
            strcat(temp, postfix);

            if (agk::GetFileExists(temp))
            {
                strcpy(fileLoc[counter], temp);

                if (filename[0] != 'd')
                    addToQueue(pImageNumber[counter], 0);
                else
                    addToQueue(pImageNumber[counter], 1);

                pImageNumber[counter++] = agk::LoadImage(temp);

                int length = agk::Len(temp);

                for (int i = 0; i < length; i++)
                    temp[i] = '\0';

                length = agk::Len(postfix);

                for (int i = 0; i < length; i++)
                    postfix[i] = '\0';
            }
            else
                counter = -1;
        }
    }
}

unsigned int ParentImage::getImageNumber(const char filename[])
{
    for (int i = 0; i < 4; i++)
    {
        int tempImgNum;

        tempImgNum = agk::LoadSubImage(pImageNumber[i], filename);

        if (agk::GetImageExists(tempImgNum))
        {
            if (fileLoc[0][0] != 'd')
                addToQueue(tempImgNum, 0);
            else
                addToQueue(tempImgNum, 1);

            return tempImgNum;
        }
    }

    return 0;
}


unsigned int *ParentImage::getParentImage(void)
{
	return pImageNumber;
}
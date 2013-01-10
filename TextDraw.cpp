#include "TextDraw.h"
#include "assets.gen.h"

void TextDraw::draw(VideoBuffer* vid, const char* cText, int iOffsety)
{
	int iLength = 0;
	for(; cText[iLength] != '\0'; iLength++)
		;
	
	if(iLength > 16)
		iLength = 16;	//Crop off if it's too long
	
	int iOffsetx = 8 - (iLength/2);
	
	UInt2 pos = {iOffsetx, iOffsety};
	UInt2 size = {iLength,1};
	vid->bg1.fillMask(pos, size);
	for(int i = 0; i < iLength; i++)
	{
		char frame = cText[i] - ' ';
		if(frame > Font.numFrames())
			frame = 31;	//Print '?' for unknown characters
		vid->bg1.image(pos, Font, frame);
		pos.x += 1;
	}
}
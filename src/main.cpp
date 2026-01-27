// --------------------- //
// (C) 2026 madoodia.com //
// --------------------- //

#include "ray.h"
#include "utils.h"

int main(int, char**)
{
	ImageBuffer image = AllocateImage(1280, 720);

	u32* finalOutput = image.pixels;

	{
		TimeStamp timer;
		for (u32 Y = 0;
			 Y < image.height;
			 ++Y)
		{
			for (u32 X = 0;
				 X < image.width;
				 ++X)
			{
				*finalOutput++ = (Y < image.height / 3) ? 0xFFAA0000 : ((Y < 2 * image.height / 3) ? 0xFFFFFFFF : 0xFF00AA00);
			}
		}

		WriteImage(image, "images/output.bmp");
	}

	return 0;
}

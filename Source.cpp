#include<iostream>
#include<string>
#include"Header.h"
int main()
{
	int whith = 1920;
	int height = 1280;
	Image image(whith, height);
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < whith; x++)
		{
			image.SetColor(Color((float)x / (float)whith, 1.0f - (float)x / (float)whith, (float)y / (float)height), x, y);
		}
	}
	image.Export("img1.bmp");

	Image copy(0, 0);
	copy.Read("img1.bmp");
	copy.Export("copy.bmp");
	return 0;
}
#pragma once
#include<iostream>
#include <vector>
struct Color
{
	float r, g, b;
	Color();
	Color(float r, float g, float b);
	~Color();
};
 
class Image {
public:
	Image(int whith, int height);
	~Image();

	Color GetColor(int x, int y) const;
	void SetColor(const Color& color, int x, int y);
	void Read(const char* path);
	void Export(const char* path);
private:
	int m_whith;
	int m_height;

	std::vector<Color> m_color;


};
#include "Header.h"
#include <fstream>
#include <iostream>
#include"Locale.h"
Color::Color()
	:r(0), g(0), b(0)
{
}

Color::Color(float r, float g, float b)
	:r(r), g(g), b(b)
{
}

Color::~Color()
{
}

Image::Image(int whith, int height)
	:m_whith(whith), m_height(height), m_color(std::vector<Color>(whith * height))
{
}
Image::~Image()
{
}

Color Image::GetColor(int x, int y) const
	
{
	return m_color[y * m_whith + x];

}

void Image::SetColor(const Color& color, int x, int y)
{
	m_color[y * m_whith + x].r = color.r;
	m_color[y * m_whith + x].g = color.g;
	m_color[y * m_whith + x].b = color.b;
}

void Image::Read(const char* path)
{
	setlocale(LC_ALL, "Russian");
	int state;
	std::ifstream f;
	std::ofstream tx;
	tx.open("pixel.txt", std::ios::out);
    f.open(path, std::ios::in | std::ios::binary);
		if (!f.is_open())
		{
			std::cout << "Не удалось открыть файл" << std::endl;
		    state = 1;
		}
	const int fileHeaderSize = 14;
	const int informationHeaderSize = 40;

	unsigned char fileHeader[fileHeaderSize];
	f.read(reinterpret_cast<char*>(fileHeader), fileHeaderSize);
	
	unsigned char informationHeader[informationHeaderSize];
	f.read(reinterpret_cast<char*>(informationHeader), informationHeaderSize);

	int fileSize = fileHeader[2] + (fileHeader[3] << 8) + (fileHeader[4] << 16) + (fileHeader[5] << 24);

	m_whith = informationHeader[4] + (informationHeader[5] << 8) + (informationHeader[6] << 16) + (informationHeader[7] << 24);
    m_height = informationHeader[8] + (informationHeader[9] << 8) + (informationHeader[10] << 16) + (informationHeader[11] << 24);

	m_color.resize(m_whith * m_height);

	const int paddingAmount = ((4 - (m_whith * 3) % 4) % 4);

	for (int y = 0; y < m_height; y++)
	{
		for (int x = 0; x < m_whith; x++)
		{
			unsigned char color[3];
			f.read(reinterpret_cast<char*> (color), 3);
			float a = m_color[y * m_whith + x].r = static_cast<float>(color[2]) / 255.0f;
			float j = m_color[y * m_whith + x].g = static_cast<float>(color[1]) / 255.0f;
			float k =m_color[y * m_whith + x].b = static_cast<float>(color[0]) / 255.0f;
            
			tx << a * 255.0f << " " <<
				j * 255.0f << " " <<
				k * 255.0f << std::endl;
		}
		f.ignore(paddingAmount);
	}
	f.close();
}

void Image::Export(const char* path)
{
	setlocale(LC_ALL, "Russian");
	std::ofstream f;
	f.open(path, std::ios::out | std::ios::binary);
	if (!f.is_open())
	{
		std::cout << "Не удалость открыть файл" << std::endl;
	}
	unsigned char bmoPad[] = { 0,0,0 };
	int paddingAmount = ((4 - (m_whith * 3) % 4) % 4);
	const int fileHeaderSize = 14;
	const int informationHeaderSize = 40;
	const int fileSize = fileHeaderSize + informationHeaderSize + m_whith * m_height * 3 + paddingAmount * m_whith;

	unsigned char fileHeader[fileHeaderSize];

	//fileType
	fileHeader[0] = 'B';
	fileHeader[1] = 'M';
	//fileSize
	fileHeader[2] = fileSize;
	fileHeader[3] = fileSize >> 8;
	fileHeader[4] = fileSize >> 16;
	fileHeader[5] = fileSize >> 24;
	//reserved1(не используется)
	fileHeader[6] = 0;
	fileHeader[7] = 0;
	//reseived2
	fileHeader[8] = 0;
	fileHeader[9] = 0;
	//offset bits
	fileHeader[10] = fileHeaderSize + informationHeaderSize;
	fileHeader[11] = 0;
	fileHeader[12] = 0;
	fileHeader[13] = 0;

	unsigned char informationHeader[informationHeaderSize];

	//header size
	informationHeader[0] = informationHeaderSize;
	informationHeader[1] = 0;
	informationHeader[2] = 0;
	informationHeader[3] = 0;
	//image whith
	informationHeader[4] = m_whith;
	informationHeader[5] = m_whith >> 8;
	informationHeader[6] = m_whith >> 16;
	informationHeader[7] = m_whith >> 24;
	//image height
	informationHeader[8] = m_height;
	informationHeader[9] = m_height >> 8;
	informationHeader[10] = m_height >> 16;
	informationHeader[11] = m_height >> 24;
	//planes(число плоскостей)
	informationHeader[12] = 1;
	informationHeader[13] = 0;
	//bitCount
	informationHeader[14] = 24;
	informationHeader[15] = 0;
	//compression (не ипользуется)
	informationHeader[16] = 0;
	informationHeader[17] = 0;
	informationHeader[18] = 0;
	informationHeader[19] = 0;
	//sizelmale(не используется)
	informationHeader[20] = 0;
	informationHeader[21] = 0;
	informationHeader[22] = 0;
	informationHeader[23] = 0;
	//x pixel per meter()
	informationHeader[24] = 0;
	informationHeader[25] = 0;
	informationHeader[26] = 0;
	informationHeader[27] = 0;
	//y pixel per meter 
	informationHeader[28] = 0;
	informationHeader[29] = 0;
	informationHeader[30] = 0;
	informationHeader[31] = 0;
	//total color
	informationHeader[32] = 0;
	informationHeader[33] = 0;
	informationHeader[34] = 0;
	informationHeader[35] = 0;
	//important colors
	informationHeader[36] = 0;
	informationHeader[37] = 0;
	informationHeader[38] = 0;
	informationHeader[39] = 0;

	f.write(reinterpret_cast<char*>(fileHeader), fileHeaderSize);
	f.write(reinterpret_cast<char*>(informationHeader), informationHeaderSize);
	
	for (int y = 0; y < m_height; y++)
	{
		for (int x = 0; x < m_whith; x++)
		{
			unsigned char r = static_cast<unsigned char>(GetColor(x, y).r * 255.0f);
			unsigned char g = static_cast<unsigned char>(GetColor(x, y).g * 255.0f);
			unsigned char b = static_cast<unsigned char>(GetColor(x, y).b * 255.0f);

			unsigned char color[] = { b, g, r };
			f.write(reinterpret_cast<char*>(color), 3);
		}
		f.write(reinterpret_cast <char*> (bmoPad),paddingAmount);

	}
	f.close();
	std::cout << "Файл создан" << std::endl;
}
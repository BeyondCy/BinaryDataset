#include "BinaryDatasetReader.h"

std::shared_ptr<std::vector<std::pair<int,std::vector<uint8_t>>>> BinaryDatasetReader::readBina(
	std::string binaName,int imgWidth, int imgHeight,int channels
) 
{
	FILE *fp = fopen(binaName.c_str(), "rb");
	if (fp == NULL)
	{
		std::cout << "Open error!" << std::endl;
		fclose(fp);
		return nullptr;
	}
	auto pairs = std::make_shared<std::vector<std::pair<int, std::vector<uint8_t>>>>();
	char ch = fgetc(fp);
	int imgSize = imgWidth*imgHeight*channels;
	fseek(fp, 0L, SEEK_END);
	int fileSize = ftell(fp);
	int imageNum = fileSize / (imgSize + 1);
	fseek(fp, 0L, SEEK_SET);
	for (int i = 0;i < imageNum;i++)
	{
		int label = fgetc(fp);
		std::vector<uint8_t> img(imgSize);

		int size = fread(img.data(),1, imgSize,fp);
		if (size != imgSize)
		{
			std::cout << "read file error" << std::endl;
			return nullptr;
		}
		std::pair<int, std::vector<uint8_t>> labelAndImage;
		labelAndImage.first = label;
		labelAndImage.second = img;
		pairs.get()->push_back(labelAndImage);
	}
	fclose(fp);
	return pairs;
}
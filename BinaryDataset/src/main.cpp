#include "BinaryDatasetWriter.h"
#include "BinaryDatasetReader.h"
#include <random>
#define WITER 0

static std::shared_ptr<std::vector<std::pair<int, std::string>>> shuffle_data(std::vector<std::pair<int, std::string>>& labelsAndImages)
{
	std::vector<size_t> indexArray;
	for (size_t i = 0; i < labelsAndImages.size();i++)
	{
		indexArray.push_back(i);
	}
	std::random_shuffle(indexArray.begin(), indexArray.end());
	auto shuffleLabelsAndImages = std::make_shared<std::vector<std::pair<int, std::string>>>();
	for (size_t i = 0; i < labelsAndImages.size(); i++)
	{
		const size_t dstIndex = i;
		const size_t srcIndex = indexArray[i];
		std::pair<int, std::string> tmp;
		std::pair<int, std::string> src = labelsAndImages[srcIndex];
		tmp.first = src.first;
		tmp.second = src.second;
		shuffleLabelsAndImages.get()->push_back(tmp);
	}
	return shuffleLabelsAndImages;
}

void main()
{
#if WITER
	//首先，把不同类别的图片放到不同的文件夹下，比如，人脸图片放到一个文件夹下，非人脸放到一个文件夹下
	//在Vector中的pair下添加所有的类别文件夹和其标签，比如人脸为1，非人脸为0
	//生成二进制文件的过程不需要知道图片的大小，通过opencv读入图片后，就可以知道图片的大小了
	//这种方式目前只适合打包大小相同的图片

	std::vector<std::pair<int, std::string>> all(2);
	all.at(0).first = 1;
	all.at(0).second = "E:\\dataset\\TrainingImages\\FACES";
	all.at(1).first = 0;
	all.at(1).second = "E:\\dataset\\TrainingImages\\NFACES";



	BinaryDatasetWriter binData;
	auto labelsAndImages = std::make_shared<std::vector<std::pair<int, std::string>>>();

	for each (auto pair in all)
	{
		std::vector<std::string> images = binData.getFileLists(pair.second); // load files name
		std::vector<int> labels(images.size(), pair.first);  // generate lables
		for (int i = 0;i < images.size();i++) 
		{
			std::pair<int, std::string> tmpPair;
			tmpPair.first = labels[i];
			tmpPair.second = images[i];
			labelsAndImages.get()->push_back(tmpPair);
		}
	}

	auto shuffleLabelsAndImages = shuffle_data(*labelsAndImages.get());

	std::string binfile = "E:\\dataset\\face_detection.bin";
	binData.images2BinaryFile(*shuffleLabelsAndImages.get(), binfile);
	std::cout << "write " << shuffleLabelsAndImages.get()->size() << " images to " << binfile << std::endl;
#else
	//测试的时候，我们必须事前知道图像的大小和通道数目
	std::string binfile = "E:\\dataset\\face_detection.bin";
	auto labelAndImages = BinaryDatasetReader::readBina(binfile,19,19,1);

	std::random_device rd;//来产生一个随机数当作种子  
	std::uniform_int_distribution<int> uni_dist(0, labelAndImages.get()->size()-10); //指定范围的随机数发生器  
	int startIndex = uni_dist(rd);
	for (int i = startIndex;i < startIndex + 10;i++) 
	{
		std::pair<int, std::vector<uint8_t>> oneLabelAndData = labelAndImages.get()->at(i);
		std::cout << "label is " << oneLabelAndData.first << std::endl;
		cv::Mat img(19, 19, CV_8UC1, oneLabelAndData.second.data(), 19);
		cv::imshow("test", img);
		cv::waitKey(0);
	}

#endif
	cv::waitKey(0);
	
}

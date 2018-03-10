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
	//���ȣ��Ѳ�ͬ����ͼƬ�ŵ���ͬ���ļ����£����磬����ͼƬ�ŵ�һ���ļ����£��������ŵ�һ���ļ�����
	//��Vector�е�pair��������е�����ļ��к����ǩ����������Ϊ1��������Ϊ0
	//���ɶ������ļ��Ĺ��̲���Ҫ֪��ͼƬ�Ĵ�С��ͨ��opencv����ͼƬ�󣬾Ϳ���֪��ͼƬ�Ĵ�С��
	//���ַ�ʽĿǰֻ�ʺϴ����С��ͬ��ͼƬ

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
	//���Ե�ʱ�����Ǳ�����ǰ֪��ͼ��Ĵ�С��ͨ����Ŀ
	std::string binfile = "E:\\dataset\\face_detection.bin";
	auto labelAndImages = BinaryDatasetReader::readBina(binfile,19,19,1);

	std::random_device rd;//������һ���������������  
	std::uniform_int_distribution<int> uni_dist(0, labelAndImages.get()->size()-10); //ָ����Χ�������������  
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

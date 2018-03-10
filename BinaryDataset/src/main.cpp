#include "BinaryDatasetWriter.h"
#include "BinaryDatasetReader.h"
#include <random>
#define WITER 1



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
	BinaryDatasetWriter bdw;
	bdw.genBinaryDataset(all);

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

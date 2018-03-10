
#ifndef BINARY_DATASET_WRITER_H
#define BINARY_DATASET_WRITER_H
#pragma once

#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

class BinaryDatasetWriter
{
public:
	BinaryDatasetWriter()
	{
		_iHeight = 32;
		_iWidth  = 32;
	}
	~BinaryDatasetWriter(){}

public:
	void images2BinaryFile(std::vector<std::pair<int, std::string>>& labelsAndImages, std::string filename );
	
	void mat2Binary( std::string& image_file, int label, FILE*& fp );


//	cv::Mat imageReshape( cv::Mat& input );

	std::string getFileName( std::string & filename );

	std::vector<std::string> getFileLists( std::string file_folder );

public:
	int _iHeight;
	int _iWidth;
};
#endif // BINARY_DATASET_WRITER_H

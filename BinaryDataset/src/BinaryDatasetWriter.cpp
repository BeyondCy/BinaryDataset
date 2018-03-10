#include "BinaryDatasetWriter.h"
#define DEBUG 1

void BinaryDatasetWriter::images2BinaryFile(
	std::vector<std::pair<int, std::string>>& labelsAndImages,
	std::string filename )
{
	const int size_list = labelsAndImages.size();

	FILE *fp = fopen( filename.c_str(), "wb" );
	if ( fp == NULL )
	{
		std::cout << "Open error!" << std::endl;
		fclose(fp);
		return;
	}

	for ( int idx = 0; idx <size_list; ++idx )
	{
		mat2Binary(labelsAndImages[idx].second, labelsAndImages[idx].first, fp );
#if DEBUG
		std::cout << labelsAndImages[idx].second << " write to file " << std::endl;
#endif
	}

	fclose(fp);
}

void BinaryDatasetWriter::mat2Binary(
	std::string& image_file, int label, FILE*& fp )
{
	cv::Mat image = cv::imread( image_file, IMREAD_UNCHANGED );
	if ( !image.data )
	{
		std::cout << "Image " << getFileName(image_file) << " load failed!"
			<< std::endl;
	}
	else
	{
		int ret = fwrite(&label, sizeof(char), 1, fp);
		if (ret != 1)
		{
			std::cout << "fwrite error" << std::endl;
			return;
		}
		if ( image.channels() == 1 )
		{
			int pixelCount = image.rows * image.cols;
			char* pData = (char*)image.data;
			ret = fwrite(pData, sizeof(char), pixelCount, fp);
			if (ret != pixelCount)
			{
				std::cout << "fwrite error" << std::endl;
				return;
			}
		}
		else
		{
			Mat channel[3];
			split(image, channel);
			for (int i = 2;i > -1;i--) 
			{
				int pixelCount = image.rows * image.cols*image.channels();
				char* pData = (char*)channel[i].data;
				ret = fwrite(pData, sizeof(char), pixelCount, fp);
				if (ret != pixelCount)
				{
					std::cout << "fwrite error" << std::endl;
					return;
				}
			}
		}
		image.release();
	}
}

std::string BinaryDatasetWriter::getFileName( std::string & filename )
{
	int iBeginIndex = filename.find_last_of("\\")+1;
	int iEndIndex   = filename.length();
	
	return filename.substr( iBeginIndex, iEndIndex - iBeginIndex );
}

std::vector<std::string> BinaryDatasetWriter::getFileLists( std::string file_folder )
{
	string file_finding = file_folder + "\\*.*";
	const char * mystr= file_finding.c_str();
	std::vector<std::string> flist;
	std::string lineStr;
	std::vector<std::string> extendName;
	extendName.push_back("jpg");
	extendName.push_back("JPG");
	extendName.push_back("bmp");
	extendName.push_back("png");
	extendName.push_back("gif");

	HANDLE file;
	WIN32_FIND_DATA fileData;
	file = FindFirstFile(mystr, &fileData );
	FindNextFile( file, &fileData );
	while(FindNextFile( file, &fileData ))
    {
		lineStr = fileData.cFileName;
        // remove the files which are not images
		for (int i = 0; i < 4; i ++)
		{
			if (lineStr.find(extendName[i]) < 999)
			{
				flist.push_back(file_folder+"\\"+lineStr);
				break;
			}
		}	
	}
	return flist;
}

std::shared_ptr<std::vector<std::pair<int, std::string>>> BinaryDatasetWriter::shuffle_data(std::vector<std::pair<int, std::string>>& labelsAndImages)
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

void BinaryDatasetWriter::genBinaryDataset(std::vector<std::pair<int, std::string>>& all)
{
	auto labelsAndImages = std::make_shared<std::vector<std::pair<int, std::string>>>();

	for each (auto pair in all)
	{
		std::vector<std::string> images = getFileLists(pair.second); // load files name
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
	images2BinaryFile(*shuffleLabelsAndImages.get(), binfile);
	std::cout << "write " << shuffleLabelsAndImages.get()->size() << " images to " << binfile << std::endl;
}

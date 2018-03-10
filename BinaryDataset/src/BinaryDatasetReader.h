
#ifndef BINARY_DATASET_READER_H
#define BINARY_DATASET_READER_H
#pragma once

#include <iostream>
#include <vector>
#include <memory>

class BinaryDatasetReader {
public:
	static std::shared_ptr<std::vector<std::pair<int,std::vector<uint8_t>>>> readBina(std::string binaName,int imgWidth,int imgHeight,int channels);
};

#endif // BINARY_DATASET_READER_H

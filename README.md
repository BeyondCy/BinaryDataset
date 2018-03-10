# BinaryDataset
write labels and images to a binary file
# Format
label image label image ......
# Usage--Write to(The face detection dataset example offered.)

Put different categories of pictures under different folders
Specify categories and folders in your code,Like this:
```
	std::vector<std::pair<int, std::string>> all(2);
	all.at(0).first = 1;
	all.at(0).second = "E:\\dataset\\TrainingImages\\FACES";
	all.at(1).first = 0;
	all.at(1).second = "E:\\dataset\\TrainingImages\\NFACES";
```
And call genBinaryDataset method in BinaryDatasetWriter generate binary dataset:
```
	BinaryDatasetWriter bdw;
	bdw.genBinaryDataset(all);
```

#Usage--Read from
Call BinaryDatasetReader::readBina method to get a std::shared_ptr<std::vector<std::pair<int,std::vector<uint8_t>>>> which contains labels and images.
parameters is : a path of binary dataset file,image widht,image height,image channel.
```
	std::string binfile = "E:\\dataset\\face_detection.bin";
	auto labelAndImages = BinaryDatasetReader::readBina(binfile,19,19,1);
```



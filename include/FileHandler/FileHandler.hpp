#ifndef FILE_HANDLER_HPP
#define FILE_HANDLER_HPP

#include<sndfile.h>
#include<armadillo>
#include<memory>
#include<iostream>
#include <utils/ObjectGuard.hpp>

namespace NsFileHandler {

class FileHandler
{
private:
	std::string path;
	unsigned int numOfChannels;
	int sampleRate;
	double * ptrToData;
	SF_INFO * info;
	uint64_t numberOfFrames;

	double * readSamples(const uint64_t numOfSamples);
public:
	FileHandler(const std::string & path, const int numOfChan, const int rate);
	~FileHandler(void);
	void writeSamples(std::shared_ptr<double[]> y, const uint64_t size);
	int getNumberOfFrames();
	std::shared_ptr<double[]> getSignalHandler(const uint64_t numOfSamples);
};

}

#endif
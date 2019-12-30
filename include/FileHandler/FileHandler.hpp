#ifndef FILE_HANDLER_HPP
#define FILE_HANDLER_HPP

#include <sndfile.h>
#include <armadillo>
#include <memory>
#include <iostream>
#include <fstream>
#include <utils/ObjectGuard.hpp>

namespace NsFileHandler {

class FileHandler
{
private:
	std::string path;
	int sampleRate;
	double * ptrToData;
	SF_INFO * info;
	SNDFILE * file;
	uint64_t numberOfFrames;

	double * readSamples(const uint64_t numOfSamples);
	void createFileToWrite(const std::string & filename);
	SNDFILE * openSoundFileToRead();
	SNDFILE * openFileToWriteAndReuseInfo(const std::string & filename);
public:
	FileHandler(const std::string & path);
	~FileHandler(void);
	void writeSamples(std::shared_ptr<double[]> y, const uint64_t size, const std::string & filename);
	int getNumberOfFrames();
	std::shared_ptr<double[]> getSignalHandler(const uint64_t numOfSamples);
};

}

#endif
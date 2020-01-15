#ifndef FILE_HANDLER_HPP
#define FILE_HANDLER_HPP

#include <sndfile.h>
#include <armadillo>
#include <memory>
#include <iostream>
#include <fstream>
#include <utils/ObjectGuard.hpp>
#include <Signal/Signal.hpp>

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

	double * readSamples(const int64_t numOfSamples);
	void createFileToWrite(const std::string & filename);
	SNDFILE * openSoundFileToRead();
	SNDFILE * openFileToWriteAndReuseInfo(const std::string & filename);
public:
	FileHandler(const std::string & path);
	~FileHandler(void);
	void writeSamples(NsSignal::Signal<double> & signal, const std::string & filename);
	int getNumberOfFrames();
	NsSignal::Signal<double> getSignalHandler(const int64_t numOfSamples);
};

}

#endif
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

	enum Samples {
		READ_ALL = -1
	};

	std::string path;
	int sampleRate;
	double * ptrToData;
	SF_INFO * info;
	SNDFILE * file;
	int64_t numberOfFrames;

	double * readSamples(const int64_t numOfSamples);
	SNDFILE * openSoundFileToRead();
	SNDFILE * openFileToWriteAndReuseInfo(const std::string & filename);
public:
	FileHandler(const std::string & path);
	~FileHandler(void);
	void writeSamples(NsSignal::Signal<double> & signal, const std::string & filename);
	int getNumberOfFrames();
	NsSignal::Signal<double> getSignalHandler(int64_t numOfSamples);
};

}

#endif
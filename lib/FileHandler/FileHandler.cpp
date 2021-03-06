#ifndef FILE_HANDLER_CPP
#define FILE_HANDLER_CPP

#include <functional>
#include <FileHandler/FileHandler.hpp>

using NsObjectGuard::ObjectGuard;
using NsSignal::Signal;

namespace NsFileHandler {

SNDFILE * FileHandler::openSoundFileToRead() {
	info = new SF_INFO();
	info->format = 0;
	file = sf_open(path.c_str(), SFM_READ, info);
	if(file == nullptr) {
		throw std::runtime_error("Sound file does not exist/wrong path!");
	}
	return file;
}

SNDFILE * FileHandler::openFileToWriteAndReuseInfo(const std::string & filename) {
	SF_INFO * tmpInfo = new SF_INFO();
	ObjectGuard guard([tmpInfo](){delete tmpInfo;});
	tmpInfo->samplerate = info->samplerate;
	tmpInfo->channels = info->channels;
	tmpInfo->format = info->format;
	tmpInfo->sections = info->sections;
	tmpInfo->seekable = info->seekable;
	tmpInfo->frames = info->frames;
	auto file = sf_open(filename.c_str(), SFM_WRITE, tmpInfo);
	if(file == nullptr) {
		throw std::runtime_error("Failed to open sound file to write!");
	}
	return file;
}

FileHandler::FileHandler(const std::string & p) : path(p), numberOfFrames(0)
{
	file = openSoundFileToRead();
}

FileHandler::~FileHandler(void)
{
	delete info;
	sf_close(file);
}

Signal<double> FileHandler::getSignalHandler(int64_t numOfSamples) {
	if(numOfSamples == Samples::READ_ALL) {
		numOfSamples = info->frames;
	}
	auto samples = readSamples(numOfSamples);
	Signal<double> signal(samples, getNumberOfFrames(), info->channels);
	return signal;
}

double * FileHandler::readSamples(const int64_t numOfSamples) {
	ptrToData = new double[info->channels * numOfSamples];
	numberOfFrames = sf_readf_double(file, ptrToData, numOfSamples);
	if(numberOfFrames != numOfSamples) {
		std::cout << "Failed to read. Required value: " << numOfSamples << ". Number of frames read: " << numberOfFrames << '\n';
	}
	std::cout << info->channels <<" "<< info->format <<" "<< info->samplerate  << " " << numberOfFrames << '\n';
	return ptrToData;
}

void FileHandler::writeSamples(Signal<double> & signal, const std::string & filename) {
	SNDFILE * fileOut = openFileToWriteAndReuseInfo(filename);
	auto count = sf_writef_double(fileOut, signal.getSignal().get(), signal.getLength());
	if(count != signal.getLength()) {
		std::cout <<"Number of Frames error" << std::endl;
	}
	sf_write_sync(fileOut);
	sf_close(fileOut);
}

int FileHandler::getNumberOfFrames() {
	return numberOfFrames;
}

}

#endif
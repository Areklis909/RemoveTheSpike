#ifndef FILE_HANDLER_CPP
#define FILE_HANDLER_CPP

#include <functional>
#include "../include/FileHandler.hpp"

using NsObjectGuard::ObjectGuard;

namespace NsFileHandler {

FileHandler::FileHandler(const std::string & p, const int numOfChan, const int rate) : path(p), numOfChannels(numOfChan), sampleRate(rate), numberOfFrames(0)
{
	// sampleRate = 48000;
	// numberOfFrames = 0;
}


FileHandler::~FileHandler(void)
{
}

std::shared_ptr<double[]> FileHandler::getSignalHandler(const uint64_t numOfSamples) {
	auto samples = readSamples(numOfSamples);
	std::cout << "Samples read: " << getNumberOfFrames() << std::endl;
	return std::shared_ptr<double[]>(samples, [](double * p){ delete [] p; } );
}

double * FileHandler::readSamples(const uint64_t numOfFrames) {
	info = new SF_INFO();
	ObjectGuard guard([this](){delete info;});

	info->format = 0;

	auto file = sf_open(path.c_str(), SFM_READ, info);
	ObjectGuard sndGuard([&](){sf_close(file);});

	std::cout << info->channels <<" "<< info->format <<" "<< info->samplerate << std::endl;
	ptrToData = new double[numOfChannels * numOfFrames];
	numberOfFrames = sf_readf_double(file, ptrToData, numOfFrames);
	if(numberOfFrames != numOfFrames) {
		std::cout << "Failed to read. Required value: " << numOfFrames << ". Number of frames read: " << numberOfFrames << '\n';
	}
	return ptrToData;
}

void FileHandler::writeSamples(std::shared_ptr<double[]> y, const uint64_t size) {
	info = new SF_INFO();
	ObjectGuard guard([this](){delete info;});

	info->channels = numOfChannels;
	info->samplerate = sampleRate;
	info->format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;
	
	auto file = sf_open(path.c_str(), SFM_WRITE, info);
	ObjectGuard sndGuard([&](){sf_close(file);});

	auto count = sf_writef_double(file, y.get(), size);
	if(count != size) {
		std::cout <<"Number of Frames error" << std::endl;
	}

	sf_write_sync(file);
}

int FileHandler::getNumberOfFrames() {
	return numberOfFrames;
}

}

#endif
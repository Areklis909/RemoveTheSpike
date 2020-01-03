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

Signal<double> FileHandler::getSignalHandler(const uint64_t numOfSamples) {
	auto samples = readSamples(numOfSamples);
	Signal<double> signal(samples, getNumberOfFrames());
	return signal;
}

double * FileHandler::readSamples(const uint64_t numOfFrames) {
	std::cout << info->channels <<" "<< info->format <<" "<< info->samplerate  << std::endl;
	ptrToData = new double[info->channels * numOfFrames];
	numberOfFrames = sf_readf_double(file, ptrToData, numOfFrames);
	if(numberOfFrames != numOfFrames) {
		std::cout << "Failed to read. Required value: " << numOfFrames << ". Number of frames read: " << numberOfFrames << '\n';
	}
	return ptrToData;
}

void FileHandler::createFileToWrite(const std::string & filename) {
	std::fstream f;
	f.open(filename, std::ios::out);
	f.close();
}

void FileHandler::writeSamples(Signal<double> & signal, const std::string & filename) {

	createFileToWrite(filename);
	SNDFILE * fileOut = openFileToWriteAndReuseInfo(filename);
	ObjectGuard guard([fileOut](){delete fileOut;});
	auto count = sf_writef_double(fileOut, signal.getSignal().get(), signal.getLength());
	if(count != signal.getLength()) {
		std::cout <<"Number of Frames error" << std::endl;
	}

	sf_write_sync(file);
}

int FileHandler::getNumberOfFrames() {
	return numberOfFrames;
}

}

#endif
#include <stdio.h>
#include <iostream>
#include <string>
#include <algorithm>
#include <Loaders/Loader.h>

namespace Soft {

/**
 * Loader
 * ------
 */
Loader::~Loader() {}

void Loader::fillBufferUntil(std::string end) {
	if (!isLoading) {
		return;
	}

	setChunkDelimiter(end);

	char c;

	while (!isAtDelimiter() && !isAtEOL() && (c = nextChar()) != EOF) {
		buffer += c;
	}

	if (c == EOF) {
		fclose(file);

		isLoading = false;
	} else if (isAtDelimiter()) {
		int delimiterLen = delimiter.length();
		int delimiterPos = (int)(buffer.length() - delimiterLen);

		// Ignore the delimiter string at the end of the buffer
		// so chunks can be cleanly parsed with only their contents.
		buffer.erase(delimiterPos, delimiterLen);
	}
}

bool Loader::bufferEndsWith(std::string str) {
	int pos = std::max((int)(buffer.length() - str.length()), 0);
	int len = str.length();

	return buffer.length() > 0 && buffer.compare(pos, len, str) == 0;
}

bool Loader::isAtDelimiter() {
	return bufferEndsWith(delimiter);
}

bool Loader::isAtEOL() {
	return bufferEndsWith("\n");
}

void Loader::load(const char* filePath) {
	FILE* f = fopen(filePath, "r");

	if (f != NULL) {
		file = f;
		isLoading = true;
	} else {
		printf("[Loader] Error opening file: %s\n", filePath);
	}
}

char Loader::nextChar() {
	return fgetc(file);
}

std::string Loader::readNextChunk() {
	buffer.clear();
	fillBufferUntil(delimiter);

	return isAtEOL() ? buffer.substr(0, buffer.size() - 1) : buffer;
}

void Loader::nextLine() {
	fillBufferUntil("\n");
	buffer.clear();
}

void Loader::setChunkDelimiter(std::string delimiter) {
	this->delimiter = delimiter;
}

} // namespace Soft

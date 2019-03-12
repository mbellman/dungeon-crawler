#pragma once

#include <string>
#include <vector>

namespace Soft {

class Loader {
public:
	virtual ~Loader() = 0;

protected:
	bool isLoading = false;

	void load(const char* filePath);
	std::string readNextChunk();
	void nextLine();
	void setChunkDelimiter(std::string delimiter);

private:
	std::string buffer = "";
	std::string delimiter = " ";
	FILE* file = 0;

	void fillBufferUntil(std::string end);
	bool bufferEndsWith(std::string str);
	bool isAtDelimiter();
	bool isAtEOL();
	int getDelimiterOffset();
	char nextChar();
};

} // namespace Soft

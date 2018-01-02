
#include "Reader.hpp"
#include <iostream>
#include <curl/curl.h>
#include <cmath>
#include <map>
#include <sstream>

Reader::Reader()
	: _filename("")
	, _size(-1)
	, _position(-1)
{
	curl_global_init(CURL_GLOBAL_DEFAULT);
}

Reader::~Reader()
{}

MediaioStatus Reader::set_filename(const char* filename)
{
	_filename = std::string(filename);
	return kMediaioStatusOK;
}

const char* Reader::get_filename()
{
	return _filename.c_str();
}

enum HeaderType
{
	AcceptRanges,
	ContentLength,
	ContentRange,
	ContentType
};

static size_t write_data(void *ptr, size_t size, size_t count, void *handle)
{
	std::map<HeaderType, std::string>* raw_header = (std::map<HeaderType, std::string>*)handle;
	std::string header((char*)ptr, count);

	if(strncmp(header.c_str(), "Accept-Ranges: ", 15) == 0)
	{
		header.erase(0, 15);
		raw_header->insert({HeaderType::AcceptRanges, header});
	}
	if(strncmp(header.c_str(), "Content-Length: ", 16) == 0)
	{
		header.erase(0, 16);
		raw_header->insert({HeaderType::ContentLength, header});
	}
	if(strncmp(header.c_str(), "Content-Range: ", 15) == 0)
	{
		header.erase(0, 15);
		raw_header->insert({HeaderType::ContentRange, header});
	}
	if(strncmp(header.c_str(), "Content-Type: ", 14) == 0)
	{
		header.erase(0, 14);
		raw_header->insert({HeaderType::ContentType, header});
	}

	return size * count;
}

MediaioStatus Reader::open()
{
	std::map<HeaderType, std::string> header;
	CURL* curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, _filename.c_str());
	curl_easy_setopt(curl, CURLOPT_RANGE, "0-1");
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
	curl_easy_setopt(curl, CURLOPT_HEADERDATA, &header);

	CURLcode res;

	res = curl_easy_perform(curl);
	// $data = curl_exec( $curl );
	if(res != CURLE_OK) {
		std::cout << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
	} else {
		std::string content_range = header.find(HeaderType::ContentRange)->second;

		size_t position = content_range.find('/');
		content_range.erase(0, position + 1);
		_size = std::stol(content_range);
		// double cld;
		// res = curl_easy_getinfo(curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &cld);
		// std::cout << "Content Length Download: " << cld << std::endl;
		// double sd;
		// res = curl_easy_getinfo(curl, CURLINFO_SIZE_DOWNLOAD, &sd);
		// std::cout << "Size Download: " << sd << std::endl;
		// double su;
		// res = curl_easy_getinfo(curl, CURLINFO_SIZE_UPLOAD, &su);
		// std::cout << "Size Upload: " << su << std::endl;
		// double clu;
		// res = curl_easy_getinfo(curl, CURLINFO_CONTENT_LENGTH_UPLOAD, &clu);
		// std::cout << "Content Length Upload: " << clu << std::endl;
		// char *ct = NULL;
		// res = curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &ct);
		// std::cout << "Content Type: " << ct << std::endl;
	}

	curl_easy_cleanup(curl);
	_position = 0;
	return kMediaioStatusOK;
}

MediaioStatus Reader::close()
{
	return kMediaioStatusOK;
}

int64_t Reader::getSize()
{
	return 0;
}

MediaioStatus Reader::setCacheSize(uint64_t size)
{
	return kMediaioStatusOK;
}

uint64_t Reader::getCacheSize()
{
	return 0;
}

uint64_t Reader::getPosition()
{
	return 0;
}

struct string {
	char *ptr;
	size_t len;
};

void init_string(struct string *s) {
	s->len = 0;
	s->ptr = (char*)malloc(s->len+1);
	if (s->ptr == NULL) {
		fprintf(stderr, "malloc() failed\n");
		exit(EXIT_FAILURE);
	}
	s->ptr[0] = '\0';
}

size_t writefunc(void *ptr, size_t size, size_t nmemb, struct string *s)
{
	size_t new_len = s->len + size*nmemb;
	s->ptr = (char*)realloc(s->ptr, new_len+1);
	if (s->ptr == NULL) {
		fprintf(stderr, "realloc() failed\n");
		exit(EXIT_FAILURE);
	}
	memcpy(s->ptr+s->len, ptr, size*nmemb);
	s->ptr[new_len] = '\0';
	s->len = new_len;

	return size * nmemb;
}

uint64_t Reader::read(char* data, uint64_t size)
{
	// std::cout << "get data - " << size << " bytes" << std::endl;

	if(_buffer.size() < size){
		load_buffer(size);
	}

	memcpy(data, _buffer.data(), size);
	std::vector<char> tmp;
	tmp.resize(_buffer.size() - size);
	memcpy(tmp.data(), _buffer.data() + size, tmp.size());

	_buffer.swap(tmp);

	return size;
}

MediaioStatus Reader::load_buffer(uint64_t min_size)
{
	// std::cout << "get data over HTTP for position " << _position << std::endl;
	size_t size = std::max((uint64_t)10000, min_size);
	struct string s;
	init_string(&s);

	std::ostringstream range;
	range.precision(0);
	range << std::fixed << _position << "-" << _position + size;

	CURL* curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, _filename.c_str());
	curl_easy_setopt(curl, CURLOPT_RANGE, range.str().c_str());

	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);

	CURLcode res;
	res = curl_easy_perform(curl);
	if(res != CURLE_OK) {
		std::cout << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
		return kMediaioStatusFailed;
	} else {
		size_t previous_size = _buffer.size();
		_buffer.resize(_buffer.size() + size);
		memcpy(_buffer.data() + previous_size, s.ptr, size);
	}

	free(s.ptr);
	_position += size;
	return kMediaioStatusOK;
}

MediaioStatus Reader::seek(int64_t offset)
{
	if(offset == 0)
		return kMediaioStatusOK;

	// std::cout << "Seek " << offset << std::endl;
	if(_buffer.size() > offset)
	{
		std::vector<char> tmp;
		tmp.resize(_buffer.size() - offset);
		memcpy(tmp.data(), _buffer.data() + offset, tmp.size());

		_buffer.swap(tmp);
	}
	else
	{
		_position += offset - _buffer.size();
		_buffer.clear();
	}
	return kMediaioStatusOK;
}

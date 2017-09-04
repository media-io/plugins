
#include "Unwrapper.hpp"

#include <iostream>
#include <iomanip>

extern "C" {
#include <libavformat/avformat.h>
#include <libavformat/avio.h>
}

Unwrapper::Unwrapper()
	: _avFormatContext(nullptr)
	, _init(false)
{
}

Unwrapper::~Unwrapper()
{
	if(_init)
	{
		_reader->close(_readerHandle);
	}
}

struct buffer_data {
	uint8_t *ptr;
	size_t size; ///< size left in the buffer
};

int Unwrapper::read_packet(void *opaque, uint8_t *buf, int buf_size)
{
	struct buffer_data *bd = (struct buffer_data *)opaque;
	buf_size = FFMIN(buf_size, bd->size);
	printf("ptr:%p size:%zu\n", bd->ptr, bd->size);
	/* copy internal buffer data to buf */
	memcpy(buf, bd->ptr, buf_size);
	bd->ptr  += buf_size;
	bd->size -= buf_size;
	return buf_size;
}

MediaioStatus Unwrapper::open(MediaioPluginReader* reader, void* readerHandle)
{
	_reader = reader;
	_readerHandle = readerHandle;

	av_register_all();

	if(!(_avFormatContext = avformat_alloc_context())) {
		return kMediaioStatusFailed;
	}

	struct buffer_data bd = { 0 };

	// size_t buffer_size = 4096;
	size_t avio_ctx_buffer_size = 4096;
	// uint8_t *buffer = nullptr;
	uint8_t* avio_ctx_buffer = nullptr;

	// avio_ctx_buffer = (uint8_t*)av_malloc(avio_ctx_buffer_size);
	// if (!avio_ctx_buffer) {
	// 	return kMediaioStatusFailed;
	// }

	// _avio_ctx = avio_alloc_context(
	// 	avio_ctx_buffer,
	// 	avio_ctx_buffer_size,
	// 	0,
	// 	&bd,
	// 	&this->read_packet,
	// 	NULL,
	// 	NULL
	// );

	// if (!_avio_ctx) {
	// 	return kMediaioStatusFailed;
	// }

	// _avFormatContext->pb = _avio_ctx;
	// int ret = avformat_open_input(&_avFormatContext, NULL, NULL, NULL);
	
	const char* filename = reader->get_filename(readerHandle);
	int ret = avformat_open_input(&_avFormatContext, filename, NULL, NULL);
	if (ret < 0) {
		std::cerr << "Could not open input " << av_err2str(ret) << std::endl;
		return kMediaioStatusFailed;
	}

	return kMediaioStatusOK;
}

MediaioStatus Unwrapper::configure(const Metadata* parameters)
{
	return kMediaioStatusOK;
}

MediaioStatus Unwrapper::unwrapNextFrame(const int streamIndex, CodedData* codedData)
{
	if(!_init)
	{
		_reader->open(_readerHandle);
		_init = true;
	}

	AVPacket packet;
	av_init_packet(&packet);
	packet.data = NULL;
	packet.size = 0;

	const int ret = av_read_frame(_avFormatContext, &packet);
	if(ret < 0) // error or end of file
	{
		std::cout << "ERROR: " << av_err2str(ret) << std::endl;
		return kMediaioStatusFailed;
	}

	resize_coded_data(codedData, packet.size);
	memcpy((char*)codedData->data, packet.data, packet.size);
	return kMediaioStatusOK;
}

MediaioStatus Unwrapper::seekAtFrame(const int frame)
{
	std::cout << "seek at frame " << frame << std::endl;
	return kMediaioStatusOK;
}

MediaioStatus Unwrapper::seekAtTime(const double time)
{
	std::cout << "seek at time " << time << std::endl;
	return kMediaioStatusOK;
}

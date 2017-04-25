
#include "Wrapper.hpp"
#include <Ap4.h>

#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>

class Buffer: public AP4_MemoryByteStream
{
public:
	Buffer(size_t size = 0)
		: AP4_MemoryByteStream(size)
	{}

	AP4_Position tell()
	{
		AP4_Position position = 0;
		AP4_MemoryByteStream::Tell(position);
		return position;
	}

	~Buffer()
	{}
};

Wrapper::Wrapper()
	: _writer       (NULL)
	, _writerHandle (NULL)
	, _wrapper      ()
	, _video_stream (NULL)
	, _dts          (0)
	, _init         (false)
{}

Wrapper::~Wrapper()
{
	// if(_writer->is_open())
	{
		_writer->close(_writerHandle);
	}
}

MediaioStatus Wrapper::setWriter(MediaioPluginWriter* writer, void* writerHandle)
{
	_writer = writer;
	_writerHandle = writerHandle;
	return kMediaioStatusOK;
}

MediaioStatus Wrapper::configure(const Metadata* parameters)
{
	return kMediaioStatusOK;
}

MediaioStatus Wrapper::init(Buffer& buffer)
{
	if(_writer == NULL || _writerHandle == NULL)
	{
		return kMediaioStatusFailed;
	}

	MediaioStatus status = _writer->open(_writerHandle);
	if(status != kMediaioStatusOK)
	{
		return kMediaioStatusFailed;
	}

	unsigned int stream_type = AP4_MPEG2_STREAM_TYPE_HEVC;
	unsigned int stream_id   = AP4_MPEG2_TS_DEFAULT_STREAM_ID_VIDEO;

	int result = _wrapper.SetVideoStream(
		25, //video_track->GetMediaTimeScale(),
		stream_type,
		stream_id,
		_video_stream,
		AP4_MPEG2_TS_DEFAULT_PID_VIDEO);

	if (AP4_FAILED(result)) {
		fprintf(stderr, "could not create video stream (%d)\n", result);
		return kMediaioStatusFailed;
	}

	_wrapper.WritePAT(buffer);
	_wrapper.WritePMT(buffer);

	_dts = 0;
	return kMediaioStatusOK;
}

MediaioStatus Wrapper::wrapNextFrame(const int streamIndex, CodedData* wrappedFrame)
{
	if(wrappedFrame->size == 0)
	{
		return kMediaioStatusOK;
	}

	Buffer output_buffer(100000);

	if(!_init)
	{
		init(output_buffer);
		_init = true;
	}

	AP4_Size        size = wrappedFrame->size;
	AP4_Position    offset = 0;
	AP4_UI32        duration = 1;
	AP4_Ordinal     description_index = 0;
	AP4_UI64        dts = _dts;
	AP4_UI32        cts_delta = 0;
	bool            sync_flag = true;

	Buffer video_sample_data(size);

	AP4_Size wrote_bytes = 0;
	video_sample_data.WritePartial(wrappedFrame->data, size, wrote_bytes);
	video_sample_data.Seek(0);

	AP4_Sample video_sample = AP4_Sample(
		video_sample_data,
		offset,
		size,
		duration,
		description_index,
		dts,
		cts_delta,
		sync_flag
	);

	AP4_HvccAtom* hvccAtom = AP4_HvccAtom::Create(size, video_sample_data);

	AP4_HevcSampleDescription hevc_description(
		// AP4_SAMPLE_FORMAT_HEV1,
		AP4_SAMPLE_FORMAT_HVC1,
		1920,
		1080,
		8,
		"MediaIO - turingcodec",
		hvccAtom
	);

	int result = _video_stream->WriteSample(
		video_sample,
		&hevc_description,
		true,
		output_buffer);

	if(result != 0)
	{
		std::cout << AP4_ResultText(result) << std::endl;
		return kMediaioStatusFailed;
	}

	int generated_size = output_buffer.tell();
	_writer->write(_writerHandle, (char*)output_buffer.GetData(), generated_size);

	_dts++;
	std::cout << "wrote " << output_buffer.tell() << std::endl;
	return kMediaioStatusOK;
}

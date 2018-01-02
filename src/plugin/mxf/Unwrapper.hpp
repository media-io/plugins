#ifndef _MEDIAIO_PLUGIN_MXF_UNWRAPPER_HPP_
#define _MEDIAIO_PLUGIN_MXF_UNWRAPPER_HPP_

#include <mediaio/api/unwrapper/unwrapper.h>

#include <string>
#include <vector>

typedef std::vector<unsigned char> Key;

enum Tag
{
	InstanceUID,
	EssenceContainer,
	SampleRate,
	AudioSamplingRate,
	Locked,
	ChannelCount,
	QuantizationBits,
	AudioRefLevel,
	BlockAlign,
	AvgBps,
	ChannelStatusMode,
	FixedChannelStatusData,
	LinkedTrackID,
	ContainerDuration,

	Unknown,
};

class Unwrapper
{
	typedef std::vector<std::vector<unsigned char>> Buffer;

public:
	Unwrapper();
	~Unwrapper();

	MediaioStatus open(MediaioPluginReader* reader, void* readerHandle);
	MediaioStatus configure(const Metadata* parameters);
	MediaioStatus getDescription(struct MediaioFileDescriptor* descriptor);
	MediaioStatus getStreamDescription(const int streamIndex, struct MediaioStreamDescriptor* descriptor);
	MediaioStatus unwrapNextFrame(const int streamIndex, CodedData* unwrappedFrame);
	MediaioStatus seekAtFrame(const int frame);
	MediaioStatus seekAtTime(const double time);

private:
	bool is_mpeg2_video_descriptor_key(const Key& key);
	bool is_aes3_audio_descriptor_key(const Key& key);
	bool is_body_partition_key(const Key& key);
	bool is_video_essence_key(const Key& key);
	bool is_audio_essence_key(const Key& key);
	size_t get_audio_stream_index(const Key& key);

	Tag get_tag(int tag);
	int convert(const Key& data, size_t offset, size_t size);

	Key read_key();
	size_t read_length();

	MediaioPluginReader*                      _reader;
	std::vector<MediaioStreamDescriptor>      _descriptors;
	std::vector<MediaioAudioStreamDescriptor> _audio_descriptors;
	std::vector<MediaioImageStreamDescriptor> _image_descriptors;

	std::vector<Buffer>                       _buffers;

	void*                                     _readerHandle;
	bool                                      _init;
};

#endif

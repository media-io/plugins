
#include "Unwrapper.hpp"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>

Unwrapper::Unwrapper()
	: _reader       (NULL)
	, _readerHandle (NULL)
	, _init         (false)
{
}

Unwrapper::~Unwrapper()
{
	if(_init)
	{
		_reader->close(_readerHandle);
	}
}

MediaioStatus Unwrapper::open(MediaioPluginReader* reader, void* readerHandle)
{
	_reader = reader;
	_readerHandle = readerHandle;

	if(!_init)
	{
		_reader->open(_readerHandle);
		_init = true;
	}
	while(1)
	{
		auto key = read_key();
		if(key.size() != 16)
		{
			return kMediaioStatusFailed;
		}

		size_t value_length = read_length();

		if(is_mpeg2_video_descriptor_key(key))
		{
			MediaioStreamDescriptor descriptor;
			descriptor.kind = MediaioStreamKindImage;
			descriptor.codec = MediaioStreamCodecMpeg2;

			MediaioImageStreamDescriptor image_descriptor;
			image_descriptor.width = 1920;
			image_descriptor.height = 1080;
			_image_descriptors.push_back(image_descriptor);
			descriptor.audio = nullptr;
			descriptor.image = &_image_descriptors.back();
			_descriptors.push_back(descriptor);
		}

		if(is_aes3_audio_descriptor_key(key))
		{
			// std::cout << "AES3 Descriptor" << std::endl;
			std::vector<unsigned char> data(value_length, 0);
			_reader->read(_readerHandle, (char*)&data[0], value_length);

			int channel_count = -1;
			int sample_rate_num = -1;
			int sample_rate_den = -1;
			int container_duration = -1;
			int quantization_bits = -1;

			size_t tag_offset = 0;
			while(tag_offset < data.size())
			{
				int tag_value = convert(data, tag_offset, 2);
				tag_offset += 2;
				int length = convert(data, tag_offset, 2);
				tag_offset += 2;

				switch(get_tag(tag_value))
				{
					case Tag::InstanceUID: {
						break;
					}
					case Tag::EssenceContainer: {
						break;
					}
					case Tag::SampleRate: {
						sample_rate_num = convert(data, tag_offset, 4);
						sample_rate_den = convert(data, tag_offset + 4, 4);
						// std::cout << "SampleRate = " << num << "/" << den << std::endl;
						break;
					}
					case Tag::AudioSamplingRate: {
						// int num = convert(data, tag_offset, 4);
						// int den = convert(data, tag_offset + 4, 4);
						break;
					}
					case Tag::Locked: {
						break;
					}
					case Tag::ChannelCount: {
						channel_count = convert(data, tag_offset, 4);
						break;
					}
					case Tag::QuantizationBits: {
						quantization_bits = convert(data, tag_offset, 4);
						break;
					}
					case Tag::AudioRefLevel: {
						break;
					}
					case Tag::BlockAlign: {
						// int block_align = convert(data, tag_offset, 2);
						break;
					}
					case Tag::AvgBps: {
						// int avg_bps = convert(data, tag_offset, 4);
						break;
					}
					case Tag::LinkedTrackID: {
						break;
					}
					case Tag::ContainerDuration: {
						container_duration = convert(data, tag_offset, 8);
						break;
					}
					default: {
						// std::cout << "Unknown TAG" << std::endl;
					}
				}
				tag_offset += length;
			}

			MediaioStreamDescriptor descriptor;
			descriptor.kind = MediaioStreamKindAudio;
			descriptor.codec = MediaioStreamCodecPCM;

			MediaioAudioStreamDescriptor audio_descriptor;
			audio_descriptor.channel_count = channel_count;
			audio_descriptor.sample_rate_num = sample_rate_num;
			audio_descriptor.sample_rate_den = sample_rate_den;
			audio_descriptor.container_duration = container_duration;
			audio_descriptor.quantization_bits = quantization_bits;
			_audio_descriptors.push_back(audio_descriptor);

			descriptor.audio = &_audio_descriptors.back();
			descriptor.image = nullptr;
			_descriptors.push_back(descriptor);
			continue;
		}

		_reader->seek(_readerHandle, value_length);
		if(is_body_partition_key(key))
		{
			break;
		}
	}

	_buffers.resize(_descriptors.size());
	return kMediaioStatusOK;
}

MediaioStatus Unwrapper::configure(const Metadata* parameters)
{
	return kMediaioStatusOK;
}

MediaioStatus Unwrapper::getDescription(struct MediaioFileDescriptor* descriptor)
{
	descriptor->number_of_streams = _descriptors.size();
	return kMediaioStatusOK;
}

MediaioStatus Unwrapper::getStreamDescription(const int streamIndex, struct MediaioStreamDescriptor* descriptor)
{
	if(streamIndex > _descriptors.size())
	{
		return kMediaioStatusFailed;
	}

	descriptor->codec = _descriptors.at(streamIndex).codec;
	descriptor->kind = _descriptors.at(streamIndex).kind;
	descriptor->audio = _descriptors.at(streamIndex).audio;
	descriptor->image = _descriptors.at(streamIndex).image;

	return kMediaioStatusOK;
}

MediaioStatus Unwrapper::unwrapNextFrame(const int streamIndex, CodedData* codedData)
{
	if(!_init)
	{
		_reader->open(_readerHandle);
		_init = true;
	}
	if(_buffers.at(streamIndex).size() > 0)
	{
		auto& buffer = _buffers.at(streamIndex).front();
		resize_coded_data(codedData, buffer.size());
		memcpy(codedData->data, &buffer.at(0), buffer.size());

		_buffers.at(streamIndex).erase(_buffers.at(streamIndex).begin());

		// for (int i = 0; i < _buffers.size(); ++i)
		// {
		// 	std::cout << i << " = " << _buffers.at(i).size() << "; ";
		// }
		// std::cout << std::endl;
		return kMediaioStatusOK;
	}

	size_t value_length = 0;
	while(1)
	{
		auto key = read_key();
		if(key.size() != 16)
		{
			return kMediaioStatusFailed;
		}

		value_length = read_length();
		bool loaded_value = false;
		if(is_audio_essence_key(key))
		{
			size_t index = get_audio_stream_index(key) + 1;
			if(index == streamIndex && index == 1)
			{
				resize_coded_data(codedData, value_length);
				_reader->read(_readerHandle, (char*)codedData->data, value_length);
				loaded_value = true;
				return kMediaioStatusOK;
			}
			else
			{
				std::vector<unsigned char> data(value_length, 0);
				_reader->read(_readerHandle, (char*)&data[0], value_length);
				_buffers.at(index).push_back(data);
				loaded_value = true;
			}
		}
		if(is_video_essence_key(key))
		{
			if(streamIndex == 0)
			{
				resize_coded_data(codedData, value_length);
				_reader->read(_readerHandle, (char*)codedData->data, value_length);
				loaded_value = true;
				return kMediaioStatusOK;
			}
			else
			{
				std::vector<unsigned char> data(value_length, 0);
				_reader->read(_readerHandle, (char*)&data[0], value_length);
				_buffers.at(0).push_back(data);
				loaded_value = true;
			}
		}
		
		if(!loaded_value)
		{
			_reader->seek(_readerHandle, value_length);
		}
	}
}

MediaioStatus Unwrapper::seekAtFrame(const int frame)
{
	std::cout << "seek at frame " << frame << std::endl;
	return kMediaioStatusOK;
}

MediaioStatus Unwrapper::seekAtTime(const double time_position)
{
	std::cout << "seek at time " << time_position << std::endl;
	return kMediaioStatusOK;
}

typedef unsigned char uc;
template <uc U1, uc U2, uc U3, uc U4, uc U5, uc U6, uc U7, uc U8, uc U9, uc U10, uc U11, uc U12>
bool match_key(const Key& key)
{
	Key ref_key = {0x06, 0x0e, 0x2b, 0x34, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12};
	if(std::equal(ref_key.begin(), ref_key.end(), key.begin())){
		return true;
	}
	return false;
}

template <uc U1, uc U2, uc U3, uc U4, uc U5, uc U6, uc U7, uc U8, uc U9, uc U10, uc U11>
char match_key(const Key& key)
{
	Key ref_key = {0x06, 0x0e, 0x2b, 0x34, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11};
	if(std::equal(ref_key.begin(), ref_key.end(), key.begin())){
		return key.back();
	}
	return -1;
}

bool Unwrapper::is_mpeg2_video_descriptor_key(const Key& key)
{
	return match_key<0x02, 0x53, 0x01, 0x01, 0x0d, 0x01, 0x01, 0x01, 0x01, 0x01, 0x51, 0x00>(key);
}

bool Unwrapper::is_aes3_audio_descriptor_key(const Key& key)
{
	return match_key<0x02, 0x53, 0x01, 0x01, 0x0d, 0x01, 0x01, 0x01, 0x01, 0x01, 0x47, 0x00>(key);
}

bool Unwrapper::is_body_partition_key(const Key& key)
{
	return match_key<0x02, 0x05, 0x01, 0x01, 0x0d, 0x01, 0x02, 0x01, 0x01, 0x03, 0x04, 0x00>(key);
}

bool Unwrapper::is_video_essence_key(const Key& key)
{
	return match_key<0x01, 0x02, 0x01, 0x01, 0x0d, 0x01, 0x03, 0x01, 0x15, 0x01, 0x05, 0x00>(key);
}

bool Unwrapper::is_audio_essence_key(const Key& key)
{
	if(match_key<0x01, 0x02, 0x01, 0x01, 0x0d, 0x01, 0x03, 0x01, 0x16, 0x08, 0x03>(key) >= 0)
	{
		return true;
	}
	return false;
}

size_t Unwrapper::get_audio_stream_index(const Key& key)
{
	return key.back();
}

Key Unwrapper::read_key()
{
	Key key(16, 0);

	size_t size = _reader->read(_readerHandle, (char*)&key[0], 16);
	if(size != 16)
	{
		key.clear();
		return key;
	}
	return key;
}

size_t Unwrapper::read_length()
{
	unsigned char length = 0;
	_reader->read(_readerHandle, (char*)&length, 1);
	// std::cout << "read " << std::hex << (unsigned int)length << std::endl;
	if(length <= 0x7f)
	{
		return length;
	}
	else
	{
		Key long_length;
		long_length.resize(length - 0x80);
		_reader->read(_readerHandle, (char*)&long_length[0], long_length.size());
		size_t value_length = 0;
		for(size_t i = 0; i < long_length.size(); ++i)
		{
			int shift = long_length.size() - 1 - i;
			value_length += (long_length.at(i) << 8 * shift);
		}
		return value_length;
	}
}

Tag Unwrapper::get_tag(int tag)
{
	switch(tag)
	{
		case 0x3c0a: return Tag::InstanceUID;
		case 0x3004: return Tag::EssenceContainer;
		case 0x3001: return Tag::SampleRate;
		case 0x3d03: return Tag::AudioSamplingRate;
		case 0x3d02: return Tag::Locked;
		case 0x3d07: return Tag::ChannelCount;
		case 0x3d01: return Tag::QuantizationBits;
		case 0x3d04: return Tag::AudioRefLevel;
		case 0x3d0a: return Tag::BlockAlign;
		case 0x3d09: return Tag::AvgBps;
		case 0x3d10: return Tag::ChannelStatusMode;
		case 0x3d11: return Tag::FixedChannelStatusData;
		case 0x3006: return Tag::LinkedTrackID;
		case 0x3002: return Tag::ContainerDuration;
		default: return Tag::Unknown;
	}
}

int Unwrapper::convert(const Key& data, size_t offset, size_t size)
{
	int result = 0;
	for (int i = 0; i < size; ++i)
	{
		size_t shift = 8 * (size - i - 1);
		result += (((unsigned int)data[offset + i]) << shift);
	}
	return result;
}

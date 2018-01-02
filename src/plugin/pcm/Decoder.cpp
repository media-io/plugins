
#include "Decoder.hpp"
#include <iostream>
#include <cstdlib>
#include <cstring>

Decoder::Decoder()
{
}

Decoder::~Decoder()
{
}

MediaioStatus Decoder::configure(const Metadata* parameters)
{
	size_t index = 0;
	while( 1 )
	{
		const Metadata& m = parameters[index];
		if(m.type == eMetadataTypeEnd)
		{
			break;
		}
		++index;
	}
	return kMediaioStatusOK;
}

MediaioStatus Decoder::decode(CodedData* unwrappedFrame, AudioFrame* frame)
{
	size_t nb_channels = 1;
	create_channels(frame, nb_channels);

	for( size_t channelIndex = 0; channelIndex < nb_channels; ++ channelIndex )
	{
		Channel& channel = frame->channels[channelIndex];

		size_t samples = unwrappedFrame->size / 3;

		resize_channel(&channel, samples * 4);
		char* data = (char*)unwrappedFrame->data;
		int* output_data = (int*)channel.data;

		for (int s = 0; s < samples; ++s)
		{
			output_data[s] = (((int)data[s * 3 + 2]) << 24) + (((unsigned int)data[s * 3 + 1]) << 16) + (((unsigned int)data[s * 3 + 0]) << 8) + 0;
		}

		channel.precision = 32;
		channel.sampleSizeInByte = 4;
	}

	return kMediaioStatusOK;
}

Metadata* Decoder::getMetadatas()
{
	return nullptr;
}

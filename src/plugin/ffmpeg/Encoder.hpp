#ifndef _MEDIAIO_PLUGIN_FFMPEG_ENCODER_HPP_
#define _MEDIAIO_PLUGIN_FFMPEG_ENCODER_HPP_

#include <mediaio/api/encoder/encoder.h>

class Encoder
{
public:
	Encoder();
	~Encoder();

	MediaioStatus configure(const Metadata* parameters);
	MediaioStatus encode(ImageFrame* decodedFrame, CodedData* unwrappedFrame);
	Metadata* getMetadatas();
private:
};

#endif

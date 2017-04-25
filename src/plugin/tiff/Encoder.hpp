#ifndef _MEDIAIO_PLUGIN_TIFF_ENCODER_HPP_
#define _MEDIAIO_PLUGIN_TIFF_ENCODER_HPP_

#include <mediaio/api/encoder/encoder.h>

class Encoder
{
public:
	Encoder();
	~Encoder();

	MediaioStatus configure(const Metadata* parameters);
	MediaioStatus encode(Frame* decodedFrame, CodedData* unwrappedFrame);
	Metadata* getMetadatas();
private:
};

#endif

#ifndef _MEDIAIO_PLUGIN_TURINGCODEC_ENCODER_HPP_
#define _MEDIAIO_PLUGIN_TURINGCODEC_ENCODER_HPP_

#include <mediaio/api/encoder/encoder.h>

#include <vector>

typedef struct turing_encoder turing_encoder;

class Encoder
{
public:
	Encoder();
	~Encoder();

	MediaioStatus configure(const Metadata* parameters);
	MediaioStatus encode(Frame* decodedFrame, CodedData* unwrappedFrame);
	Metadata* getMetadatas();
private:

	bool init(Frame* frame);
	turing_encoder* _encoder;
	std::vector<uint8_t> _header;
	size_t _pts;
	bool _init;
};

#endif

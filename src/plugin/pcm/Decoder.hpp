#ifndef _MEDIAIO_PLUGIN_PCM_DECODER_HPP_
#define _MEDIAIO_PLUGIN_PCM_DECODER_HPP_

#include <mediaio/api/decoder/decoder.h>

typedef void * opj_codec_t;
struct opj_dparameters;

class Decoder
{
public:
	Decoder();
	~Decoder();

	MediaioStatus configure( const Metadata* parameters );

	MediaioStatus decode(CodedData* unwrappedFrame, AudioFrame* decodedFrame);

	Metadata* getMetadatas();

private:
};

#endif

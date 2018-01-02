#ifndef _MEDIAIO_PLUGIN_OPENJPEG_DECODER_HPP_
#define _MEDIAIO_PLUGIN_OPENJPEG_DECODER_HPP_

#include <mediaio/api/decoder/decoder.h>

typedef void * opj_codec_t;
struct opj_dparameters;

class Decoder
{
public:
	Decoder();
	~Decoder();

	MediaioStatus configure( const Metadata* parameters );

	MediaioStatus decode(CodedData* unwrappedFrame, ImageFrame* decodedFrame);

	Metadata* getMetadatas();

private:
	opj_codec_t*            _codec;
	struct opj_dparameters* _parameters;
	size_t                  _subresolution;
};

#endif

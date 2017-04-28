#ifndef _MEDIAIO_PLUGIN_TIFF_DECODER_HPP_
#define _MEDIAIO_PLUGIN_TIFF_DECODER_HPP_

#include <mediaio/api/decoder/decoder.h>

class Decoder
{
public:
	Decoder();
	~Decoder();

	MediaioStatus configure( const Metadata* parameters );
	MediaioStatus decode(CodedData* unwrappedFrame, Frame* decodedFrame);
	Metadata* getMetadatas();
};

#endif

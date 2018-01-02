#ifndef _MEDIAIO_PLUGIN_FFMPEG_DECODER_HPP_
#define _MEDIAIO_PLUGIN_FFMPEG_DECODER_HPP_

#include <mediaio/api/decoder/decoder.h>

struct AVCodecContext;

class Decoder
{
public:
	Decoder();
	~Decoder();

	MediaioStatus configure( const Metadata* parameters );
	MediaioStatus decode(CodedData* unwrappedFrame, ImageFrame* decodedFrame);
	Metadata* getMetadatas();

private:
	AVCodecContext* _context;
};

#endif

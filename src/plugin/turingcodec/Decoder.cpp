
#include "Decoder.hpp"

Decoder::Decoder()
{
}

Decoder::~Decoder()
{
}

MediaioStatus Decoder::configure( const Metadata* parameters )
{
	return kMediaioStatusOK;
}

MediaioStatus Decoder::decode(CodedData* unwrappedFrame, ImageFrame* decodedFrame)
{
	return kMediaioStatusOK;
}

Metadata* Decoder::getMetadatas()
{
	return nullptr;
}

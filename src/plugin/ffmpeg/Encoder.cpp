
#include "Encoder.hpp"

Encoder::Encoder()
{
}

Encoder::~Encoder()
{
}

MediaioStatus Encoder::configure(const Metadata* parameters)
{
	return kMediaioStatusOK;
}

MediaioStatus Encoder::encode(Frame* frame, CodedData* codedFrame)
{
	return kMediaioStatusOK;
}

Metadata* Encoder::getMetadatas()
{
	return nullptr;
}

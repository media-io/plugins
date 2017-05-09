
#include "Decoder.hpp"

#include <tiffio.h>
#include <tiffio.hxx>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>

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

MediaioStatus Decoder::decode(CodedData* unwrappedFrame, Frame* decodedFrame)
{
	std::string content((char*)unwrappedFrame->data, unwrappedFrame->size);
	std::istringstream inputTiffStream(content);
	TIFF* reader = TIFFStreamOpen("TiffReader", &inputTiffStream);

	if( reader == NULL )
	{
		return kMediaioStatusFailed;
	}

	size_t width = 0;
	size_t height = 0;
	size_t precision = 0;
	size_t components = 0;

	TIFFGetField(reader, TIFFTAG_IMAGEWIDTH, &width);
	TIFFGetField(reader, TIFFTAG_IMAGELENGTH, &height);
	TIFFGetField(reader, TIFFTAG_BITSPERSAMPLE, &precision);
	TIFFGetField(reader, TIFFTAG_SAMPLESPERPIXEL, &components);

	size_t size = precision / 8;

	create_components(decodedFrame, components);

	for(size_t index = 0; index < components; ++index)
	{
		Component& component = decodedFrame->components[index];
		resize_component(&component, width * height * precision);
		component.width = width;
		component.height = height;
		component.precision = precision;
		component.sampleSizeInByte = size;
		component.horizontalSubsampling = 0;
		component.verticalSubsampling = 0;
		component.signedData = false;
	}

	char* ptrR = (char*)decodedFrame->components[0].data;
	char* ptrG = (char*)decodedFrame->components[1].data;
	char* ptrB = (char*)decodedFrame->components[2].data;

	size_t scanlineSize = TIFFScanlineSize(reader);
	char tmpBuffer[scanlineSize];

	for(size_t row = 0; row < height; row++)
	{
		TIFFReadScanline(reader, tmpBuffer, row);
		char* tmpPtr = tmpBuffer;
		for(size_t col = 0; col < width; ++col)
		{
			memcpy(ptrR, tmpPtr, size);
			ptrR += size;
			tmpPtr += size;
			memcpy(ptrG, tmpPtr, size);
			ptrG += size;
			tmpPtr += size;
			memcpy(ptrB, tmpPtr, size);
			ptrB += size;
			tmpPtr += size;
		}
	}

	TIFFClose(reader);
	return kMediaioStatusOK;
}

Metadata* Decoder::getMetadatas()
{
	return nullptr;
}

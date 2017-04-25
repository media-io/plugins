
#include "Encoder.hpp"

#include <tiffio.h>
#include <tiffio.hxx>
#include <fstream>
#include <sstream>

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
	std::ostringstream outputTiffStream;
	TIFF* writer = TIFFStreamOpen("TiffWriter", &outputTiffStream);

	if(writer == NULL)
	{
		return kMediaioStatusFailed;
	}

	TIFFSetField(writer, TIFFTAG_XPOSITION, (float) 0);
	TIFFSetField(writer, TIFFTAG_YPOSITION, (float) 0);

	TIFFSetField(writer, TIFFTAG_IMAGEWIDTH, frame->components[0].width);
	TIFFSetField(writer, TIFFTAG_IMAGELENGTH, frame->components[0].height);
	TIFFSetField(writer, TIFFTAG_PIXAR_IMAGEFULLWIDTH, frame->components[0].width);
	TIFFSetField(writer, TIFFTAG_PIXAR_IMAGEFULLWIDTH, frame->components[0].height);
	TIFFSetField(writer, TIFFTAG_BITSPERSAMPLE, frame->components[0].precision);
	TIFFSetField(writer, TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_UINT);
	TIFFSetField(writer, TIFFTAG_SAMPLESPERPIXEL, frame->numberOfComponents);

	TIFFSetField(writer, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);

	TIFFSetField(writer, TIFFTAG_ROWSPERSTRIP,  frame->components[0].height);
	int orientation = ORIENTATION_TOPLEFT;
	TIFFSetField(writer, TIFFTAG_ORIENTATION, orientation);
	TIFFSetField(writer, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
	TIFFSetField(writer, TIFFTAG_COMPRESSION, COMPRESSION_NONE);

	TIFFCheckpointDirectory(writer);

	char* ptrR = (char*)frame->components[0].data;
	char* ptrG = (char*)frame->components[1].data;
	char* ptrB = (char*)frame->components[2].data;

	size_t scanlineSize = TIFFScanlineSize(writer);
	char tmpBuffer[ scanlineSize ];

	size_t size = frame->components[0].precision / 8;

	for(size_t row = 0; row < frame->components[0].height; row++)
	{
		char* tmpPtr = tmpBuffer;
		for(size_t col = 0; col < frame->components[0].width; ++col)
		{
			memcpy(tmpPtr, ptrR, size);
			ptrR += size;
			tmpPtr += size;
			memcpy(tmpPtr, ptrG, size);
			ptrG += size;
			tmpPtr += size;
			memcpy(tmpPtr, ptrB, size);
			ptrB += size;
			tmpPtr += size;
		}
		TIFFWriteScanline(writer, (tdata_t)tmpBuffer, row);
		TIFFCheckpointDirectory(writer);
	}

	resize_coded_data(codedFrame, outputTiffStream.tellp());
	memcpy(codedFrame->data, outputTiffStream.str().c_str(), codedFrame->size);

	TIFFClose(writer);
	return kMediaioStatusOK;
}

Metadata* Encoder::getMetadatas()
{
	return nullptr;
}


#include "Encoder.hpp"

#include <iostream>
#include <cstring>
#include <turing.h>

Encoder::Encoder()
	: _pts(0)
	, _init(false)
{
}

Encoder::~Encoder()
{
}

MediaioStatus Encoder::configure(const Metadata* parameters)
{
	return kMediaioStatusOK;
}

bool Encoder::init(Frame* frame)
{
	if(!frame || frame->numberOfComponents != 3) {
		return false;
	}

	const int width = frame->components[0].width;
	const int height = frame->components[0].height;
	const int bit_depth = frame->components[0].precision;


	char options[1024];
	char* s = options;
	char *end = &options[sizeof(options)];
	char const* argv[32];
	char const** p = argv;

	*p++ = s;
	*p++ = s += 1 + snprintf(s, end - s, "turing encode");
	*p++ = s += 1 + snprintf(s, end - s, "--input-res=%dx%d", width, height);
	*p++ = s += 1 + snprintf(s, end - s, "--frame-rate=%d", 24);
	*p++ = s += 1 + snprintf(s, end - s, "--frames=0");

	*p++ = s += 1 + snprintf(s, end - s, "--bit-depth=%d", bit_depth);
	*p++ = s += 1 + snprintf(s, end - s, "--internal-bit-depth=%d", bit_depth);

	int gop_n = 1;
	int gop_m = 1;

	*p++ = s += 1 + snprintf(s, end - s, "--max-gop-n=%d", gop_n);
	*p++ = s += 1 + snprintf(s, end - s, "--max-gop-m=%d", gop_m);

	*p++ = s += 1 + snprintf(s, end - s, "--verbosity=%d", 0);
	*p++ = s += 1 + snprintf(s, end - s, "--speed=fast");

	*p++ = s += 1 + snprintf(s, end - s, "dummy-input-filename");

	turing_encoder_settings settings;

	settings.argv = argv;
	settings.argc = p - argv - 1;

	try
	{
		_encoder = turing_create_encoder(settings);
		if (!_encoder) {
			std::cerr << "Failed to create libturing encoder." << std::endl;
			return false;
		}
	}
	catch(const std::runtime_error& error)
	{
		std::cout << "ERRORturing codec:" << error.what() << std::endl;
		return false;
	}
	catch(...)
	{
		std::cout << "ERROR: turing codec options" << std::endl;
		return false;
	}

	_pts = 0;
	return true;
}

MediaioStatus Encoder::encode(Frame* frame, CodedData* codedFrame)
{
	if(!_init)
	{
		if(!init(frame)) {
			return kMediaioStatusFailed;
		}
		_init = true;
	}

	turing_encoder_output const *output;

	if(frame) {
		turing_picture picture;
		size_t components = frame->numberOfComponents;

		for(size_t componentIndex = 0; componentIndex < components; ++ componentIndex)
		{
			Component& component = frame->components[componentIndex];

			picture.image[componentIndex].p = (uint8_t*) component.data;
			picture.image[componentIndex].stride = component.width;
		}

		picture.pts = _pts;

		_pts++;

		output = turing_encode_picture(_encoder, &picture);
	} else {
		output = turing_encode_picture(_encoder, 0);
	}

	if(output)
	{
		resize_coded_data(codedFrame, output->bitstream.size);
		memcpy(codedFrame->data, output->bitstream.p, output->bitstream.size);
	}
	return kMediaioStatusOK;
}


Metadata* Encoder::getMetadatas()
{
	return nullptr;
}

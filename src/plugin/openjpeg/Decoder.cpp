
#include "Decoder.hpp"
#include <openjpeg-2.1/openjpeg.h>
#include <iostream>
#include <cstdlib>

static void callback(const char *msg, void *client_data)
{
	// std::cout << "[callback] " << msg;
}

Decoder::Decoder()
	: _codec         (nullptr)
	, _parameters    (nullptr)
	, _subresolution (0)
{
}

Decoder::~Decoder()
{
}

MediaioStatus Decoder::configure(const Metadata* parameters)
{
	size_t index = 0;
	while( 1 )
	{
		const Metadata& m = parameters[index];
		if(m.type == eMetadataTypeEnd)
		{
			break;
		}
		if((!strcmp(m.id, "subresolution")) &&
			(m.type == eMetadataTypeString))
		{
			_subresolution = std::atoi((const char*)m.data);
		}
		++index;
	}
	return kMediaioStatusOK;
}

void opj_read_from_buffer(void* p_buffer, OPJ_SIZE_T p_nb_bytes, const char* codedFrame)
{
	memcpy(p_buffer, codedFrame, p_nb_bytes);
}

MediaioStatus Decoder::decode(CodedData* unwrappedFrame, Frame* decodedFrame)
{
	_codec = opj_create_decompress(OPJ_CODEC_J2K);
	opj_set_info_handler(_codec, callback, 0);
	opj_set_warning_handler(_codec, callback, 0);
	opj_set_error_handler(_codec, callback, 0);

	_parameters = new opj_dparameters_t();
	opj_set_default_decoder_parameters(_parameters);
	bool status = opj_setup_decoder(_codec, _parameters);

	if( !status )
	{
		std::cerr << "unable to set parameters to the decoder" << std::endl;
	}

	opj_stream_free_user_data_fn p_function = NULL;
	opj_image_t* decodedImage = NULL;

	opj_stream_t* stream = opj_stream_create(unwrappedFrame->size, OPJ_TRUE);

	if( ! stream )
	{
		std::cerr << "unable to set stream to decode it" << std::endl;
		return kMediaioStatusFailed;
	}

	opj_stream_set_user_data(stream, (void*)unwrappedFrame->data, p_function);
	opj_stream_set_user_data_length(stream, unwrappedFrame->size);

	opj_stream_set_read_function(stream, (opj_stream_read_fn) opj_read_from_buffer);

	if( _codec == NULL )
	{
		std::cerr << "unable to create codec decoder engine" << std::endl;
		return kMediaioStatusFailed;
	}

	opj_set_decoded_resolution_factor(_codec, _subresolution);

	bool readHeaderStatus = opj_read_header(stream, _codec, &decodedImage);
	if( !readHeaderStatus )
	{
		std::cerr << "unable to read header" << std::endl;
		return kMediaioStatusFailed;
	}

	bool decodingStatus = opj_decode(_codec, stream, decodedImage);
	if( !decodingStatus )
	{
		std::cerr << "unable to decode image" << std::endl;
		return kMediaioStatusFailed;
	}

	create_components(decodedFrame, decodedImage->numcomps);

	for( size_t componentIndex = 0; componentIndex < decodedImage->numcomps; ++ componentIndex )
	{
		Component& comp = decodedFrame->components[componentIndex];
		opj_image_comp_t& component = decodedImage->comps[componentIndex];

		size_t sampleSize = 4;
		if( component.prec <= 16 )
		{
			resize_component(&comp, component.w * component.h * sizeof(unsigned short));
			int* decodedPtr = component.data;
			unsigned short* dataPtr = (unsigned short*) comp.data;
			for( size_t index = 0; index < component.w * component.h; ++index )
			{
				*dataPtr = *decodedPtr;
				++decodedPtr;
				++dataPtr;
			}
			sampleSize = 2;
		}
		else
		{
			resize_component(&comp, component.w * component.h * sizeof(unsigned int) );
			memcpy( (void*)comp.data, component.data, comp.size);
		}


		comp.width = component.w;
		comp.height = component.h;
		comp.precision = component.prec;
		comp.sampleSizeInByte = sampleSize;
		comp.horizontalSubsampling = component.dx;
		comp.verticalSubsampling = component.dy;
		comp.signedData = ( component.sgnd == 1 );
		// std::cout << "precision " << component.prec << " | " << component.w << " x " << component.h << " -> " << comp.size << std::endl;
	}

	opj_stream_destroy(stream);
	opj_image_destroy(decodedImage);
	decodedImage = NULL;

	opj_destroy_codec(_codec);
	_codec = nullptr;

	delete _parameters;
	_parameters = nullptr;

	return kMediaioStatusOK;
}

Metadata* Decoder::getMetadatas()
{
	return nullptr;
}

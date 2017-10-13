
#include "Decoder.hpp"
#include <iostream>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libavutil/pixdesc.h>
}

Decoder::Decoder()
	: _context(nullptr)
{
}

Decoder::~Decoder()
{
}

MediaioStatus Decoder::configure(const Metadata* parameters)
{
	return kMediaioStatusOK;
}

std::string getDescriptionFromErrorCode(const int code)
{
	char err[AV_ERROR_MAX_STRING_SIZE];
	av_strerror(code, err, sizeof(err));
	return std::string(err);
}

static int decode(AVCodecContext *avctx, AVFrame *frame, int *got_frame, AVPacket *pkt)
{
	int ret;

	*got_frame = 0;

	if (pkt) {
		ret = avcodec_send_packet(avctx, pkt);
		// In particular, we don't expect AVERROR(EAGAIN), because we read all
		// decoded frames with avcodec_receive_frame() until done.

		if (ret < 0 && ret != AVERROR_EOF)
		{
			char err[AV_ERROR_MAX_STRING_SIZE];
			av_strerror(ret, err, sizeof(err));
			std::cout << "ERROR: send packet " << err << std::endl;
			return ret;
		}
	}

	ret = avcodec_receive_frame(avctx, frame);
	if (ret < 0 && ret != AVERROR(EAGAIN))
		return ret;
	if (ret >= 0)
		*got_frame = 1;
	return 0;
}

MediaioStatus Decoder::decode(CodedData* codedFrame, Frame* decodedFrame)
{
	if(!_context)
	{
		avcodec_register_all();
		AVCodec* codec = avcodec_find_decoder(AV_CODEC_ID_MPEG2VIDEO);
		if (!codec)
		{
			return kMediaioStatusFailed;
		}
		_context = avcodec_alloc_context3(codec);

		if(avcodec_open2(_context, codec, NULL) < 0)
		{
			return kMediaioStatusFailed;
		}
	}

	AVPacket packet;
	av_new_packet(&packet, codedFrame->size);
	av_packet_from_data(&packet, (uint8_t *)codedFrame->data, codedFrame->size);

	AVFrame* frame = av_frame_alloc();

	int got_frame = 0;

	if(::decode(_context, frame, &got_frame, &packet) < 0) {
		printf("An error occurred during video decoding\n");
		return kMediaioStatusFailed;
	}

	if(got_frame == 1) {

		int numcomps = 3;
		create_components(decodedFrame, numcomps);

		for(int componentIndex = 0; componentIndex < numcomps; ++componentIndex)
		{
			Component& comp = decodedFrame->components[componentIndex];

			int height = frame->height;
			int width = frame->width;
			int h_shift = 1;
			int v_shift = 1;

			if(componentIndex != 0){
				avcodec_get_chroma_sub_sample((enum AVPixelFormat) frame->format, &h_shift, &v_shift);

				height /= (h_shift + 1);
				width /= (v_shift + 1);
			}

			resize_component(&comp, width * height * sizeof(unsigned char));
			memcpy((void*)comp.data, frame->data[componentIndex], comp.size);

			comp.width = width;
			comp.height = height;
			comp.precision = 8;
			comp.sampleSizeInByte = 1;
			comp.horizontalSubsampling = h_shift;
			comp.verticalSubsampling = v_shift;
			comp.signedData = false;
		}
		av_frame_free(&frame);
		return kMediaioStatusOK;
	} else {
		// printf("no frame decoded\n");
	}
	return kMediaioStatusFailed;
}

Metadata* Decoder::getMetadatas()
{
	return nullptr;
}

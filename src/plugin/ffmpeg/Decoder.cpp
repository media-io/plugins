
#include "Decoder.hpp"

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

static int decode(AVCodecContext *avctx, AVFrame *frame, int *got_frame, AVPacket *pkt)
{
	int ret;

	*got_frame = 0;

	if (pkt) {
		ret = avcodec_send_packet(avctx, pkt);
		// In particular, we don't expect AVERROR(EAGAIN), because we read all
		// decoded frames with avcodec_receive_frame() until done.
		if (ret < 0 && ret != AVERROR_EOF)
			return ret;
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
		_context = avcodec_alloc_context3(codec);
	}

	AVPacket packet;
	av_new_packet(&packet, codedFrame->size);
	av_packet_from_data(&packet, (uint8_t *)codedFrame->data, codedFrame->size);

	AVFrame* frame = av_frame_alloc();

	int got_frame = 0;

	// int decode(AVCodecContext *avctx, AVFrame *frame, int *got_frame, AVPacket *pkt)

	if(::decode(_context, frame, &got_frame, &packet) < 0) {
		printf("An error occurred during video decoding");
		return kMediaioStatusFailed;
	}

	if(got_frame == 1) {

		// let f: AVPixelFormat = transmute((*frame).format);
		// println!("frame size : {}x{}", (*frame).width, (*frame).height);
		// println!("pict type  : {:?}", (*frame).pict_type);
		// println!("sar        : {:?}", (*frame).sample_aspect_ratio);
		// println!("format     : {:?}", f);
		// println!("data       : {:?}", (*frame).data);
		// println!("linesize   : {:?}", (*frame).linesize);

		// size_t data_size = frame->linesize[0] * frame->height;

		// std::vector<unsigned char> slice = std::vector(frame->data[0], data_size);

	} else {
		printf("no frame decoded");
	}

	av_frame_free(&frame);
	return kMediaioStatusOK;
}

Metadata* Decoder::getMetadatas()
{
	return nullptr;
}

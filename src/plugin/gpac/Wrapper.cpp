
#include "Wrapper.hpp"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>

#define AUDIO_OD_ESID	100
#define AUDIO_DATA_ESID	101
#define VIDEO_DATA_ESID	105

static GF_Err void_input_ctrl(GF_ESInterface *ifce, u32 act_type, void *param)
{
	return GF_OK;
}

Wrapper::Wrapper()
	: _writer       (NULL)
	, _writerHandle (NULL)
	, _init         (false)
{}

Wrapper::~Wrapper()
{
	// if(_writer->is_open())
	{
		_writer->close(_writerHandle);
	}
}

MediaioStatus Wrapper::open(MediaioPluginWriter* writer, void* writerHandle)
{
	_writer = writer;
	_writerHandle = writerHandle;

	return kMediaioStatusOK;
}

MediaioStatus Wrapper::configure(const Metadata* parameters)
{
	return kMediaioStatusOK;
}

MediaioStatus Wrapper::init()
{
	if(_writer == NULL || _writerHandle == NULL)
	{
		return kMediaioStatusFailed;
	}

	MediaioStatus status = _writer->open(_writerHandle);
	if(status != kMediaioStatusOK)
	{
		return kMediaioStatusFailed;
	}

	/*****************/
	/*   gpac init   */
	/*****************/
	gf_sys_init(GF_MemTrackerNone);
	// gf_log_set_tool_level(GF_LOG_ALL, GF_LOG_WARNING);
	gf_log_set_tool_level(GF_LOG_ALL, GF_LOG_DEBUG);


	int mux_rate = 0;
	int psi_refresh_rate = GF_M2TS_PSI_DEFAULT_REFRESH_RATE;
	Bool real_time = GF_FALSE;

	_muxer = gf_m2ts_mux_new(mux_rate, psi_refresh_rate, real_time);
	if (!_muxer) {
		std::cerr << "Could not create the muxer" << std::endl;
		return kMediaioStatusFailed;
	}

	GF_M2TS_PackMode pes_packing_mode = GF_M2TS_PACK_AUDIO_ONLY;
	int pcr_ms = 100;
	Bool enable_forced_pcr = GF_FALSE;

	gf_m2ts_mux_use_single_au_pes_mode(_muxer, pes_packing_mode);
	// if (pcr_init_val>=0) gf_m2ts_mux_set_initial_pcr(_muxer, (u64) pcr_init_val);
	gf_m2ts_mux_set_pcr_max_interval(_muxer, pcr_ms);
	gf_m2ts_mux_enable_pcr_only_packets(_muxer, enable_forced_pcr);


	/****************************************/
	/*   declare all streams to the muxer   */
	/****************************************/
	int cur_pid = 100; /*PIDs start from 100*/
	int program_id = 1;
	u32 pcr_offset = (u32) -1;

	u32 prog_pcr_offset = 0;
	if (pcr_offset==(u32)-1) {
		Double r = 8 * 8;
		r *= 90000;
		r/= mux_rate;
		//add 10% of safety to cover TS signaling and other potential table update while sending the largest PES
		r *= 1.1;
		prog_pcr_offset = (u32) r;
	} else {
		prog_pcr_offset = pcr_offset;
	}
	std::cout << "Setting up program ID " << program_id << " - send rates: PSI " << psi_refresh_rate << " ms PCR " << pcr_ms << " ms - PCR offset " << prog_pcr_offset << std::endl;
	_program = gf_m2ts_mux_program_add(_muxer, program_id, cur_pid, psi_refresh_rate, prog_pcr_offset, GF_TRUE);

	_interface.stream_type = GF_STREAM_VISUAL;
	_interface.object_type_indication = GPAC_OTI_VIDEO_AVC;
	_interface.input_ctrl = void_input_ctrl;
	_interface.stream_id = VIDEO_DATA_ESID;
	_interface.timescale = 1000;

	_stream = gf_m2ts_program_stream_add(_program, &_interface, cur_pid + 1, GF_TRUE, GF_FALSE);
	gf_m2ts_mux_update_config(_muxer, GF_TRUE);

	// if (nb_pck_pack>1) {
	// 	ts_pack_buffer = gf_malloc(sizeof(char) * 188 * nb_pck_pack);
	// }

	return kMediaioStatusOK;
}

MediaioStatus Wrapper::wrapNextFrame(const int streamIndex, CodedData* wrappedFrame)
{
	if(!_init)
	{
		init();
		_init = true;
	}

	GF_ESIPacket pck;
	memset(&pck, 0, sizeof(GF_ESIPacket));
	pck.data = (char*)wrappedFrame->data;
	pck.data_len = wrappedFrame->size;
	pck.flags |= GF_ESI_DATA_HAS_CTS;
	pck.flags |= GF_ESI_DATA_HAS_DTS;
	pck.flags |= GF_ESI_DATA_AU_START;
	pck.flags |= GF_ESI_DATA_AU_END;
	// if (ts) pck.cts = pck.dts = ts;

	// if (priv->rap)
	// 	pck.flags |= GF_ESI_DATA_AU_RAP;
	// if (source->repeat || !priv->vers_inc) {
	// 	pck.flags |= GF_ESI_DATA_REPEAT;
	// 	fprintf(stderr, "RAP carousel from scene engine sent: ESID=%d - size=%d - ts="LLD"\n", ESID, size, ts);
	// } else {
	// 	if (ESID != AUDIO_DATA_ESID && ESID != VIDEO_DATA_ESID)	/*don't log A/V inputs*/
	// 		fprintf(stderr, "Update from scene engine sent: ESID=%d - size=%d - ts="LLD"\n", ESID, size, ts);
	// }

	_interface.output_ctrl(&_interface, GF_ESI_OUTPUT_DATA_DISPATCH, &pck);

	const char *ts_pck = NULL;
	u32 status = 0;
	u32 usec_till_next = 0;

	while((ts_pck = gf_m2ts_mux_process(_muxer, &status, &usec_till_next))){
		if(ts_pck){
			_writer->write(_writerHandle, (const char*)ts_pck, 188);
		}
	};

	return kMediaioStatusOK;
}

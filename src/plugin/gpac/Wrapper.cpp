
#include "Wrapper.hpp"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>

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

	return kMediaioStatusOK;
}

MediaioStatus Wrapper::wrapNextFrame(const int streamIndex, CodedData* wrappedFrame)
{
	if(!_init)
	{
		init();
		_init = true;
	}
	return kMediaioStatusOK;
}

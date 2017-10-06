#ifndef VAPOURSYNTH_YADIF_MOD2_H
#define VAPOURSYNTH_YADIF_MOD2_H

#include <cstdint>
#include "arch.h"

typedef void(*proc_filter_t)(
    const uint8_t* currp, const uint8_t* prevp, const uint8_t* nextp,
    const uint8_t* fm_prev, const uint8_t* fm_next, const uint8_t* edeintp,
    uint8_t* dstp, size_t width, int cstride, int pstride, int nstride,
    int fm_pstride, int fm_nstride, int estride2, int dstride2, const int count);

typedef void(*interpolate_t)(uint8_t* dstp, const uint8_t* srcp, int stride, size_t width);

proc_filter_t get_main_proc(int bits_per_sample, bool spcheck, bool edeint, arch_t arch);

interpolate_t get_interp(int bytes_per_sample);

#endif

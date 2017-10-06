/*
    yadifmod2 : yadif + yadifmod for VapourSynth
        Copyright (C) 2016 OKA Motofumi

    yadifmod : Modification of Fizick's yadif avisynth filter.
        Copyright (C) 2007 Kevin Stone aka tritical

    Yadif C-plugin for Avisynth 2.5 : Port from MPlayer filter
        Copyright (C) 2007 Alexander G. Balakhnin aka Fizick

    YADIF : Yet Another DeInterlacing Filter
        Copyright (C) 2006 Michael Niedermayer <michaelni@gmx.at>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/


// #include <algorithm>
// #include <cstring>
// #include <map>
// #include <tuple>
// #include "yadifmod2.h"




// YadifMod2::YadifMod2(VSNodeRef* c, VSNodeRef* e, int o, int f, int m,
//                      arch_t arch, VSCore* core, const VSAPI* api) :
//     clip(c), eclip(e), order(o), field(f), mode(m)
// {
//     vi = *api->getVideoInfo(clip);

//     nfSrc = vi.numFrames - 1;

//     if (mode == 1 || mode == 3) {
//         vi.numFrames *= 2;
//         set_fps(vi, vi.fpsNum * 2, vi.fpsDen);
//     }

//     if (field == -1) {
//         field = order;
//     }

//     prevFirst = nfSrc == 0 ? 0 : eclip ? 0 : 1;

//     interp = get_interp(vi.format->bytesPerSample);

//     int bps = vi.format->bitsPerSample;
//     if (bps == 9) {
//         bps = 10;
//     }
//     if (arch == NO_SIMD && bps == 10) {
//         bps = 16;
//     }
//     if (bps == 32) {
//         arch = arch < USE_AVX ? USE_SSE2 : USE_AVX;
//     }

//     mainProc = get_main_proc(bps, mode < 2, eclip != nullptr, arch);
// }


// void YadifMod2::requestFrames(int n, const VSAPI* api, VSFrameContext* ctx)
// {
//     if (mode == 1 || mode == 3) {
//         n /= 2;
//     }

//     api->requestFrameFilter(n == 0 ? prevFirst : n - 1, clip, ctx);
//     api->requestFrameFilter(n, clip, ctx);
//     api->requestFrameFilter(std::min(n + 1, nfSrc), clip, ctx);

//     if (eclip) {
//         api->requestFrameFilter(n, eclip, ctx);
//     }
// }


// const VSFrameRef* YadifMod2::
// getFrame(int n, VSCore* core, const VSAPI* api, VSFrameContext* ctx)
// {
//     int ft = field;
//     if (mode == 1 || mode == 3) {
//         ft = (n & 1) ? 1 - order : order;
//         n /= 2;
//     }

//     auto edeint = eclip ? api->getFrameFilter(n, eclip, ctx)
//                 : nullptr;
//     auto curr = api->getFrameFilter(n, clip, ctx);
//     auto prev = api->getFrameFilter(n == 0 ? prevFirst : n - 1, clip, ctx);
//     auto next = api->getFrameFilter(std::min(n + 1, nfSrc), clip, ctx);
//     auto dst = api->newVideoFrame(vi.format, vi.width, vi.height, curr, core);

//     for (int p = 0; p < vi.format->numPlanes; ++p) {
//         auto currp = api->getReadPtr(curr, p);
//         auto prevp = api->getReadPtr(prev, p);
//         auto nextp = api->getReadPtr(next, p);

//         size_t width = api->getFrameWidth(curr, p);
//         size_t rowsize = width * vi.format->bytesPerSample;
//         size_t height = api->getFrameHeight(curr, p);
//         int cstride = api->getStride(curr, p);
//         int pstride = api->getStride(prev, p);
//         int nstride = api->getStride(next, p);

//         int begin = 2 + ft;
//         int count = (height - 4 + ft - begin) / 2 + 1;

//         const uint8_t *fm_prev, *fm_next;
//         int fm_pstride, fm_nstride;
//         if (ft != order) {
//             fm_pstride = cstride * 2;
//             fm_nstride = nstride * 2;
//             fm_prev = prevp + begin + cstride;
//             fm_next = nextp + begin * nstride;
//         } else {
//             fm_pstride = pstride * 2;
//             fm_nstride = cstride * 2;
//             fm_prev = prevp + begin * pstride;
//             fm_next = currp + begin * cstride;
//         }

//         const uint8_t* edeintp = nullptr;
//         int estride = 0;
//         if (eclip) {
//             edeintp = api->getReadPtr(edeint, p);
//             estride = api->getStride(edeint, p);
//         }

//         auto dstp = api->getWritePtr(dst, p);
//         int dstride = api->getStride(dst, p);

//         if (ft == 0) {
//             memcpy(dstp, currp + cstride, rowsize);
//             if (edeintp) {
//                 memcpy(dstp + dstride * (height - 2),
//                        edeintp + estride * (height - 2), rowsize);
//             } else {
//                 interp(dstp + dstride * (height - 2),
//                        currp + cstride * (height - 3), cstride, width);
//             }
//         } else {
//             if (edeintp) {
//                 memcpy(dstp + dstride, edeintp + estride, rowsize);
//             } else {
//                 interp(dstp + dstride, currp, cstride, width);
//             }
//             memcpy(dstp + dstride * (height - 1),
//                    currp + cstride * (height - 2), rowsize);
//         }
//         bitblt(dstp + (1 - ft) * dstride, dstride * 2,
//                currp + (1 - ft) * cstride, cstride * 2, rowsize, height / 2);

//         mainProc(currp + begin * cstride, prevp + begin * pstride,
//                  nextp + begin * nstride, fm_prev, fm_next,
//                  edeintp + begin * estride, dstp + begin * dstride, width,
//                  cstride, pstride, nstride, fm_pstride, fm_nstride, estride * 2,
//                  dstride * 2, count);
//     }

//     return dst;

// }
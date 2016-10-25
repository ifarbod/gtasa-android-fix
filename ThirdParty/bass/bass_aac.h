#ifndef BASSAAC_H
#define BASSAAC_H

#include "bass.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef BASSAACDEF
#define BASSAACDEF(f) WINAPI f
#endif

// additional error codes returned by BASS_ErrorGetCode
#define BASS_ERROR_MP4_NOSTREAM	6000 // non-streamable due to MP4 atom order ("mdat" before "moov")

// additional BASS_SetConfig options
#define BASS_CONFIG_MP4_VIDEO	0x10700 // play the audio from MP4 videos
#define BASS_CONFIG_AAC_MP4		0x10701 // support MP4 in BASS_AAC_StreamCreateXXX functions (no need for BASS_MP4_StreamCreateXXX)
#define BASS_CONFIG_AAC_PRESCAN	0x10702 // pre-scan ADTS AAC files for seek points and accurate length

// additional BASS_AAC_StreamCreateFile/etc flags
#define BASS_AAC_FRAME960		0x1000 // 960 samples per frame
#define BASS_AAC_STEREO			0x400000 // downmatrix to stereo

// BASS_CHANNELINFO type
#define BASS_CTYPE_STREAM_AAC	0x10b00 // AAC
#define BASS_CTYPE_STREAM_MP4	0x10b01 // AAC in MP4


HSTREAM BASSAACDEF(BASS_AAC_StreamCreateFile)(BOOL mem, const void *file, QWORD offset, QWORD length, DWORD flags);
HSTREAM BASSAACDEF(BASS_AAC_StreamCreateURL)(const char *url, DWORD offset, DWORD flags, DOWNLOADPROC *proc, void *user);
HSTREAM BASSAACDEF(BASS_AAC_StreamCreateFileUser)(DWORD system, DWORD flags, const BASS_FILEPROCS *procs, void *user);
HSTREAM BASSAACDEF(BASS_MP4_StreamCreateFile)(BOOL mem, const void *file, QWORD offset, QWORD length, DWORD flags);
HSTREAM BASSAACDEF(BASS_MP4_StreamCreateFileUser)(DWORD system, DWORD flags, const BASS_FILEPROCS *procs, void *user);

#ifdef __cplusplus
}
#endif

#endif

/*
 * Copyright (C) 2002 Alexandre Julliard
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef OUR_GUID_ENTRY
#define OUR_GUID_ENTRY(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
           DEFINE_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8);
#endif

#define MEDIATYPE_NULL       GUID_NULL
#define MEDIASUBTYPE_NULL    GUID_NULL

OUR_GUID_ENTRY(MEDIASUBTYPE_None,                    0xe436eb8e, 0x524f, 0x11ce, 0x9f, 0x53, 0x00, 0x20, 0xaf, 0x0b, 0xa7, 0x70)
OUR_GUID_ENTRY(MEDIATYPE_Video,                      0x73646976, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71)
OUR_GUID_ENTRY(MEDIATYPE_Audio,                      0x73647561, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71)
OUR_GUID_ENTRY(MEDIATYPE_Text,                       0x73747874, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71)
OUR_GUID_ENTRY(MEDIATYPE_Midi,                       0x7364696d, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71)
OUR_GUID_ENTRY(MEDIATYPE_Stream,                     0xe436eb83, 0x524f, 0x11ce, 0x9f, 0x53, 0x00, 0x20, 0xaf, 0x0b, 0xa7, 0x70)
OUR_GUID_ENTRY(MEDIATYPE_Interleaved,                0x73766169, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71)
OUR_GUID_ENTRY(MEDIATYPE_File,                       0x656c6966, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71)
OUR_GUID_ENTRY(MEDIATYPE_ScriptCommand,              0x73636d64, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71)
OUR_GUID_ENTRY(MEDIATYPE_AUXLine21Data,              0x670aea80, 0x3a82, 0x11d0, 0xb7, 0x9b, 0x00, 0xaa, 0x00, 0x37, 0x67, 0xa7)
OUR_GUID_ENTRY(MEDIATYPE_Timecode,                   0x0482dee3, 0x7817, 0x11cf, 0x8a, 0x03, 0x00, 0xaa, 0x00, 0x6e, 0xcb, 0x65)
OUR_GUID_ENTRY(MEDIATYPE_LMRT,                       0x74726c6d, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71)
OUR_GUID_ENTRY(MEDIATYPE_URL_STREAM,                 0x736c7275, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71)
OUR_GUID_ENTRY(MEDIASUBTYPE_YVU9,                    0x39555659, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71)
OUR_GUID_ENTRY(MEDIASUBTYPE_Y411,                    0x31313459, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71)
OUR_GUID_ENTRY(MEDIASUBTYPE_Y41P,                    0x50313459, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71)
OUR_GUID_ENTRY(MEDIASUBTYPE_YUY2,                    0x32595559, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71)
OUR_GUID_ENTRY(MEDIASUBTYPE_YVYU,                    0x55595659, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71)
OUR_GUID_ENTRY(MEDIASUBTYPE_UYVY,                    0x59565955, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71)
OUR_GUID_ENTRY(MEDIASUBTYPE_Y211,                    0x31313259, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71)
OUR_GUID_ENTRY(MEDIASUBTYPE_YV12,                    0x32315659, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71)
OUR_GUID_ENTRY(MEDIASUBTYPE_CLJR,                    0x524a4c43, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71)
OUR_GUID_ENTRY(MEDIASUBTYPE_IF09,                    0x39304649, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71)
OUR_GUID_ENTRY(MEDIASUBTYPE_CPLA,                    0x414c5043, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71)
OUR_GUID_ENTRY(MEDIASUBTYPE_MJPG,                    0x47504a4d, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71)
OUR_GUID_ENTRY(MEDIASUBTYPE_TVMJ,                    0x4a4d5654, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71)
OUR_GUID_ENTRY(MEDIASUBTYPE_WAKE,                    0x454b4157, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71)
OUR_GUID_ENTRY(MEDIASUBTYPE_CFCC,                    0x43434643, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71)
OUR_GUID_ENTRY(MEDIASUBTYPE_IJPG,                    0x47504a49, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71)
OUR_GUID_ENTRY(MEDIASUBTYPE_Plum,                    0x6d756c50, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71)
OUR_GUID_ENTRY(MEDIASUBTYPE_DVCS,                    0x53435644, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71)
OUR_GUID_ENTRY(MEDIASUBTYPE_DVSD,                    0x44535644, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71)
OUR_GUID_ENTRY(MEDIASUBTYPE_MDVF,                    0x4656444d, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71)
OUR_GUID_ENTRY(MEDIASUBTYPE_RGB1,                    0xe436eb78, 0x524f, 0x11ce, 0x9f, 0x53, 0x00, 0x20, 0xaf, 0x0b, 0xa7, 0x70)
OUR_GUID_ENTRY(MEDIASUBTYPE_RGB4,                    0xe436eb79, 0x524f, 0x11ce, 0x9f, 0x53, 0x00, 0x20, 0xaf, 0x0b, 0xa7, 0x70)
OUR_GUID_ENTRY(MEDIASUBTYPE_RGB8,                    0xe436eb7a, 0x524f, 0x11ce, 0x9f, 0x53, 0x00, 0x20, 0xaf, 0x0b, 0xa7, 0x70)
OUR_GUID_ENTRY(MEDIASUBTYPE_RGB565,                  0xe436eb7b, 0x524f, 0x11ce, 0x9f, 0x53, 0x00, 0x20, 0xaf, 0x0b, 0xa7, 0x70)
OUR_GUID_ENTRY(MEDIASUBTYPE_RGB555,                  0xe436eb7c, 0x524f, 0x11ce, 0x9f, 0x53, 0x00, 0x20, 0xaf, 0x0b, 0xa7, 0x70)
OUR_GUID_ENTRY(MEDIASUBTYPE_RGB24,                   0xe436eb7d, 0x524f, 0x11ce, 0x9f, 0x53, 0x00, 0x20, 0xaf, 0x0b, 0xa7, 0x70)
OUR_GUID_ENTRY(MEDIASUBTYPE_RGB32,                   0xe436eb7e, 0x524f, 0x11ce, 0x9f, 0x53, 0x00, 0x20, 0xaf, 0x0b, 0xa7, 0x70)
OUR_GUID_ENTRY(MEDIASUBTYPE_Overlay,                 0xe436eb7f, 0x524f, 0x11ce, 0x9f, 0x53, 0x00, 0x20, 0xaf, 0x0b, 0xa7, 0x70)
OUR_GUID_ENTRY(MEDIASUBTYPE_MPEG1Packet,             0xe436eb80, 0x524f, 0x11ce, 0x9f, 0x53, 0x00, 0x20, 0xaf, 0x0b, 0xa7, 0x70)
OUR_GUID_ENTRY(MEDIASUBTYPE_MPEG1Payload,            0xe436eb81, 0x524f, 0x11ce, 0x9f, 0x53, 0x00, 0x20, 0xaf, 0x0b, 0xa7, 0x70)
OUR_GUID_ENTRY(MEDIASUBTYPE_MPEG1AudioPayload,       0x00000050, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71)
OUR_GUID_ENTRY(MEDIATYPE_MPEG1SystemStream,          0xe436eb82, 0x524f, 0x11ce, 0x9f, 0x53, 0x00, 0x20, 0xaf, 0x0b, 0xa7, 0x70)
OUR_GUID_ENTRY(MEDIASUBTYPE_MPEG1System,             0xe436eb84, 0x524f, 0x11ce, 0x9f, 0x53, 0x00, 0x20, 0xaf, 0x0b, 0xa7, 0x70)
OUR_GUID_ENTRY(MEDIASUBTYPE_MPEG1VideoCD,            0xe436eb85, 0x524f, 0x11ce, 0x9f, 0x53, 0x00, 0x20, 0xaf, 0x0b, 0xa7, 0x70)
OUR_GUID_ENTRY(MEDIASUBTYPE_MPEG1Video,              0xe436eb86, 0x524f, 0x11ce, 0x9f, 0x53, 0x00, 0x20, 0xaf, 0x0b, 0xa7, 0x70)
OUR_GUID_ENTRY(MEDIASUBTYPE_MPEG1Audio,              0xe436eb87, 0x524f, 0x11ce, 0x9f, 0x53, 0x00, 0x20, 0xaf, 0x0b, 0xa7, 0x70)
OUR_GUID_ENTRY(MEDIASUBTYPE_Avi,                     0xe436eb88, 0x524f, 0x11ce, 0x9f, 0x53, 0x00, 0x20, 0xaf, 0x0b, 0xa7, 0x70)
OUR_GUID_ENTRY(MEDIASUBTYPE_Asf,                     0x3db80f90, 0x9412, 0x11d1, 0xad, 0xed, 0x00, 0x00, 0xf8, 0x75, 0x4b, 0x99)
OUR_GUID_ENTRY(MEDIASUBTYPE_QTMovie,                 0xe436eb89, 0x524f, 0x11ce, 0x9f, 0x53, 0x00, 0x20, 0xaf, 0x0b, 0xa7, 0x70)
OUR_GUID_ENTRY(MEDIASUBTYPE_QTRpza,                  0x617a7072, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71)
OUR_GUID_ENTRY(MEDIASUBTYPE_QTSmc,                   0x20636d73, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71)
OUR_GUID_ENTRY(MEDIASUBTYPE_QTRle,                   0x20656c72, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71)
OUR_GUID_ENTRY(MEDIASUBTYPE_QTJpeg,                  0x6765706a, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71)
OUR_GUID_ENTRY(MEDIASUBTYPE_PCMAudio_Obsolete,       0xe436eb8a, 0x524f, 0x11ce, 0x9f, 0x53, 0x00, 0x20, 0xaf, 0x0b, 0xa7, 0x70)
OUR_GUID_ENTRY(MEDIASUBTYPE_PCM,                     0x00000001, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71)
OUR_GUID_ENTRY(MEDIASUBTYPE_WAVE,                    0xe436eb8b, 0x524f, 0x11ce, 0x9f, 0x53, 0x00, 0x20, 0xaf, 0x0b, 0xa7, 0x70)
OUR_GUID_ENTRY(MEDIASUBTYPE_AU,                      0xe436eb8c, 0x524f, 0x11ce, 0x9f, 0x53, 0x00, 0x20, 0xaf, 0x0b, 0xa7, 0x70)
OUR_GUID_ENTRY(MEDIASUBTYPE_AIFF,                    0xe436eb8d, 0x524f, 0x11ce, 0x9f, 0x53, 0x00, 0x20, 0xaf, 0x0b, 0xa7, 0x70)
OUR_GUID_ENTRY(MEDIASUBTYPE_dvsd,                    0x64737664, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71)
OUR_GUID_ENTRY(MEDIASUBTYPE_dvhd,                    0x64687664, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71)
OUR_GUID_ENTRY(MEDIASUBTYPE_dvsl,                    0x6c737664, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71)
OUR_GUID_ENTRY(MEDIASUBTYPE_Line21_BytePair,         0x6e8d4a22, 0x310c, 0x11d0, 0xb7, 0x9a, 0x00, 0xaa, 0x00, 0x37, 0x67, 0xa7)
OUR_GUID_ENTRY(MEDIASUBTYPE_Line21_GOPPacket,        0x6e8d4a23, 0x310c, 0x11d0, 0xb7, 0x9a, 0x00, 0xaa, 0x00, 0x37, 0x67, 0xa7)
OUR_GUID_ENTRY(MEDIASUBTYPE_Line21_VBIRawData,       0x6e8d4a24, 0x310c, 0x11d0, 0xb7, 0x9a, 0x00, 0xaa, 0x00, 0x37, 0x67, 0xa7)
OUR_GUID_ENTRY(MEDIASUBTYPE_DssVideo,                0xa0af4f81, 0xe163, 0x11d0, 0xba, 0xd9, 0x00, 0x60, 0x97, 0x44, 0x11, 0x1a)
OUR_GUID_ENTRY(MEDIASUBTYPE_DssAudio,                0xa0af4f82, 0xe163, 0x11d0, 0xba, 0xd9, 0x00, 0x60, 0x97, 0x44, 0x11, 0x1a)
OUR_GUID_ENTRY(MEDIASUBTYPE_VPVideo,                 0x5a9b6a40, 0x1a22, 0x11d1, 0xba, 0xd9, 0x00, 0x60, 0x97, 0x44, 0x11, 0x1a)
OUR_GUID_ENTRY(MEDIASUBTYPE_VPVBI,                   0x5a9b6a41, 0x1a22, 0x11d1, 0xba, 0xd9, 0x00, 0x60, 0x97, 0x44, 0x11, 0x1a)
OUR_GUID_ENTRY(IID_IAMCutListElement,                0xcde29520, 0x3418, 0x11cf, 0xa5, 0xb0, 0x00, 0x20, 0xaf, 0x05, 0x3d, 0x8f)
OUR_GUID_ENTRY(IID_IAMFileCutListElement,            0xf0947070, 0x276c, 0x11d0, 0x83, 0x16, 0x00, 0x20, 0xaf, 0x11, 0xc0, 0x10)
OUR_GUID_ENTRY(IID_IAMVideoCutListElement,           0xcde29522, 0x3418, 0x11cf, 0xa5, 0xb0, 0x00, 0x20, 0xaf, 0x05, 0x3d, 0x8f)
OUR_GUID_ENTRY(IID_IAMAudioCutListElement,           0xcde29524, 0x3418, 0x11cf, 0xa5, 0xb0, 0x00, 0x20, 0xaf, 0x05, 0x3d, 0x8f)
OUR_GUID_ENTRY(CLSID_CutListSource,                  0xa5ea8d20, 0x253d, 0x11d1, 0xb3, 0xf1, 0x00, 0xaa, 0x00, 0x37, 0x61, 0xc5)
OUR_GUID_ENTRY(IID_IStandardCutList,                 0xa5ea8d29, 0x253d, 0x11d1, 0xb3, 0xf1, 0x00, 0xaa, 0x00, 0x37, 0x61, 0xc5)
OUR_GUID_ENTRY(IID_IFileClip,                        0xa5ea8d2a, 0x253d, 0x11d1, 0xb3, 0xf1, 0x00, 0xaa, 0x00, 0x37, 0x61, 0xc5)
OUR_GUID_ENTRY(IID_ICutListGraphBuilder,             0xa5ea8d2c, 0x253d, 0x11d1, 0xb3, 0xf1, 0x00, 0xaa, 0x00, 0x37, 0x61, 0xc5)
OUR_GUID_ENTRY(CLSID_CutListGraphBuilder,            0xa5ea8d2f, 0x253d, 0x11d1, 0xb3, 0xf1, 0x00, 0xaa, 0x00, 0x37, 0x61, 0xc5)
OUR_GUID_ENTRY(CLSID_SimpleCutList,                  0xa5ea8d30, 0x253d, 0x11d1, 0xb3, 0xf1, 0x00, 0xaa, 0x00, 0x37, 0x61, 0xc5)
OUR_GUID_ENTRY(CLSID_VideoFileClip,                  0xa5ea8d31, 0x253d, 0x11d1, 0xb3, 0xf1, 0x00, 0xaa, 0x00, 0x37, 0x61, 0xc5)
OUR_GUID_ENTRY(CLSID_AudioFileClip,                  0xa5ea8d32, 0x253d, 0x11d1, 0xb3, 0xf1, 0x00, 0xaa, 0x00, 0x37, 0x61, 0xc5)
OUR_GUID_ENTRY(CLSID_CutListCacheMemory,             0xa5ea8d33, 0x253d, 0x11d1, 0xb3, 0xf1, 0x00, 0xaa, 0x00, 0x37, 0x61, 0xc5)
OUR_GUID_ENTRY(CLSID_CaptureGraphBuilder,            0xbf87b6e0, 0x8c27, 0x11d0, 0xb3, 0xf0, 0x00, 0xaa, 0x00, 0x37, 0x61, 0xc5)
OUR_GUID_ENTRY(CLSID_CaptureGraphBuilder2,           0xbf87b6e1, 0x8c27, 0x11d0, 0xb3, 0xf0, 0x00, 0xaa, 0x00, 0x37, 0x61, 0xc5)
OUR_GUID_ENTRY(CLSID_ProtoFilterGraph,               0xe436ebb0, 0x524f, 0x11ce, 0x9f, 0x53, 0x00, 0x20, 0xaf, 0x0b, 0xa7, 0x70)
OUR_GUID_ENTRY(CLSID_SystemClock,                    0xe436ebb1, 0x524f, 0x11ce, 0x9f, 0x53, 0x00, 0x20, 0xaf, 0x0b, 0xa7, 0x70)
OUR_GUID_ENTRY(CLSID_FilterMapper,                   0xe436ebb2, 0x524f, 0x11ce, 0x9f, 0x53, 0x00, 0x20, 0xaf, 0x0b, 0xa7, 0x70)
OUR_GUID_ENTRY(CLSID_FilterGraph,                    0xe436ebb3, 0x524f, 0x11ce, 0x9f, 0x53, 0x00, 0x20, 0xaf, 0x0b, 0xa7, 0x70)
OUR_GUID_ENTRY(CLSID_FilterGraphNoThread,            0xe436ebb8, 0x524f, 0x11ce, 0x9f, 0x53, 0x00, 0x20, 0xaf, 0x0b, 0xa7, 0x70)
OUR_GUID_ENTRY(CLSID_MPEG1Doc,                       0xe4bbd160, 0x4269, 0x11ce, 0x83, 0x8d, 0x00, 0xaa, 0x00, 0x55, 0x59, 0x5a)
OUR_GUID_ENTRY(CLSID_FileSource,                     0x701722e0, 0x8ae3, 0x11ce, 0xa8, 0x5c, 0x00, 0xaa, 0x00, 0x2f, 0xea, 0xb5)
OUR_GUID_ENTRY(CLSID_MPEG1PacketPlayer,              0x26c25940, 0x4ca9, 0x11ce, 0xa8, 0x28, 0x00, 0xaa, 0x00, 0x2f, 0xea, 0xb5)
OUR_GUID_ENTRY(CLSID_MPEG1Splitter,                  0x336475d0, 0x942a, 0x11ce, 0xa8, 0x70, 0x00, 0xaa, 0x00, 0x2f, 0xea, 0xb5)
OUR_GUID_ENTRY(CLSID_CMpegVideoCodec,                0xfeb50740, 0x7bef, 0x11ce, 0x9b, 0xd9, 0x00, 0x00, 0xe2, 0x02, 0x59, 0x9c)
OUR_GUID_ENTRY(CLSID_CMpegAudioCodec,                0x4a2286e0, 0x7bef, 0x11ce, 0x9b, 0xd9, 0x00, 0x00, 0xe2, 0x02, 0x59, 0x9c)
OUR_GUID_ENTRY(CLSID_TextRender,                     0xe30629d3, 0x27e5, 0x11ce, 0x87, 0x5d, 0x00, 0x60, 0x8c, 0xb7, 0x80, 0x66)
OUR_GUID_ENTRY(CLSID_InfTee,                         0xf8388a40, 0xd5bb, 0x11d0, 0xbe, 0x5a, 0x00, 0x80, 0xc7, 0x06, 0x56, 0x8e)
OUR_GUID_ENTRY(CLSID_AviSplitter,                    0x1b544c20, 0xfd0b, 0x11ce, 0x8c, 0x63, 0x00, 0xaa, 0x00, 0x44, 0xb5, 0x1e)
OUR_GUID_ENTRY(CLSID_AviReader,                      0x1b544c21, 0xfd0b, 0x11ce, 0x8c, 0x63, 0x00, 0xaa, 0x00, 0x44, 0xb5, 0x1e)
OUR_GUID_ENTRY(CLSID_VfwCapture,                     0x1b544c22, 0xfd0b, 0x11ce, 0x8c, 0x63, 0x00, 0xaa, 0x00, 0x44, 0xb5, 0x1e)
OUR_GUID_ENTRY(CLSID_CaptureProperties,              0x1b544c22, 0xfd0b, 0x11ce, 0x8c, 0x63, 0x00, 0xaa, 0x00, 0x44, 0xb5, 0x1f)
OUR_GUID_ENTRY(CLSID_FGControl,                      0xe436ebb4, 0x524f, 0x11ce, 0x9f, 0x53, 0x00, 0x20, 0xaf, 0x0b, 0xa7, 0x70)
OUR_GUID_ENTRY(CLSID_MOVReader,                      0x44584800, 0xf8ee, 0x11ce, 0xb2, 0xd4, 0x00, 0xdd, 0x01, 0x10, 0x1b, 0x85)
OUR_GUID_ENTRY(CLSID_QuickTimeParser,                0xd51bd5a0, 0x7548, 0x11cf, 0xa5, 0x20, 0x00, 0x80, 0xc7, 0x7e, 0xf5, 0x8a)
OUR_GUID_ENTRY(CLSID_QTDec,                          0xfdfe9681, 0x74a3, 0x11d0, 0xaf, 0xa7, 0x00, 0xaa, 0x00, 0xb6, 0x7a, 0x42)
OUR_GUID_ENTRY(CLSID_AVIDoc,                         0xd3588ab0, 0x0781, 0x11ce, 0xb0, 0x3a, 0x00, 0x20, 0xaf, 0x0b, 0xa7, 0x70)
OUR_GUID_ENTRY(CLSID_AVIDocWriter,                   0xd3588ab1, 0x0781, 0x11ce, 0xb0, 0x3a, 0x00, 0x20, 0xaf, 0x0b, 0xa7, 0x70)
OUR_GUID_ENTRY(CLSID_VideoRenderer,                  0x70e102b0, 0x5556, 0x11ce, 0x97, 0xc0, 0x00, 0xaa, 0x00, 0x55, 0x59, 0x5a)
OUR_GUID_ENTRY(CLSID_Colour,                         0x1643e180, 0x90f5, 0x11ce, 0x97, 0xd5, 0x00, 0xaa, 0x00, 0x55, 0x59, 0x5a)
OUR_GUID_ENTRY(CLSID_Dither,                         0x1da08500, 0x9edc, 0x11cf, 0xbc, 0x10, 0x00, 0xaa, 0x00, 0xac, 0x74, 0xf6)
OUR_GUID_ENTRY(CLSID_ModexRenderer,                  0x07167665, 0x5011, 0x11cf, 0xbf, 0x33, 0x00, 0xaa, 0x00, 0x55, 0x59, 0x5a)
OUR_GUID_ENTRY(CLSID_AudioRender,                    0xe30629d1, 0x27e5, 0x11ce, 0x87, 0x5d, 0x00, 0x60, 0x8c, 0xb7, 0x80, 0x66)
OUR_GUID_ENTRY(CLSID_AudioProperties,                0x05589faf, 0xc356, 0x11ce, 0xbf, 0x01, 0x00, 0xaa, 0x00, 0x55, 0x59, 0x5a)
OUR_GUID_ENTRY(CLSID_DSoundRender,                   0x79376820, 0x07d0, 0x11cf, 0xa2, 0x4d, 0x00, 0x20, 0xaf, 0xd7, 0x97, 0x67)
OUR_GUID_ENTRY(CLSID_AudioRecord,                    0xe30629d2, 0x27e5, 0x11ce, 0x87, 0x5d, 0x00, 0x60, 0x8c, 0xb7, 0x80, 0x66)
OUR_GUID_ENTRY(CLSID_AudioInputMixerProperties,      0x2ca8ca52, 0x3c3f, 0x11d2, 0xb7, 0x3d, 0x00, 0xc0, 0x4f, 0xb6, 0xbd, 0x3d)
OUR_GUID_ENTRY(CLSID_AVIDec,                         0xcf49d4e0, 0x1115, 0x11ce, 0xb0, 0x3a, 0x00, 0x20, 0xaf, 0x0b, 0xa7, 0x70)
OUR_GUID_ENTRY(CLSID_AVIDraw,                        0xa888df60, 0x1e90, 0x11cf, 0xac, 0x98, 0x00, 0xaa, 0x00, 0x4c, 0x0f, 0xa9)
OUR_GUID_ENTRY(CLSID_ACMWrapper,                     0x6a08cf80, 0x0e18, 0x11cf, 0xa2, 0x4d, 0x00, 0x20, 0xaf, 0xd7, 0x97, 0x67)
OUR_GUID_ENTRY(CLSID_AsyncReader,                    0xe436ebb5, 0x524f, 0x11ce, 0x9f, 0x53, 0x00, 0x20, 0xaf, 0x0b, 0xa7, 0x70)
OUR_GUID_ENTRY(CLSID_URLReader,                      0xe436ebb6, 0x524f, 0x11ce, 0x9f, 0x53, 0x00, 0x20, 0xaf, 0x0b, 0xa7, 0x70)
OUR_GUID_ENTRY(CLSID_PersistMonikerPID,              0xe436ebb7, 0x524f, 0x11ce, 0x9f, 0x53, 0x00, 0x20, 0xaf, 0x0b, 0xa7, 0x70)
OUR_GUID_ENTRY(CLSID_AMovie,                         0x5f2759c0, 0x7685, 0x11cf, 0x8b, 0x23, 0x00, 0x80, 0x5f, 0x6c, 0xef, 0x60)
OUR_GUID_ENTRY(CLSID_AVICo,                          0xd76e2820, 0x1563, 0x11cf, 0xac, 0x98, 0x00, 0xaa, 0x00, 0x4c, 0x0f, 0xa9)
OUR_GUID_ENTRY(CLSID_FileWriter,                     0x8596e5f0, 0x0da5, 0x11d0, 0xbd, 0x21, 0x00, 0xa0, 0xc9, 0x11, 0xce, 0x86)
OUR_GUID_ENTRY(CLSID_AviDest,                        0xe2510970, 0xf137, 0x11ce, 0x8b, 0x67, 0x00, 0xaa, 0x00, 0xa3, 0xf1, 0xa6)
OUR_GUID_ENTRY(CLSID_AsfMux,                         0xf560ae42, 0x6cdd, 0x11d1, 0xad, 0xe2, 0x00, 0x00, 0xf8, 0x75, 0x4b, 0x99)
OUR_GUID_ENTRY(CLSID_AviMuxProptyPage,               0xc647b5c0, 0x157c, 0x11d0, 0xbd, 0x23, 0x00, 0xa0, 0xc9, 0x11, 0xce, 0x86)
OUR_GUID_ENTRY(CLSID_AviMuxProptyPage1,              0x0a9ae910, 0x85c0, 0x11d0, 0xbd, 0x42, 0x00, 0xa0, 0xc9, 0x11, 0xce, 0x86)
OUR_GUID_ENTRY(CLSID_AVIMIDIRender,                  0x07b65360, 0xc445, 0x11ce, 0xaf, 0xde, 0x00, 0xaa, 0x00, 0x6c, 0x14, 0xf4)
OUR_GUID_ENTRY(CLSID_DVVideoCodec,                   0xb1b77c00, 0xc3e4, 0x11cf, 0xaf, 0x79, 0x00, 0xaa, 0x00, 0xb6, 0x7a, 0x42)
OUR_GUID_ENTRY(CLSID_DVVideoEnc,                     0x13aa3650, 0xbb6f, 0x11d0, 0xaf, 0xb9, 0x00, 0xaa, 0x00, 0xb6, 0x7a, 0x42)
OUR_GUID_ENTRY(CLSID_DVSplitter,                     0x4eb31670, 0x9fc6, 0x11cf, 0xaf, 0x6e, 0x00, 0xaa, 0x00, 0xb6, 0x7a, 0x42)
OUR_GUID_ENTRY(CLSID_DVMux,                          0x129d7e40, 0xc10d, 0x11d0, 0xaf, 0xb9, 0x00, 0xaa, 0x00, 0xb6, 0x7a, 0x42)
OUR_GUID_ENTRY(CLSID_SeekingPassThru,                0x060af76c, 0x68dd, 0x11d0, 0x8f, 0xc1, 0x00, 0xc0, 0x4f, 0xd9, 0x18, 0x9d)
OUR_GUID_ENTRY(CLSID_Line21Decoder,                  0x6e8d4a20, 0x310c, 0x11d0, 0xb7, 0x9a, 0x00, 0xaa, 0x00, 0x37, 0x67, 0xa7)
OUR_GUID_ENTRY(CLSID_OverlayMixer,                   0xcd8743a1, 0x3736, 0x11d0, 0x9e, 0x69, 0x00, 0xc0, 0x4f, 0xd7, 0xc1, 0x5b)
OUR_GUID_ENTRY(CLSID_VBISurfaces,                    0x814b9800, 0x1c88, 0x11d1, 0xba, 0xd9, 0x00, 0x60, 0x97, 0x44, 0x11, 0x1a)
OUR_GUID_ENTRY(CLSID_SystemDeviceEnum,               0x62be5d10, 0x60eb, 0x11d0, 0xbd, 0x3b, 0x00, 0xa0, 0xc9, 0x11, 0xce, 0x86)
OUR_GUID_ENTRY(CLSID_CDeviceMoniker,                 0x4315d437, 0x5b8c, 0x11d0, 0xbd, 0x3b, 0x00, 0xa0, 0xc9, 0x11, 0xce, 0x86)
OUR_GUID_ENTRY(CLSID_VideoInputDeviceCategory,       0x860bb310, 0x5d01, 0x11d0, 0xbd, 0x3b, 0x00, 0xa0, 0xc9, 0x11, 0xce, 0x86)
OUR_GUID_ENTRY(CLSID_CVidCapClassManager,            0x860bb310, 0x5d01, 0x11d0, 0xbd, 0x3b, 0x00, 0xa0, 0xc9, 0x11, 0xce, 0x86)
OUR_GUID_ENTRY(CLSID_LegacyAmFilterCategory,         0x083863f1, 0x70de, 0x11d0, 0xbd, 0x40, 0x00, 0xa0, 0xc9, 0x11, 0xce, 0x86)
OUR_GUID_ENTRY(CLSID_CQzFilterClassManager,          0x083863f1, 0x70de, 0x11d0, 0xbd, 0x40, 0x00, 0xa0, 0xc9, 0x11, 0xce, 0x86)
OUR_GUID_ENTRY(CLSID_VideoCompressorCategory,        0x33d9a760, 0x90c8, 0x11d0, 0xbd, 0x43, 0x00, 0xa0, 0xc9, 0x11, 0xce, 0x86)
OUR_GUID_ENTRY(CLSID_CIcmCoClassManager,             0x33d9a760, 0x90c8, 0x11d0, 0xbd, 0x43, 0x00, 0xa0, 0xc9, 0x11, 0xce, 0x86)
OUR_GUID_ENTRY(CLSID_AudioCompressorCategory,        0x33d9a761, 0x90c8, 0x11d0, 0xbd, 0x43, 0x00, 0xa0, 0xc9, 0x11, 0xce, 0x86)
OUR_GUID_ENTRY(CLSID_CAcmCoClassManager,             0x33d9a761, 0x90c8, 0x11d0, 0xbd, 0x43, 0x00, 0xa0, 0xc9, 0x11, 0xce, 0x86)
OUR_GUID_ENTRY(CLSID_AudioInputDeviceCategory,       0x33d9a762, 0x90c8, 0x11d0, 0xbd, 0x43, 0x00, 0xa0, 0xc9, 0x11, 0xce, 0x86)
OUR_GUID_ENTRY(CLSID_CWaveinClassManager,            0x33d9a762, 0x90c8, 0x11d0, 0xbd, 0x43, 0x00, 0xa0, 0xc9, 0x11, 0xce, 0x86)
OUR_GUID_ENTRY(CLSID_AudioRendererCategory,          0xe0f158e1, 0xcb04, 0x11d0, 0xbd, 0x4e, 0x00, 0xa0, 0xc9, 0x11, 0xce, 0x86)
OUR_GUID_ENTRY(CLSID_CWaveOutClassManager,           0xe0f158e1, 0xcb04, 0x11d0, 0xbd, 0x4e, 0x00, 0xa0, 0xc9, 0x11, 0xce, 0x86)
OUR_GUID_ENTRY(CLSID_MidiRendererCategory,           0x4efe2452, 0x168a, 0x11d1, 0xbc, 0x76, 0x00, 0xc0, 0x4f, 0xb9, 0x45, 0x3b)
OUR_GUID_ENTRY(CLSID_CMidiOutClassManager,           0x4efe2452, 0x168a, 0x11d1, 0xbc, 0x76, 0x00, 0xc0, 0x4f, 0xb9, 0x45, 0x3b)
OUR_GUID_ENTRY(CLSID_TransmitCategory,               0xcc7bfb41, 0xf175, 0x11d1, 0xa3, 0x92, 0x00, 0xe0, 0x29, 0x1f, 0x39, 0x59)
OUR_GUID_ENTRY(CLSID_DeviceControlCategory,          0xcc7bfb46, 0xf175, 0x11d1, 0xa3, 0x92, 0x00, 0xe0, 0x29, 0x1f, 0x39, 0x59)
OUR_GUID_ENTRY(CLSID_ActiveMovieCategories,          0xda4e3da0, 0xd07d, 0x11d0, 0xbd, 0x50, 0x00, 0xa0, 0xc9, 0x11, 0xce, 0x86)
OUR_GUID_ENTRY(CLSID_DVDHWDecodersCategory,          0x2721ae20, 0x7e70, 0x11d0, 0xa5, 0xd6, 0x28, 0xdb, 0x04, 0xc1, 0x00, 0x00)
OUR_GUID_ENTRY(CLSID_FilterMapper2,                  0xcda42200, 0xbd88, 0x11d0, 0xbd, 0x4e, 0x00, 0xa0, 0xc9, 0x11, 0xce, 0x86)
OUR_GUID_ENTRY(CLSID_MemoryAllocator,                0x1e651cc0, 0xb199, 0x11d0, 0x82, 0x12, 0x00, 0xc0, 0x4f, 0xc3, 0x2c, 0x45)
OUR_GUID_ENTRY(CLSID_MediaPropertyBag,               0xcdbd8d00, 0xc193, 0x11d0, 0xbd, 0x4e, 0x00, 0xa0, 0xc9, 0x11, 0xce, 0x86)
OUR_GUID_ENTRY(CLSID_DvdGraphBuilder,                0xfcc152b7, 0xf372, 0x11d0, 0x8e, 0x00, 0x00, 0xc0, 0x4f, 0xd7, 0xc0, 0x8b)
OUR_GUID_ENTRY(CLSID_DVDNavigator,                   0x9b8c4620, 0x2c1a, 0x11d0, 0x84, 0x93, 0x00, 0xa0, 0x24, 0x38, 0xad, 0x48)
OUR_GUID_ENTRY(CLSID_SmartTee,                       0xcc58e280, 0x8aa1, 0x11d1, 0xb3, 0xf1, 0x00, 0xaa, 0x00, 0x37, 0x61, 0xc5)
OUR_GUID_ENTRY(FORMAT_None,                          0x0f6417d6, 0xc318, 0x11d0, 0xa4, 0x3f, 0x00, 0xa0, 0xc9, 0x22, 0x31, 0x96)
OUR_GUID_ENTRY(FORMAT_VideoInfo,                     0x05589f80, 0xc356, 0x11ce, 0xbf, 0x01, 0x00, 0xaa, 0x00, 0x55, 0x59, 0x5a)
OUR_GUID_ENTRY(FORMAT_VideoInfo2,                    0xf72a76a0, 0xeb0a, 0x11d0, 0xac, 0xe4, 0x00, 0x00, 0xc0, 0xcc, 0x16, 0xba)
OUR_GUID_ENTRY(FORMAT_WaveFormatEx,                  0x05589f81, 0xc356, 0x11ce, 0xbf, 0x01, 0x00, 0xaa, 0x00, 0x55, 0x59, 0x5a)
OUR_GUID_ENTRY(FORMAT_MPEGVideo,                     0x05589f82, 0xc356, 0x11ce, 0xbf, 0x01, 0x00, 0xaa, 0x00, 0x55, 0x59, 0x5a)
OUR_GUID_ENTRY(FORMAT_MPEGStreams,                   0x05589f83, 0xc356, 0x11ce, 0xbf, 0x01, 0x00, 0xaa, 0x00, 0x55, 0x59, 0x5a)
OUR_GUID_ENTRY(FORMAT_DvInfo,                        0x05589f84, 0xc356, 0x11ce, 0xbf, 0x01, 0x00, 0xaa, 0x00, 0x55, 0x59, 0x5a)
OUR_GUID_ENTRY(CLSID_DirectDrawProperties,           0x944d4c00, 0xdd52, 0x11ce, 0xbf, 0x0e, 0x00, 0xaa, 0x00, 0x55, 0x59, 0x5a)
OUR_GUID_ENTRY(CLSID_PerformanceProperties,          0x59ce6880, 0xacf8, 0x11cf, 0xb5, 0x6e, 0x00, 0x80, 0xc7, 0xc4, 0xb6, 0x8a)
OUR_GUID_ENTRY(CLSID_QualityProperties,              0x418afb70, 0xf8b8, 0x11ce, 0xaa, 0xc6, 0x00, 0x20, 0xaf, 0x0b, 0x99, 0xa3)
OUR_GUID_ENTRY(IID_IBaseVideoMixer,                  0x61ded640, 0xe912, 0x11ce, 0xa0, 0x99, 0x00, 0xaa, 0x00, 0x47, 0x9a, 0x58)
OUR_GUID_ENTRY(IID_IDirectDrawVideo,                 0x36d39eb0, 0xdd75, 0x11ce, 0xbf, 0x0e, 0x00, 0xaa, 0x00, 0x55, 0x59, 0x5a)
OUR_GUID_ENTRY(IID_IQualProp,                        0x1bd0ecb0, 0xf8e2, 0x11ce, 0xaa, 0xc6, 0x00, 0x20, 0xaf, 0x0b, 0x99, 0xa3)
OUR_GUID_ENTRY(CLSID_VPObject,                       0xce292861, 0xfc88, 0x11d0, 0x9e, 0x69, 0x00, 0xc0, 0x4f, 0xd7, 0xc1, 0x5b)
OUR_GUID_ENTRY(IID_IVPObject,                        0xce292862, 0xfc88, 0x11d0, 0x9e, 0x69, 0x00, 0xc0, 0x4f, 0xd7, 0xc1, 0x5b)
OUR_GUID_ENTRY(IID_IVPControl,                       0x25df12c1, 0x3de0, 0x11d1, 0x9e, 0x69, 0x00, 0xc0, 0x4f, 0xd7, 0xc1, 0x5b)
OUR_GUID_ENTRY(CLSID_VPVBIObject,                    0x814b9801, 0x1c88, 0x11d1, 0xba, 0xd9, 0x00, 0x60, 0x97, 0x44, 0x11, 0x1a)
OUR_GUID_ENTRY(IID_IVPVBIObject,                     0x814b9802, 0x1c88, 0x11d1, 0xba, 0xd9, 0x00, 0x60, 0x97, 0x44, 0x11, 0x1a)
OUR_GUID_ENTRY(IID_IVPConfig,                        0xbc29a660, 0x30e3, 0x11d0, 0x9e, 0x69, 0x00, 0xc0, 0x4f, 0xd7, 0xc1, 0x5b)
OUR_GUID_ENTRY(IID_IVPNotify,                        0xc76794a1, 0xd6c5, 0x11d0, 0x9e, 0x69, 0x00, 0xc0, 0x4f, 0xd7, 0xc1, 0x5b)
OUR_GUID_ENTRY(IID_IVPNotify2,                       0xebf47183, 0x8764, 0x11d1, 0x9e, 0x69, 0x00, 0xc0, 0x4f, 0xd7, 0xc1, 0x5b)
OUR_GUID_ENTRY(IID_IVPVBIConfig,                     0xec529b00, 0x1a1f, 0x11d1, 0xba, 0xd9, 0x00, 0x60, 0x97, 0x44, 0x11, 0x1a)
OUR_GUID_ENTRY(IID_IVPVBINotify,                     0xec529b01, 0x1a1f, 0x11d1, 0xba, 0xd9, 0x00, 0x60, 0x97, 0x44, 0x11, 0x1a)
OUR_GUID_ENTRY(IID_IMixerPinConfig,                  0x593cdde1, 0x0759, 0x11d1, 0x9e, 0x69, 0x00, 0xc0, 0x4f, 0xd7, 0xc1, 0x5b)
OUR_GUID_ENTRY(IID_IMixerPinConfig2,                 0xebf47182, 0x8764, 0x11d1, 0x9e, 0x69, 0x00, 0xc0, 0x4f, 0xd7, 0xc1, 0x5b)
OUR_GUID_ENTRY(CLSID_ModexProperties,                0x0618aa30, 0x6bc4, 0x11cf, 0xbf, 0x36, 0x00, 0xaa, 0x00, 0x55, 0x59, 0x5a)
OUR_GUID_ENTRY(IID_IFullScreenVideo,                 0xdd1d7110, 0x7836, 0x11cf, 0xbf, 0x47, 0x00, 0xaa, 0x00, 0x55, 0x59, 0x5a)
OUR_GUID_ENTRY(IID_IFullScreenVideoEx,               0x53479470, 0xf1dd, 0x11cf, 0xbc, 0x42, 0x00, 0xaa, 0x00, 0xac, 0x74, 0xf6)
OUR_GUID_ENTRY(CLSID_DVDecPropertiesPage,            0x101193c0, 0x0bfe, 0x11d0, 0xaf, 0x91, 0x00, 0xaa, 0x00, 0xb6, 0x7a, 0x42)
OUR_GUID_ENTRY(CLSID_DVEncPropertiesPage,            0x4150f050, 0xbb6f, 0x11d0, 0xaf, 0xb9, 0x00, 0xaa, 0x00, 0xb6, 0x7a, 0x42)
OUR_GUID_ENTRY(CLSID_DVMuxPropertyPage,              0x4db880e0, 0xc10d, 0x11d0, 0xaf, 0xb9, 0x00, 0xaa, 0x00, 0xb6, 0x7a, 0x42)
OUR_GUID_ENTRY(IID_IAMDirectSound,                   0x546f4260, 0xd53e, 0x11cf, 0xb3, 0xf0, 0x00, 0xaa, 0x00, 0x37, 0x61, 0xc5)
OUR_GUID_ENTRY(IID_IMpegAudioDecoder,                0xb45dd570, 0x3c77, 0x11d1, 0xab, 0xe1, 0x00, 0xa0, 0xc9, 0x05, 0xf3, 0x75)
OUR_GUID_ENTRY(IID_IAMLine21Decoder,                 0x6e8d4a21, 0x310c, 0x11d0, 0xb7, 0x9a, 0x00, 0xaa, 0x00, 0x37, 0x67, 0xa7)
OUR_GUID_ENTRY(FORMAT_AnalogVideo,                   0x0482dde0, 0x7817, 0x11cf, 0x8a, 0x03, 0x00, 0xaa, 0x00, 0x6e, 0xcb, 0x65)
OUR_GUID_ENTRY(MEDIATYPE_AnalogVideo,                0x0482dde1, 0x7817, 0x11cf, 0x8a, 0x03, 0x00, 0xaa, 0x00, 0x6e, 0xcb, 0x65)
OUR_GUID_ENTRY(MEDIASUBTYPE_AnalogVideo_NTSC_M,      0x0482dde2, 0x7817, 0x11cf, 0x8a, 0x03, 0x00, 0xaa, 0x00, 0x6e, 0xcb, 0x65)
OUR_GUID_ENTRY(MEDIASUBTYPE_AnalogVideo_PAL_B,       0x0482dde5, 0x7817, 0x11cf, 0x8a, 0x03, 0x00, 0xaa, 0x00, 0x6e, 0xcb, 0x65)
OUR_GUID_ENTRY(MEDIASUBTYPE_AnalogVideo_PAL_D,       0x0482dde6, 0x7817, 0x11cf, 0x8a, 0x03, 0x00, 0xaa, 0x00, 0x6e, 0xcb, 0x65)
OUR_GUID_ENTRY(MEDIASUBTYPE_AnalogVideo_PAL_G,       0x0482dde7, 0x7817, 0x11cf, 0x8a, 0x03, 0x00, 0xaa, 0x00, 0x6e, 0xcb, 0x65)
OUR_GUID_ENTRY(MEDIASUBTYPE_AnalogVideo_PAL_H,       0x0482dde8, 0x7817, 0x11cf, 0x8a, 0x03, 0x00, 0xaa, 0x00, 0x6e, 0xcb, 0x65)
OUR_GUID_ENTRY(MEDIASUBTYPE_AnalogVideo_PAL_I,       0x0482dde9, 0x7817, 0x11cf, 0x8a, 0x03, 0x00, 0xaa, 0x00, 0x6e, 0xcb, 0x65)
OUR_GUID_ENTRY(MEDIASUBTYPE_AnalogVideo_PAL_M,       0x0482ddea, 0x7817, 0x11cf, 0x8a, 0x03, 0x00, 0xaa, 0x00, 0x6e, 0xcb, 0x65)
OUR_GUID_ENTRY(MEDIASUBTYPE_AnalogVideo_PAL_N,       0x0482ddeb, 0x7817, 0x11cf, 0x8a, 0x03, 0x00, 0xaa, 0x00, 0x6e, 0xcb, 0x65)
OUR_GUID_ENTRY(MEDIASUBTYPE_AnalogVideo_SECAM_B,     0x0482ddf0, 0x7817, 0x11cf, 0x8a, 0x03, 0x00, 0xaa, 0x00, 0x6e, 0xcb, 0x65)
OUR_GUID_ENTRY(MEDIASUBTYPE_AnalogVideo_SECAM_D,     0x0482ddf1, 0x7817, 0x11cf, 0x8a, 0x03, 0x00, 0xaa, 0x00, 0x6e, 0xcb, 0x65)
OUR_GUID_ENTRY(MEDIASUBTYPE_AnalogVideo_SECAM_G,     0x0482ddf2, 0x7817, 0x11cf, 0x8a, 0x03, 0x00, 0xaa, 0x00, 0x6e, 0xcb, 0x65)
OUR_GUID_ENTRY(MEDIASUBTYPE_AnalogVideo_SECAM_H,     0x0482ddf3, 0x7817, 0x11cf, 0x8a, 0x03, 0x00, 0xaa, 0x00, 0x6e, 0xcb, 0x65)
OUR_GUID_ENTRY(MEDIASUBTYPE_AnalogVideo_SECAM_K,     0x0482ddf4, 0x7817, 0x11cf, 0x8a, 0x03, 0x00, 0xaa, 0x00, 0x6e, 0xcb, 0x65)
OUR_GUID_ENTRY(MEDIASUBTYPE_AnalogVideo_SECAM_K1,    0x0482ddf5, 0x7817, 0x11cf, 0x8a, 0x03, 0x00, 0xaa, 0x00, 0x6e, 0xcb, 0x65)
OUR_GUID_ENTRY(MEDIASUBTYPE_AnalogVideo_SECAM_L,     0x0482ddf6, 0x7817, 0x11cf, 0x8a, 0x03, 0x00, 0xaa, 0x00, 0x6e, 0xcb, 0x65)
OUR_GUID_ENTRY(MEDIATYPE_AnalogAudio,                0x0482dee1, 0x7817, 0x11cf, 0x8a, 0x03, 0x00, 0xaa, 0x00, 0x6e, 0xcb, 0x65)
OUR_GUID_ENTRY(TIME_FORMAT_NONE,                     0x00000000, 0x0000, 0x0000, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00)
OUR_GUID_ENTRY(TIME_FORMAT_FRAME,                    0x7b785570, 0x8c82, 0x11cf, 0xbc, 0x0c, 0x00, 0xaa, 0x00, 0xac, 0x74, 0xf6)
OUR_GUID_ENTRY(TIME_FORMAT_BYTE,                     0x7b785571, 0x8c82, 0x11cf, 0xbc, 0x0c, 0x00, 0xaa, 0x00, 0xac, 0x74, 0xf6)
OUR_GUID_ENTRY(TIME_FORMAT_SAMPLE,                   0x7b785572, 0x8c82, 0x11cf, 0xbc, 0x0c, 0x00, 0xaa, 0x00, 0xac, 0x74, 0xf6)
OUR_GUID_ENTRY(TIME_FORMAT_FIELD,                    0x7b785573, 0x8c82, 0x11cf, 0xbc, 0x0c, 0x00, 0xaa, 0x00, 0xac, 0x74, 0xf6)
OUR_GUID_ENTRY(TIME_FORMAT_MEDIA_TIME,               0x7b785574, 0x8c82, 0x11cf, 0xbc, 0x0c, 0x00, 0xaa, 0x00, 0xac, 0x74, 0xf6)
OUR_GUID_ENTRY(AMPROPSETID_Pin,                      0x9b00f101, 0x1567, 0x11d1, 0xb3, 0xf1, 0x00, 0xaa, 0x00, 0x37, 0x61, 0xc5)
OUR_GUID_ENTRY(PIN_CATEGORY_CAPTURE,                 0xfb6c4281, 0x0353, 0x11d1, 0x90, 0x5f, 0x00, 0x00, 0xc0, 0xcc, 0x16, 0xba)
OUR_GUID_ENTRY(PIN_CATEGORY_PREVIEW,                 0xfb6c4282, 0x0353, 0x11d1, 0x90, 0x5f, 0x00, 0x00, 0xc0, 0xcc, 0x16, 0xba)
OUR_GUID_ENTRY(PIN_CATEGORY_ANALOGVIDEOIN,           0xfb6c4283, 0x0353, 0x11d1, 0x90, 0x5f, 0x00, 0x00, 0xc0, 0xcc, 0x16, 0xba)
OUR_GUID_ENTRY(PIN_CATEGORY_VBI,                     0xfb6c4284, 0x0353, 0x11d1, 0x90, 0x5f, 0x00, 0x00, 0xc0, 0xcc, 0x16, 0xba)
OUR_GUID_ENTRY(PIN_CATEGORY_VIDEOPORT,               0xfb6c4285, 0x0353, 0x11d1, 0x90, 0x5f, 0x00, 0x00, 0xc0, 0xcc, 0x16, 0xba)
OUR_GUID_ENTRY(PIN_CATEGORY_NABTS,                   0xfb6c4286, 0x0353, 0x11d1, 0x90, 0x5f, 0x00, 0x00, 0xc0, 0xcc, 0x16, 0xba)
OUR_GUID_ENTRY(PIN_CATEGORY_EDS,                     0xfb6c4287, 0x0353, 0x11d1, 0x90, 0x5f, 0x00, 0x00, 0xc0, 0xcc, 0x16, 0xba)
OUR_GUID_ENTRY(PIN_CATEGORY_TELETEXT,                0xfb6c4288, 0x0353, 0x11d1, 0x90, 0x5f, 0x00, 0x00, 0xc0, 0xcc, 0x16, 0xba)
OUR_GUID_ENTRY(PIN_CATEGORY_CC,                      0xfb6c4289, 0x0353, 0x11d1, 0x90, 0x5f, 0x00, 0x00, 0xc0, 0xcc, 0x16, 0xba)
OUR_GUID_ENTRY(PIN_CATEGORY_STILL,                   0xfb6c428a, 0x0353, 0x11d1, 0x90, 0x5f, 0x00, 0x00, 0xc0, 0xcc, 0x16, 0xba)
OUR_GUID_ENTRY(PIN_CATEGORY_TIMECODE,                0xfb6c428b, 0x0353, 0x11d1, 0x90, 0x5f, 0x00, 0x00, 0xc0, 0xcc, 0x16, 0xba)
OUR_GUID_ENTRY(PIN_CATEGORY_VIDEOPORT_VBI,           0xfb6c428c, 0x0353, 0x11d1, 0x90, 0x5f, 0x00, 0x00, 0xc0, 0xcc, 0x16, 0xba)
OUR_GUID_ENTRY(LOOK_UPSTREAM_ONLY,                   0xac798be0, 0x98e3, 0x11d1, 0xb3, 0xf1, 0x00, 0xaa, 0x00, 0x37, 0x61, 0xc5)
OUR_GUID_ENTRY(LOOK_DOWNSTREAM_ONLY,                 0xac798be1, 0x98e3, 0x11d1, 0xb3, 0xf1, 0x00, 0xaa, 0x00, 0x37, 0x61, 0xc5)
OUR_GUID_ENTRY(CLSID_TVTunerFilterPropertyPage,      0x266eee41, 0x6c63, 0x11cf, 0x8a, 0x03, 0x00, 0xaa, 0x00, 0x6e, 0xcb, 0x65)
OUR_GUID_ENTRY(CLSID_CrossbarFilterPropertyPage,     0x71f96461, 0x78f3, 0x11d0, 0xa1, 0x8c, 0x00, 0xa0, 0xc9, 0x11, 0x89, 0x56)
OUR_GUID_ENTRY(CLSID_TVAudioFilterPropertyPage,      0x71f96463, 0x78f3, 0x11d0, 0xa1, 0x8c, 0x00, 0xa0, 0xc9, 0x11, 0x89, 0x56)
OUR_GUID_ENTRY(CLSID_VideoProcAmpPropertyPage,       0x71f96464, 0x78f3, 0x11d0, 0xa1, 0x8c, 0x00, 0xa0, 0xc9, 0x11, 0x89, 0x56)
OUR_GUID_ENTRY(CLSID_CameraControlPropertyPage,      0x71f96465, 0x78f3, 0x11d0, 0xa1, 0x8c, 0x00, 0xa0, 0xc9, 0x11, 0x89, 0x56)
OUR_GUID_ENTRY(CLSID_AnalogVideoDecoderPropertyPage, 0x71f96466, 0x78f3, 0x11d0, 0xa1, 0x8c, 0x00, 0xa0, 0xc9, 0x11, 0x89, 0x56)
OUR_GUID_ENTRY(CLSID_VideoStreamConfigPropertyPage,  0x71f96467, 0x78f3, 0x11d0, 0xa1, 0x8c, 0x00, 0xa0, 0xc9, 0x11, 0x89, 0x56)
OUR_GUID_ENTRY(CLSID_HTMLDocument,                   0x25336920, 0x03f9, 0x11cf, 0x8f, 0xd0, 0x00, 0xaa, 0x00, 0x68, 0x6f, 0x13)

#undef OUR_GUID_ENTRY

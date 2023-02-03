/******************************************************************************
*
* Copyright (C) 2015-2023 Allegro DVT2
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
******************************************************************************/

#include "HEVC_Sections.h"
#include "lib_common/Nuts.h"
#include "lib_bitstream/HEVC_RbspEncod.h"

static AL_TNalHeader GetNalHeaderHevc(uint8_t uNUT, uint8_t uNalRefIdc, uint8_t uLayerId, uint8_t uTempId)
{
  (void)uNalRefIdc;
  AL_TNalHeader nh;
  nh.size = 2;
  nh.bytes[0] = ((uLayerId & 0x20) >> 5) | ((uNUT & 0x3F) << 1);
  nh.bytes[1] = (uTempId + 1) | ((uLayerId & 0x1F) << 3);
  return nh;
}

AL_TNuts CreateHevcNuts(void)
{
  AL_TNuts nuts =
  {
    &GetNalHeaderHevc,
    AL_HEVC_NUT_SPS,
    AL_HEVC_NUT_PPS,
    AL_HEVC_NUT_VPS,
    AL_HEVC_NUT_AUD,
    AL_HEVC_NUT_FD,
    AL_HEVC_NUT_PREFIX_SEI,
    AL_HEVC_NUT_SUFFIX_SEI,
    0,
    0,
  };
  return nuts;
}

void HEVC_GenerateSections(AL_TEncCtx* pCtx, AL_TBuffer* pStream, AL_TEncPicStatus const* pPicStatus, int iLayerID, int iPicID, bool bMustWritePPS, bool bMustWriteAUD)
{
  AL_TNuts nuts = CreateHevcNuts();
  AL_TNalsData nalsData = AL_ExtractNalsData(pCtx, iLayerID, iPicID);
  nalsData.bMustWritePPS = bMustWritePPS;
  nalsData.bMustWriteAud = bMustWriteAUD;
  AL_TEncChanParam const* pChannel = &pCtx->pSettings->tChParam[0];
  GenerateSections(AL_GetHevcRbspWriter(), nuts, &nalsData, pStream, pPicStatus, iLayerID, pChannel->uNumSlices, pChannel->bSubframeLatency, false);
}


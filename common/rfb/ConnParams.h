/* Copyright (C) 2002-2005 RealVNC Ltd.  All Rights Reserved.
 * Copyright 2014 Pierre Ossman for Cendio AB
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307,
 * USA.
 */
//
// ConnParams - structure containing the connection parameters.
//

#ifndef __RFB_CONNPARAMS_H__
#define __RFB_CONNPARAMS_H__

#include <set>

#include <rdr/types.h>
#include <rfb/Cursor.h>
#include <rfb/PixelFormat.h>
#include <rfb/ScreenSet.h>

namespace rdr { class InStream; }

namespace rfb {

  const int subsampleUndefined = -1;
  const int subsampleNone = 0;
  const int subsampleGray = 1;
  const int subsample2X = 2;
  const int subsample4X = 3;
  const int subsample8X = 4;
  const int subsample16X = 5;

  class SMsgHandler;

  class ConnParams {
  public:
    ConnParams();
    ~ConnParams();

    bool readVersion(rdr::InStream* is, bool* done);
    void writeVersion(rdr::OutStream* os);

    int majorVersion;
    int minorVersion;

    void setVersion(int major, int minor) {
      majorVersion = major; minorVersion = minor;
    }
    bool isVersion(int major, int minor) const {
      return majorVersion == major && minorVersion == minor;
    }
    bool beforeVersion(int major, int minor) const {
      return (majorVersion < major ||
              (majorVersion == major && minorVersion < minor));
    }
    bool afterVersion(int major, int minor) const {
      return !beforeVersion(major,minor+1);
    }

    int width;
    int height;
    ScreenSet screenLayout;

    const PixelFormat& pf() const { return pf_; }
    void setPF(const PixelFormat& pf);

    void setSHandler(SMsgHandler *s) { shandler = s; }

    const char* name() const { return name_; }
    void setName(const char* name);

    const Cursor& cursor() const { return *cursor_; }
    void setCursor(const Cursor& cursor);

    const Point& cursorPos() const { return cursorPos_; }
    void setCursorPos(const Point& pos);

    bool supportsEncoding(rdr::S32 encoding) const;

    void setEncodings(int nEncodings, const rdr::S32* encodings);

    unsigned int ledState() { return ledState_; }
    void setLEDState(unsigned int state);

    rdr::U32 clipboardFlags() const { return clipFlags; }
    void setClipboardCaps(rdr::U32 flags, const rdr::U32* lengths);

    bool useCopyRect;

    bool supportsLocalCursor;
    bool supportsLocalXCursor;
    bool supportsLocalCursorWithAlpha;
    bool supportsVMWareCursor;
    bool supportsCursorPosition;
    bool supportsDesktopResize;
    bool supportsExtendedDesktopSize;
    bool supportsDesktopRename;
    bool supportsLastRect;
    bool supportsLEDState;
    bool supportsQEMUKeyEvent;
    bool supportsWEBP;

    bool supportsSetDesktopSize;
    bool supportsFence;
    bool supportsContinuousUpdates;
    bool supportsExtendedClipboard;

    bool supportsUdp;

    int compressLevel;
    int qualityLevel;
    int fineQualityLevel;
    int subsampling;

    // kasm exposed settings, skippable with -IgnoreClientSettingsKasm
    enum {
        KASM_JPEG_VIDEO_QUALITY,
        KASM_WEBP_VIDEO_QUALITY,
        KASM_TREAT_LOSSLESS,
        KASM_PREFER_BANDWIDTH,
        KASM_DYNAMIC_QUALITY_MIN,
        KASM_DYNAMIC_QUALITY_MAX,
        KASM_VIDEO_AREA,
        KASM_VIDEO_TIME,
        KASM_FRAME_RATE,
        KASM_MAX_VIDEO_RESOLUTION,

        KASM_NUM_SETTINGS
    };

    bool kasmPassed[KASM_NUM_SETTINGS];

  private:

    PixelFormat pf_;
    char* name_;
    Cursor* cursor_;
    Point cursorPos_;
    std::set<rdr::S32> encodings_;
    char verStr[13];
    int verStrPos;
    unsigned int ledState_;
    SMsgHandler *shandler;
    rdr::U32 clipFlags;
    rdr::U32 clipSizes[16];
  };
}
#endif

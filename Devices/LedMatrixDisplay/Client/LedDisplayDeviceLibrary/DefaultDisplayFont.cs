﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace LedDisplayDeviceLibrary
{
    // Source: https://github.com/dhepper/font8x8
    // Rotated

    public class DefaultDisplayFont
    {
        public static byte[] GetCharacterBitmap(char ch)
        {
            if (ch >= 0xA0 && ch <= 0xFF)
            {
                return font8x8_ext_latin[ch - 0xA0];
            }
            else if (ch >= 0x00 && ch <= 0x7F)
            {
                return font8x8_basic[ch];
            }
            else
            {
                return font8x8_invalid;
            }
        }

        public static byte[] font8x8_invalid = new byte[] { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

        // Constant: font8x8_basic
        // Contains an 8x8 font map for unicode points U+0000 - U+007F (basic latin)
        public static byte[][] font8x8_basic = new byte[128][] 
        {
            new byte[] {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
            new byte[] {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
            new byte[] {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
            new byte[] {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
            new byte[] {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
            new byte[] {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
            new byte[] {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
            new byte[] {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
            new byte[] {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
            new byte[] {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
            new byte[] {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
            new byte[] {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
            new byte[] {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
            new byte[] {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
            new byte[] {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
            new byte[] {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
            new byte[] {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
            new byte[] {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
            new byte[] {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
            new byte[] {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
            new byte[] {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
            new byte[] {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
            new byte[] {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
            new byte[] {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
            new byte[] {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
            new byte[] {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
            new byte[] {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
            new byte[] {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
            new byte[] {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
            new byte[] {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
            new byte[] {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
            new byte[] {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
            new byte[] {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
            new byte[] {0x00, 0x00, 0x60, 0xfa, 0xfa, 0x60, 0x00, 0x00},
            new byte[] {0x00, 0xc0, 0xc0, 0x00, 0xc0, 0xc0, 0x00, 0x00},
            new byte[] {0x28, 0xfe, 0xfe, 0x28, 0xfe, 0xfe, 0x28, 0x00},
            new byte[] {0x24, 0x74, 0xd6, 0xd6, 0x5c, 0x48, 0x00, 0x00},
            new byte[] {0x62, 0x66, 0x0c, 0x18, 0x30, 0x66, 0x46, 0x00},
            new byte[] {0x0c, 0x5e, 0xf2, 0xba, 0xec, 0x5e, 0x12, 0x00},
            new byte[] {0x20, 0xe0, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00},
            new byte[] {0x00, 0x38, 0x7c, 0xc6, 0x82, 0x00, 0x00, 0x00},
            new byte[] {0x00, 0x82, 0xc6, 0x7c, 0x38, 0x00, 0x00, 0x00},
            new byte[] {0x10, 0x54, 0x7c, 0x38, 0x38, 0x7c, 0x54, 0x10},
            new byte[] {0x10, 0x10, 0x7c, 0x7c, 0x10, 0x10, 0x00, 0x00},
            new byte[] {0x00, 0x01, 0x07, 0x06, 0x00, 0x00, 0x00, 0x00},
            new byte[] {0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00, 0x00},
            new byte[] {0x00, 0x00, 0x06, 0x06, 0x00, 0x00, 0x00, 0x00},
            new byte[] {0x06, 0x0c, 0x18, 0x30, 0x60, 0xc0, 0x80, 0x00},
            new byte[] {0x7c, 0xfe, 0x8e, 0x9a, 0xb2, 0xfe, 0x7c, 0x00},
            new byte[] {0x02, 0x42, 0xfe, 0xfe, 0x02, 0x02, 0x00, 0x00},
            new byte[] {0x46, 0xce, 0x9a, 0x92, 0xf6, 0x66, 0x00, 0x00},
            new byte[] {0x44, 0xc6, 0x92, 0x92, 0xfe, 0x6c, 0x00, 0x00},
            new byte[] {0x18, 0x38, 0x68, 0xca, 0xfe, 0xfe, 0x0a, 0x00},
            new byte[] {0xe4, 0xe6, 0xa2, 0xa2, 0xbe, 0x9c, 0x00, 0x00},
            new byte[] {0x3c, 0x7e, 0xd2, 0x92, 0x9e, 0x0c, 0x00, 0x00},
            new byte[] {0xc0, 0xc0, 0x8e, 0x9e, 0xf0, 0xe0, 0x00, 0x00},
            new byte[] {0x6c, 0xfe, 0x92, 0x92, 0xfe, 0x6c, 0x00, 0x00},
            new byte[] {0x60, 0xf2, 0x92, 0x96, 0xfc, 0x78, 0x00, 0x00},
            new byte[] {0x00, 0x00, 0x66, 0x66, 0x00, 0x00, 0x00, 0x00},
            new byte[] {0x00, 0x01, 0x67, 0x66, 0x00, 0x00, 0x00, 0x00},
            new byte[] {0x10, 0x38, 0x6c, 0xc6, 0x82, 0x00, 0x00, 0x00},
            new byte[] {0x24, 0x24, 0x24, 0x24, 0x24, 0x24, 0x00, 0x00},
            new byte[] {0x00, 0x82, 0xc6, 0x6c, 0x38, 0x10, 0x00, 0x00},
            new byte[] {0x40, 0xc0, 0x8a, 0x9a, 0xf0, 0x60, 0x00, 0x00},
            new byte[] {0x7c, 0xfe, 0x82, 0xba, 0xba, 0xf8, 0x78, 0x00},
            new byte[] {0x3e, 0x7e, 0xc8, 0xc8, 0x7e, 0x3e, 0x00, 0x00},
            new byte[] {0x82, 0xfe, 0xfe, 0x92, 0x92, 0xfe, 0x6c, 0x00},
            new byte[] {0x38, 0x7c, 0xc6, 0x82, 0x82, 0xc6, 0x44, 0x00},
            new byte[] {0x82, 0xfe, 0xfe, 0x82, 0xc6, 0x7c, 0x38, 0x00},
            new byte[] {0x82, 0xfe, 0xfe, 0x92, 0xba, 0x82, 0xc6, 0x00},
            new byte[] {0x82, 0xfe, 0xfe, 0x92, 0xb8, 0x80, 0xc0, 0x00},
            new byte[] {0x38, 0x7c, 0xc6, 0x82, 0x8a, 0xce, 0x4e, 0x00},
            new byte[] {0xfe, 0xfe, 0x10, 0x10, 0xfe, 0xfe, 0x00, 0x00},
            new byte[] {0x00, 0x82, 0xfe, 0xfe, 0x82, 0x00, 0x00, 0x00},
            new byte[] {0x0c, 0x0e, 0x02, 0x82, 0xfe, 0xfc, 0x80, 0x00},
            new byte[] {0x82, 0xfe, 0xfe, 0x10, 0x38, 0xee, 0xc6, 0x00},
            new byte[] {0x82, 0xfe, 0xfe, 0x82, 0x02, 0x06, 0x0e, 0x00},
            new byte[] {0xfe, 0xfe, 0x70, 0x38, 0x70, 0xfe, 0xfe, 0x00},
            new byte[] {0xfe, 0xfe, 0x60, 0x30, 0x18, 0xfe, 0xfe, 0x00},
            new byte[] {0x38, 0x7c, 0xc6, 0x82, 0xc6, 0x7c, 0x38, 0x00},
            new byte[] {0x82, 0xfe, 0xfe, 0x92, 0x90, 0xf0, 0x60, 0x00},
            new byte[] {0x78, 0xfc, 0x84, 0x8e, 0xfe, 0x7a, 0x00, 0x00},
            new byte[] {0x82, 0xfe, 0xfe, 0x90, 0x98, 0xfe, 0x66, 0x00},
            new byte[] {0x64, 0xf6, 0xb2, 0x9a, 0xce, 0x4c, 0x00, 0x00},
            new byte[] {0xc0, 0x82, 0xfe, 0xfe, 0x82, 0xc0, 0x00, 0x00},
            new byte[] {0xfe, 0xfe, 0x02, 0x02, 0xfe, 0xfe, 0x00, 0x00},
            new byte[] {0xf8, 0xfc, 0x06, 0x06, 0xfc, 0xf8, 0x00, 0x00},
            new byte[] {0xfe, 0xfe, 0x0c, 0x18, 0x0c, 0xfe, 0xfe, 0x00},
            new byte[] {0xc2, 0xe6, 0x3c, 0x18, 0x3c, 0xe6, 0xc2, 0x00},
            new byte[] {0xe0, 0xf2, 0x1e, 0x1e, 0xf2, 0xe0, 0x00, 0x00},
            new byte[] {0xe2, 0xc6, 0x8e, 0x9a, 0xb2, 0xe6, 0xce, 0x00},
            new byte[] {0x00, 0xfe, 0xfe, 0x82, 0x82, 0x00, 0x00, 0x00},
            new byte[] {0x80, 0xc0, 0x60, 0x30, 0x18, 0x0c, 0x06, 0x00},
            new byte[] {0x00, 0x82, 0x82, 0xfe, 0xfe, 0x00, 0x00, 0x00},
            new byte[] {0x10, 0x30, 0x60, 0xc0, 0x60, 0x30, 0x10, 0x00},
            new byte[] {0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01},
            new byte[] {0x00, 0x00, 0xc0, 0xe0, 0x20, 0x00, 0x00, 0x00},
            new byte[] {0x04, 0x2e, 0x2a, 0x2a, 0x3c, 0x1e, 0x02, 0x00},
            new byte[] {0x82, 0xfe, 0xfc, 0x12, 0x12, 0x1e, 0x0c, 0x00},
            new byte[] {0x1c, 0x3e, 0x22, 0x22, 0x36, 0x14, 0x00, 0x00},
            new byte[] {0x0c, 0x1e, 0x12, 0x92, 0xfc, 0xfe, 0x02, 0x00},
            new byte[] {0x1c, 0x3e, 0x2a, 0x2a, 0x3a, 0x18, 0x00, 0x00},
            new byte[] {0x12, 0x7e, 0xfe, 0x92, 0xc0, 0x40, 0x00, 0x00},
            new byte[] {0x19, 0x3d, 0x25, 0x25, 0x1f, 0x3e, 0x20, 0x00},
            new byte[] {0x82, 0xfe, 0xfe, 0x10, 0x20, 0x3e, 0x1e, 0x00},
            new byte[] {0x00, 0x22, 0xbe, 0xbe, 0x02, 0x00, 0x00, 0x00},
            new byte[] {0x06, 0x07, 0x01, 0x01, 0xbf, 0xbe, 0x00, 0x00},
            new byte[] {0x82, 0xfe, 0xfe, 0x08, 0x1c, 0x36, 0x22, 0x00},
            new byte[] {0x00, 0x82, 0xfe, 0xfe, 0x02, 0x00, 0x00, 0x00},
            new byte[] {0x3e, 0x3e, 0x18, 0x1c, 0x38, 0x3e, 0x1e, 0x00},
            new byte[] {0x3e, 0x3e, 0x20, 0x20, 0x3e, 0x1e, 0x00, 0x00},
            new byte[] {0x1c, 0x3e, 0x22, 0x22, 0x3e, 0x1c, 0x00, 0x00},
            new byte[] {0x21, 0x3f, 0x1f, 0x25, 0x24, 0x3c, 0x18, 0x00},
            new byte[] {0x18, 0x3c, 0x24, 0x25, 0x1f, 0x3f, 0x21, 0x00},
            new byte[] {0x22, 0x3e, 0x1e, 0x32, 0x20, 0x38, 0x18, 0x00},
            new byte[] {0x12, 0x3a, 0x2a, 0x2a, 0x2e, 0x24, 0x00, 0x00},
            new byte[] {0x00, 0x20, 0x7c, 0xfe, 0x22, 0x24, 0x00, 0x00},
            new byte[] {0x3c, 0x3e, 0x02, 0x02, 0x3c, 0x3e, 0x02, 0x00},
            new byte[] {0x38, 0x3c, 0x06, 0x06, 0x3c, 0x38, 0x00, 0x00},
            new byte[] {0x3c, 0x3e, 0x0e, 0x1c, 0x0e, 0x3e, 0x3c, 0x00},
            new byte[] {0x22, 0x36, 0x1c, 0x08, 0x1c, 0x36, 0x22, 0x00},
            new byte[] {0x39, 0x3d, 0x05, 0x05, 0x3f, 0x3e, 0x00, 0x00},
            new byte[] {0x32, 0x26, 0x2e, 0x3a, 0x32, 0x26, 0x00, 0x00},
            new byte[] {0x10, 0x10, 0x7c, 0xee, 0x82, 0x82, 0x00, 0x00},
            new byte[] {0x00, 0x00, 0x00, 0xee, 0xee, 0x00, 0x00, 0x00},
            new byte[] {0x82, 0x82, 0xee, 0x7c, 0x10, 0x10, 0x00, 0x00},
            new byte[] {0x40, 0xc0, 0x80, 0xc0, 0x40, 0xc0, 0x80, 0x00},
            new byte[] {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
        };

        // Constant: font8x8_00A0
        // Contains an 8x8 font map for unicode points U+00A0 - U+00FF (extended latin)
        public static byte[][] font8x8_ext_latin = new byte[96][]
        {
            new byte[] { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
            new byte[] { 0x00, 0x00, 0x00, 0xde, 0xde, 0x00, 0x00, 0x00},
            new byte[] { 0x00, 0x24, 0x24, 0xe7, 0xe7, 0x24, 0x3c, 0x18},
            new byte[] { 0x00, 0x04, 0x66, 0xc2, 0x92, 0xfe, 0x7e, 0x16},
            new byte[] { 0x00, 0x22, 0x3e, 0x1c, 0x14, 0x1c, 0x3e, 0x22},
            new byte[] { 0x00, 0x00, 0xd4, 0xf4, 0x3f, 0x3f, 0xf4, 0xd4},
            new byte[] { 0x00, 0x00, 0x00, 0xee, 0xee, 0x00, 0x00, 0x00},
            new byte[] { 0x40, 0xc0, 0x9a, 0xbf, 0xa5, 0xfd, 0x5b, 0x02},
            new byte[] { 0x00, 0x00, 0x80, 0x80, 0x00, 0x00, 0x80, 0x80},
            new byte[] { 0x3c, 0x42, 0x81, 0xa5, 0xa5, 0x99, 0x42, 0x3c},
            new byte[] { 0x00, 0x10, 0xf0, 0xf0, 0x90, 0xf0, 0x60, 0x00},
            new byte[] { 0x44, 0x6c, 0x38, 0x10, 0x44, 0x6c, 0x38, 0x10},
            new byte[] { 0x00, 0x00, 0x1c, 0x1c, 0x10, 0x10, 0x10, 0x10},
            new byte[] { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
            new byte[] { 0x3c, 0x42, 0x95, 0xa9, 0xa9, 0xbd, 0x42, 0x3c},
            new byte[] { 0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00},
            new byte[] { 0x00, 0x00, 0x60, 0xf0, 0x90, 0xf0, 0x60, 0x00},
            new byte[] { 0x00, 0x22, 0x22, 0xfa, 0xfa, 0x22, 0x22, 0x00},
            new byte[] { 0x00, 0x00, 0x48, 0xe8, 0xb8, 0x98, 0x00, 0x00},
            new byte[] { 0x00, 0x00, 0x50, 0xf8, 0xa8, 0x88, 0x00, 0x00},
            new byte[] { 0x00, 0x00, 0x00, 0x80, 0xc0, 0x40, 0x00, 0x00},
            new byte[] { 0x00, 0x38, 0x3c, 0x04, 0x04, 0x3e, 0x3f, 0x01},
            new byte[] { 0xfe, 0xfe, 0x80, 0xfe, 0xfe, 0x90, 0xf0, 0x60},
            new byte[] { 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00},
            new byte[] { 0x00, 0x00, 0x02, 0x07, 0x05, 0x01, 0x01, 0x00},
            new byte[] { 0x00, 0x00, 0x00, 0x10, 0xf0, 0x50, 0x00, 0x00},
            new byte[] { 0x00, 0x00, 0x60, 0xf0, 0x90, 0xf0, 0x60, 0x00},
            new byte[] { 0x10, 0x38, 0x6c, 0x44, 0x10, 0x38, 0x6c, 0x44},
            new byte[] { 0x9e, 0xce, 0x7e, 0x36, 0x18, 0x1c, 0xe7, 0xf3},
            new byte[] { 0x89, 0xdd, 0x77, 0x33, 0x18, 0x0c, 0xf6, 0xf2},
            new byte[] { 0x5f, 0x6a, 0x36, 0x18, 0x0c, 0x56, 0xaa, 0xa8},
            new byte[] { 0x00, 0x00, 0x04, 0x06, 0xa2, 0xb2, 0x1e, 0x0c},
            new byte[] { 0x00, 0x0e, 0x1e, 0x34, 0x24, 0xb4, 0x9e, 0x8e},
            new byte[] { 0x00, 0x8e, 0x9e, 0xb4, 0x24, 0x34, 0x1e, 0x0e},
            new byte[] { 0x00, 0x0e, 0x5e, 0xd4, 0x94, 0xd4, 0x5e, 0x0e},
            new byte[] { 0x00, 0x8e, 0xde, 0x54, 0xd4, 0x94, 0xde, 0x4e},
            new byte[] { 0x00, 0x9e, 0xbe, 0x68, 0x48, 0x68, 0xbe, 0x9e},
            new byte[] { 0x00, 0x00, 0x0e, 0x1e, 0xd4, 0xd4, 0x1e, 0x0e},
            new byte[] { 0x00, 0x92, 0xfe, 0xfe, 0x90, 0xd0, 0x7e, 0x3e},
            new byte[] { 0x00, 0x00, 0x52, 0xdf, 0x8d, 0x89, 0xf9, 0x70},
            new byte[] { 0x00, 0x00, 0x22, 0x2a, 0x2a, 0xbe, 0xbe, 0xa2},
            new byte[] { 0x00, 0x00, 0xa2, 0xaa, 0xaa, 0x3e, 0x3e, 0x22},
            new byte[] { 0x00, 0x00, 0x22, 0x6a, 0xaa, 0xbe, 0x7e, 0x22},
            new byte[] { 0x00, 0x00, 0xa2, 0xaa, 0x2a, 0xbe, 0xbe, 0x22},
            new byte[] { 0x00, 0x00, 0x00, 0x22, 0x3e, 0xbe, 0xa2, 0x80},
            new byte[] { 0x00, 0x00, 0x80, 0xa2, 0xbe, 0x3e, 0x22, 0x00},
            new byte[] { 0x00, 0x00, 0x00, 0x52, 0x9e, 0x9e, 0x52, 0x00},
            new byte[] { 0x00, 0x00, 0x80, 0xa2, 0x3e, 0x3e, 0xa2, 0x80},
            new byte[] { 0x00, 0x7c, 0xfe, 0x82, 0xb2, 0xfe, 0xfe, 0xb2},
            new byte[] { 0x00, 0x00, 0xbe, 0xbe, 0x8c, 0x98, 0xbe, 0xbe},
            new byte[] { 0x00, 0x08, 0x1c, 0x36, 0xb6, 0x9c, 0x88, 0x00},
            new byte[] { 0x00, 0x88, 0x9c, 0xb6, 0x36, 0x1c, 0x08, 0x00},
            new byte[] { 0x00, 0x48, 0xdc, 0xb6, 0xb6, 0xdc, 0x48, 0x00},
            new byte[] { 0x00, 0x8c, 0xde, 0x52, 0xd2, 0x92, 0xde, 0x4c},
            new byte[] { 0x80, 0x98, 0x3c, 0x66, 0x66, 0x3c, 0x98, 0x80},
            new byte[] { 0x00, 0x00, 0x44, 0x6c, 0x38, 0x6c, 0x44, 0x00},
            new byte[] { 0x00, 0xb8, 0x7c, 0xf6, 0x9a, 0xce, 0x7c, 0x3a},
            new byte[] { 0x00, 0x3c, 0x3e, 0x02, 0x82, 0xbe, 0xbc, 0x00},
            new byte[] { 0x00, 0xbc, 0xbe, 0x82, 0x02, 0x3e, 0x3c, 0x00},
            new byte[] { 0x00, 0x5c, 0xde, 0x82, 0x82, 0xde, 0x5c, 0x00},
            new byte[] { 0x00, 0x00, 0xbc, 0xbe, 0x02, 0x02, 0xbe, 0xbc},
            new byte[] { 0x00, 0xb0, 0xb8, 0x8e, 0x0e, 0x38, 0x30, 0x00},
            new byte[] { 0x00, 0x18, 0x3c, 0x24, 0xa5, 0xff, 0xff, 0x81},
            new byte[] { 0x00, 0x00, 0x28, 0x7c, 0x54, 0x54, 0x7f, 0x3f},
            new byte[] { 0x00, 0x02, 0x1e, 0x3e, 0x2a, 0xaa, 0xae, 0x84},
            new byte[] { 0x00, 0x02, 0x9e, 0xbe, 0xaa, 0x2a, 0x2e, 0x04},
            new byte[] { 0x42, 0xde, 0xbe, 0xaa, 0xaa, 0xae, 0xc4, 0x40},
            new byte[] { 0x00, 0x82, 0xde, 0x7e, 0xea, 0xaa, 0xee, 0x44},
            new byte[] { 0x00, 0x02, 0x9e, 0xbe, 0x2a, 0x2a, 0xae, 0x84},
            new byte[] { 0x00, 0x02, 0x1e, 0x3e, 0xea, 0xea, 0x2e, 0x04},
            new byte[] { 0x2a, 0x2a, 0x3e, 0x3e, 0x2a, 0x2a, 0x2e, 0x04},
            new byte[] { 0x00, 0x00, 0x02, 0x27, 0x25, 0x25, 0x3c, 0x18},
            new byte[] { 0x00, 0x00, 0x18, 0x3a, 0x2a, 0xaa, 0xbe, 0x9c},
            new byte[] { 0x00, 0x00, 0x98, 0xba, 0xaa, 0x2a, 0x3e, 0x1c},
            new byte[] { 0x40, 0xd8, 0xba, 0xaa, 0xaa, 0xbe, 0xdc, 0x40},
            new byte[] { 0x00, 0x00, 0x98, 0xba, 0x2a, 0x2a, 0xbe, 0x9c},
            new byte[] { 0x00, 0x00, 0x00, 0x02, 0x3e, 0xbe, 0xa2, 0x80},
            new byte[] { 0x00, 0x00, 0x00, 0x82, 0xbe, 0xbe, 0x22, 0x00},
            new byte[] { 0x00, 0x40, 0xc2, 0xbe, 0xbe, 0xa2, 0xc0, 0x40},
            new byte[] { 0x00, 0x00, 0x80, 0x82, 0x3e, 0x3e, 0xa2, 0x80},
            new byte[] { 0x00, 0x00, 0x1c, 0xbe, 0xea, 0x4a, 0xee, 0xa4},
            new byte[] { 0x00, 0x00, 0x0e, 0x5e, 0x50, 0x50, 0x5e, 0x5e},
            new byte[] { 0x00, 0x00, 0x0c, 0x1e, 0x12, 0x52, 0x5e, 0x4c},
            new byte[] { 0x00, 0x00, 0x4c, 0x5e, 0x52, 0x12, 0x1e, 0x0c},
            new byte[] { 0x00, 0x00, 0x4c, 0xde, 0x92, 0x92, 0xde, 0x4c},
            new byte[] { 0x00, 0x80, 0xcc, 0x5e, 0xd2, 0x92, 0xde, 0x4c},
            new byte[] { 0x00, 0x00, 0x4c, 0x5e, 0x12, 0x12, 0x5e, 0x4c},
            new byte[] { 0x00, 0x10, 0x10, 0xd6, 0xd6, 0x10, 0x10, 0x00},
            new byte[] { 0x00, 0x5c, 0x7e, 0x3a, 0x2e, 0x3f, 0x1d, 0x00},
            new byte[] { 0x00, 0x02, 0x1e, 0x1e, 0x02, 0x42, 0x5e, 0x5c},
            new byte[] { 0x00, 0x02, 0x5e, 0x5e, 0x42, 0x02, 0x1e, 0x1c},
            new byte[] { 0x00, 0x02, 0x5e, 0xde, 0x82, 0x82, 0xde, 0x5c},
            new byte[] { 0x00, 0x02, 0x5e, 0x5e, 0x02, 0x02, 0x5e, 0x5c},
            new byte[] { 0x00, 0x00, 0x5e, 0x5f, 0x45, 0x05, 0x1d, 0x19},
            new byte[] { 0x00, 0x08, 0x1c, 0x14, 0x14, 0x3e, 0x3e, 0x00},
            new byte[] { 0x00, 0x00, 0x5e, 0x5f, 0x05, 0x05, 0x5d, 0x59}
        };
    }
}

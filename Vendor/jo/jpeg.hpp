// Simple and minimalistic JPEG writer
// Author(s):       Jon Olick <http://jonolick.com>
//					Yao Wei Tjong
//					iFarbod <ifarbod@outlook.com>
//
// Copyright (c) 2015-2017 Project CtNorth
//
// Distributed under the MIT license (See accompanying file LICENSE or copy at
// https://opensource.org/licenses/MIT)

// Modified by iFarbod for SAO

#pragma once


namespace JO
{

enum JO_Components
{
	JO_JPG_LUMINANCE = 1,
	JO_JPG_RGB = 3,
	JO_JPG_RGBX
};

// Writes JPG rgb stream to a file. Returns false on failure.
// Components can be either 1, 3 or 4 (Luminance, RGB or RGBX respectively)
bool WriteJPG(const char *filename, const void *data, int width, int height, int components, int quality);

}

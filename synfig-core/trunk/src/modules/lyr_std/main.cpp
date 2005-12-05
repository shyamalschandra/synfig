/*! ========================================================================
** Synfig
** bleh
** $Id: main.cpp,v 1.1.1.1 2005/01/04 01:23:10 darco Exp $
**
**	Copyright (c) 2002-2005 Robert B. Quattlebaum Jr., Adrian Bentley
**
**	This package is free software; you can redistribute it and/or
**	modify it under the terms of the GNU General Public License as
**	published by the Free Software Foundation; either version 2 of
**	the License, or (at your option) any later version.
**
**	This package is distributed in the hope that it will be useful,
**	but WITHOUT ANY WARRANTY; without even the implied warranty of
**	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
**	General Public License for more details.
**
** === N O T E S ===========================================================
**
** ========================================================================= */

/* === H E A D E R S ======================================================= */

#define SYNFIG_MODULE

#ifdef USING_PCH
#	include "pch.h"
#else
#ifdef HAVE_CONFIG_H
#	include <config.h>
#endif

#include <synfig/module.h>
#include <synfig/string.h>

#include "zoom.h"
//#include "blur.h"
#include "import.h"
#include "translate.h"
#include "rotate.h"
#include "clamp.h"
#include "stretch.h"

//#include "colorcorrect.h"

#include "supersample.h"

#include "mandelbrot.h"
#include "julia.h"
#include "insideout.h"
#include "xorpattern.h"
#include "twirl.h"
#include "sphere_distort.h"



#include "shade.h"
#include "bevel.h"
//#include "halftone2.h"

//#include "radialblur.h"

#include "warp.h"
#include "timeloop.h"

#endif

/* === E N T R Y P O I N T ================================================= */

MODULE_DESC_BEGIN(liblyr_std)
	MODULE_NAME("Standard Layers")
	MODULE_DESCRIPTION("Provides a basic set of standard layers")
	MODULE_AUTHOR("Robert B. Quattlebaum")
	MODULE_VERSION("1.0")
	MODULE_COPYRIGHT(SYNFIG_COPYRIGHT)
MODULE_DESC_END

MODULE_INVENTORY_BEGIN(liblyr_std)
	BEGIN_LAYERS
		LAYER(Zoom)
//		LAYER(Blur_Layer)
//		LAYER(RadialBlur)
		LAYER(Import)
		LAYER(Translate)
		LAYER(SuperSample)
		LAYER(Rotate)
		LAYER(Warp)

		LAYER_ALIAS(Zoom,"Zoom")
		LAYER_ALIAS(Translate,"Translate")
		LAYER_ALIAS(SuperSample,"SuperSample")
		LAYER_ALIAS(Rotate,"Rotate")
		LAYER_ALIAS(Import,"Import")
//		LAYER_ALIAS(Blur_Layer,"Blur")

//		LAYER(Halftone2)

		LAYER(Julia)
		LAYER(InsideOut)
		LAYER(Mandelbrot)
		LAYER(Layer_Clamp)
		LAYER(Layer_Stretch)
//		LAYER(Layer_ColorCorrect)
//		LAYER(XORPattern)
		LAYER(Twirl)
		LAYER(Layer_Shade)
		LAYER(Layer_Bevel)
		LAYER(Layer_TimeLoop)
		LAYER(Layer_SphereDistort)
	END_LAYERS
MODULE_INVENTORY_END

/* === S Y N F I G ========================================================= */
/*!	\file synfig/rendering/software/optimizer/optimizercontoursw.cpp
**	\brief OptimizerContourSW
**
**	$Id$
**
**	\legal
**	......... ... 2015 Ivan Mahonin
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
**	\endlegal
*/
/* ========================================================================= */

/* === H E A D E R S ======================================================= */

#ifdef USING_PCH
#	include "pch.h"
#else
#ifdef HAVE_CONFIG_H
#	include <config.h>
#endif

#ifndef _WIN32
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#endif

#include "optimizercontoursw.h"

#include "../task/taskcontoursw.h"

#endif

using namespace synfig;
using namespace rendering;

/* === M A C R O S ========================================================= */

/* === G L O B A L S ======================================================= */

/* === P R O C E D U R E S ================================================= */

/* === M E T H O D S ======================================================= */

void
OptimizerContourSW::run(const RunParams& params) const
{
	TaskContour::Handle contour = TaskContour::Handle::cast_dynamic(params.ref_task);
	if ( contour
	  && contour->target_surface
	  && contour.type_equal<TaskContour>() )
	{
		apply(params, create_and_assign<TaskContourSW>(contour));
	}
}

/* === E N T R Y P O I N T ================================================= */

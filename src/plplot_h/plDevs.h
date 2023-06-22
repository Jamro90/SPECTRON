// -*-C-*-
//
//  Maurice LeBrun
//  IFS, University of Texas at Austin
//  18-Jul-1994
//
//  Contains macro definitions that determine what device drivers are
//  compiled into the PLplot library.  On a Unix system, typically the
//  configure script builds plDevs.h from plDevs.h.in.  Elsewhere, it's
//  best to hand-configure a plDevs.h file and keep it with the
//  system-specific files.
//
// Copyright (C) 1994-2002 Maurice LeBrun
// Copyright (C) 1996-2001 Geoffrey Furnish
// Copyright (C) 2001 Joao Cardoso
// Copyright (C) 2001-2005 Rafael Laboissiere
// Copyright (C) 2001-2019 Alan W. Irwin
// Copyright (C) 2002 Vince Darley
// Copyright (C) 2004 Andrew Roach
// Copyright (C) 2005 Thomas Duck
// Copyright (C) 2005-2012 Hazen Babcock
// Copyright (C) 2006 Andrew Ross
// Copyright (C) 2006-2008 Werner Smekal
// Copyright (C) 2015-2017 Jim Dishaw
//
//  This file is part of PLplot.
//
//  PLplot is free software; you can redistribute it and/or modify
//  it under the terms of the GNU Library General Public License as published
//  by the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  PLplot is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Library General Public License for more details.
//
//  You should have received a copy of the GNU Library General Public License
//  along with PLplot; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
//
//

#ifndef __PLDEVS_H__
#define __PLDEVS_H__

// Same order (by source filename and by device) as in drivers.h.
// ps and psttf are special cases where two devices are handled with
// one PLD macro.
// xwin is special case where the macro name is PLD_xwin but the
// function name in drivers.h is plD_dispatch_init_xw

// Maintenance 2018-10
// Should include every device potentially enabled by device drivers.
/* #undef PLD_aqt */
#define PLD_epscairo
#define PLD_extcairo
#define PLD_memcairo
#define PLD_pdfcairo
#define PLD_pngcairo
#define PLD_pscairo
#define PLD_svgcairo
/* #undef PLD_wincairo */
#define PLD_xcairo
/* #undef PLD_cgm */
/* #undef PLD_gif */
/* #undef PLD_jpeg */
/* #undef PLD_png */
#define PLD_mem
#define PLD_ntk
#define PLD_null
#define PLD_pdf
/* #undef PLD_plm */
#define PLD_ps
#define PLD_psc
/* #undef PLD_pstex */
/* #undef PLD_psttf */
/* #undef PLD_psttfc */
#define PLD_bmpqt
/* #undef PLD_epsqt */
#define PLD_extqt
#define PLD_jpgqt
#define PLD_memqt
#define PLD_pdfqt
#define PLD_pngqt
#define PLD_ppmqt
#define PLD_qtwidget
#define PLD_svgqt
#define PLD_tiffqt
#define PLD_svg
#define PLD_tk
#define PLD_tkwin
/* #undef PLD_wingcc */
/* #undef PLD_wxpng */
#define PLD_wxwidgets
#define PLD_xfig
#define PLD_xwin
/* #undef PLD_wingdi */

#endif  // __PLDEVS_H__


/*
 *                 ------------------------------------
 *                 iLab Neuromorphic Vision C++ Toolkit
 *                 ------------------------------------
 *
 *      The iLab Neuromorphic Vision C++ Toolkit - Copyright (C)
 *      2001-2005 by the University of Southern California (USC) and
 *      iLab at USC.
 *
 *      Major portions of the iLab Neuromorphic Vision C++ Toolkit are
 *      protected under the U.S. patent ``Computation of Intrinsic
 *      Perceptual Saliency in Visual Environments, and Applications''
 *      by Christof Koch and Laurent Itti, California Institute of
 *      Technology, 2001 (patent pending; application number
 *      09/912,225 filed July 23, 2001; see
 *      http://pair.uspto.gov/cgi-bin/final/home.pl for current
 *      status)
 *
 *      This file is part of the iLab Neuromorphic Vision C++ Toolkit.
 *
 *      The iLab Neuromorphic Vision C++ Toolkit is free software; you
 *      can redistribute it and/or modify it under the terms of the
 *      GNU General Public License as published by the Free Software
 *      Foundation; either version 2 of the License, or (at your
 *      option) any later version.
 *
 *      The iLab Neuromorphic Vision C++ Toolkit is distributed in the
 *      hope that it will be useful, but WITHOUT ANY WARRANTY; without
 *      even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 *      PARTICULAR PURPOSE.  See the GNU General Public License for
 *      more details.
 *
 *      You should have received a copy of the GNU General Public
 *      License along with the iLab Neuromorphic Vision C++ Toolkit
 *      (see the top-level file named COPYING); if not, write to the
 *      Free Software Foundation, Inc., 59 Temple Place, Suite 330,
 *      Boston, MA 02111-1307 USA.
 */

// ######################################################################
//T.Nathan Mundhenk
// mundhenk @ usc.edu
// C / C++ Macro HSV to RGB
#define PIX_HSV_TO_RGB_COMMON(H,S,V,R,G,B)                          \
if( V == 0 )                                                        \
{ R = 0; G = 0; B = 0; }                                            \
else if( S == 0 )                                                   \
{                                                                   \
  R = V;                                                            \
  G = V;                                                            \
  B = V;                                                            \
}                                                                   \
else                                                                \
{                                                                   \
  const double hf = H / 60.0;                                       \
  const int    i  = (int) floor( hf );                              \
  const double f  = hf - i;                                         \
  const double pv  = V * ( 1 - S );                                 \
  const double qv  = V * ( 1 - S * f );                             \
  const double tv  = V * ( 1 - S * ( 1 - f ) );                     \
  switch( i )                                                       \
  {                                                                 \
  case 0:                                                           \
    R = V;                                                          \
    G = tv;                                                         \
    B = pv;                                                         \
    break;                                                          \
  case 1:                                                           \
    R = qv;                                                         \
    G = V;                                                          \
    B = pv;                                                         \
    break;                                                          \
  case 2:                                                           \
    R = pv;                                                         \
    G = V;                                                          \
    B = tv;                                                         \
    break;                                                          \
  case 3:                                                           \
    R = pv;                                                         \
    G = qv;                                                         \
    B = V;                                                          \
    break;                                                          \
  case 4:                                                           \
    R = tv;                                                         \
    G = pv;                                                         \
    B = V;                                                          \
    break;                                                          \
  case 5:                                                           \
    R = V;                                                          \
    G = pv;                                                         \
    B = qv;                                                         \
    break;                                                          \
  case 6:                                                           \
    R = V;                                                          \
    G = tv;                                                         \
    B = pv;                                                         \
    break;                                                          \
  case -1:                                                          \
    R = V;                                                          \
    G = pv;                                                         \
    B = qv;                                                         \
    break;                                                          \
  default:                                                          \
    abort();                                                        \
  break;                                                            \
  }                                                                 \
}                                                                   \
R *= 255.0F;                                                        \
G *= 255.0F;                                                        \
B *= 255.0F;


/*
 * IPv4 Heatmap
 * (C) 2007 The Measurement Factory, Inc
 * Licensed under the GPL, version 2
 * http://maps.measurement-factory.com/
 */


struct bb {
  int xmin, ymin, xmax, ymax;
};

typedef struct bb bbox;

int cidr_parse(const char *cidr, unsigned int *rfirst, unsigned int *rlast, int *rslash);
void hil_xy_from_s(unsigned s, int order, unsigned *xp, unsigned *yp);
void mor_xy_from_s(unsigned s, int order, unsigned *xp, unsigned *yp);
unsigned int xy_from_ip(unsigned ip, unsigned *xp, unsigned *yp);
bbox bbox_from_cidr(const char *prefix);
void (*xy_from_s) (unsigned s, int n, unsigned *xp, unsigned *yp) = hil_xy_from_s;

#define BBOX_WIDTH(b) (b.xmax-b.xmin)
#define BBOX_HEIGHT(b) (b.ymax-b.ymin)
#define BBOX_CTR_X(b) ((b.xmax+b.xmin)/2)
#define BBOX_CTR_Y(b) ((b.ymax+b.ymin)/2)
#define BBOX_SET(B,W,X,Y,Z) B.xmin=W; B.ymin=X; B.xmax=Y; B.ymax=Z;
#define BBOX_PRINT(B) fprintf(stderr, "%s=%d,%d,%d,%d\n", #B, B.xmin,B.ymin,B.xmax,B.ymax)

/**
 * \file AuxAngle.cpp
 * \brief Implementation for the GeographicLib::experimental::AuxAngle class.
 *
 * \note This is just sample code.  It is not part of GeographicLib itself.
 *
 * This file is an implementation of the methods described in
 * - C. F. F. Karney,
 *   On auxiliary latitudes,
 *   Technical Report, SRI International, December 2022.
 *   https://arxiv.org/abs/2212.05818
 * .
 * Copyright (c) Charles Karney (2022-2023) <charles@karney.com> and licensed
 * under the MIT/X11 License.  For more information, see
 * https://geographiclib.sourceforge.io/
 **********************************************************************/

#include <GeographicLib/AuxAngle.hpp>

namespace GeographicLib {

  using namespace std;

  AuxAngle AuxAngle::NaN() {
    return AuxAngle(std::numeric_limits<real>::quiet_NaN(),
                    std::numeric_limits<real>::quiet_NaN());
  }

  AuxAngle AuxAngle::normalized() const {
    if ( isnan( tan() ) ||
         (fabs(_y) > std::numeric_limits<real>::max()/2 &&
          fabs(_x) > std::numeric_limits<real>::max()/2) )
      // deal with
      // (0,0), (inf,inf), (nan,nan), (nan,x), (y,nan), (toobig,toobig)
      return NaN();
    real r = hypot(_y, _x),
      y = _y/r, x = _x/r;
    // deal with r = inf, then one of y,x becomes 1
    if (isnan(y)) y = copysign(real(1), _y);
    if (isnan(x)) x = copysign(real(1), _x);
    return AuxAngle(y, x);
  }

  AuxAngle AuxAngle::copyquadrant(const AuxAngle& p) const {
    return AuxAngle(copysign(y(), p.y()), copysign(x(), p.x()));
  }

  AuxAngle& AuxAngle::operator+=(const AuxAngle& p) {
    // Do nothing if p.tan() == 0 to preserve signs of y() and x()
    if (p.tan() != 0) {
      real x = _x * p._x - _y * p._y;
      _y = _y * p._x + _x * p._y;
      _x = x;
    }
    return *this;
  }

} // namespace GeographicLib

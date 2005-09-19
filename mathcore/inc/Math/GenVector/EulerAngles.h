// @(#)root/mathcore:$Name:  $:$Id: EulerAngles.h,v 1.1 2005/09/18 17:33:47 brun Exp $
// Authors: W. Brown, M. Fischler, L. Moneta    2005  

 /**********************************************************************
  *                                                                    *
  * Copyright (c) 2005 , LCG ROOT MathLib Team                         *
  *                                                                    *
  *                                                                    *
  **********************************************************************/

// Header file for class EulerAngles
//
// Created by: Lorenzo Moneta  at Tue May 10 17:55:10 2005
//
// Last update: Tue May 10 17:55:10 2005
//
#ifndef ROOT_Math_GenVector_EulerAngles 
#define ROOT_Math_GenVector_EulerAngles  1

#include "Math/GenVector/Rotation3D.h"
#include "Math/GenVector/DisplacementVector3D.h"
#include "Math/GenVector/PositionVector3D.h"
#include "Math/GenVector/LorentzVector.h"
#include "Math/GenVector/3DConversions.h"
#include <algorithm>
#include <cassert>

namespace ROOT {
namespace Math {


  /**
     EulerAngles class describing rotation as three angles (Euler Angles).
     The Euler angles definition matches that of Classical Mechanics (Goldstein).
     See also 
     <A HREF="http://mathworld.wolfram.com/EulerAngles.html">mathworld</A> 
     Euler angles definition.
  */
class EulerAngles {

public:

  typedef double Scalar;

  /**
     Default constructor
  */
  EulerAngles() : fPhi(0.0), fTheta(0.0), fPsi(0.0) { }

  /**
     Constructor from phi, theta and psi
  */
  EulerAngles( Scalar phi, Scalar theta, Scalar psi ) :
    fPhi(phi), fTheta(theta), fPsi(psi)
  {}

  /**
     Construct given a pair of pointers or iterators defining the
     beginning and end of an array of three Scalars, to be treated as
     the angles phi, theta and psi.
   */
  template<class IT>
  EulerAngles(IT begin, IT end) { SetComponents(begin,end); }

  // The compiler-generated copy ctor, copy assignment, and dtor are OK.

  /**
     Re-adjust components place angles in canonical ranges
   */
  void Rectify();


  // ======== Construction From other Rotation Forms ==================

  /**
     Construct from a rotation matrix
  */
  explicit EulerAngles(const Rotation3D & r) {gv_detail::convert(r,*this);}

  /**
     Construct from a rotation represented by a Quaternion
  */
  explicit EulerAngles(const Quaternion & q) {gv_detail::convert(q,*this);}

  /**
     Construct from an AxisAngle
  */
  explicit EulerAngles(const AxisAngle & a ) { gv_detail::convert(a, *this); }

  /**
     Construct from an axial rotation
  */
  explicit EulerAngles( RotationZ const & r ) { gv_detail::convert(r, *this); }
  explicit EulerAngles( RotationY const & r ) { gv_detail::convert(r, *this); }
  explicit EulerAngles( RotationX const & r ) { gv_detail::convert(r, *this); }

  /**
     Assign from a Rotation3D
  */
  EulerAngles &
  operator=( Rotation3D const  & r ) { return operator=(EulerAngles(r)); }

  /**
     Assign from an AxisAngle
  */
  EulerAngles &
  operator=( AxisAngle const & a ) { return operator=(EulerAngles(a)); }

  /**
     Assign from a Quaternion
  */
  EulerAngles &
  operator=( Quaternion const  & q ) {return operator=(EulerAngles(q)); }

  /**
     Assign from an axial rotation
  */
  EulerAngles &
  operator=( RotationZ const & r ) { return operator=(EulerAngles(r)); }
  EulerAngles &
  operator=( RotationY const & r ) { return operator=(EulerAngles(r)); }
  EulerAngles &
  operator=( RotationX const & r ) { return operator=(EulerAngles(r)); }

  // ======== Components ==============

  /**
     Set the three Euler angles given a pair of pointers or iterators
     defining the beginning and end of an array of three Scalars.
   */
  template<class IT>
  void SetComponents(IT begin, IT end) {
    assert (end==begin+3);
    fPhi   = *begin++;
    fTheta = *begin++;
    fPsi   = *begin;
  }

  /**
     Get the axis and then the angle into data specified by an iterator begin
     and another to the end of the desired data (4 past start).
   */
  template<class IT>
  void GetComponents(IT begin, IT end) const {
    assert (end==begin+4);
    *begin++ = fPhi;
    *begin++ = fTheta;
    *begin   = fPsi;
   }

  /**
     Set the components phi, theta, psi based on three Scalars.
   */
  void SetComponents(Scalar phi, Scalar theta, Scalar psi) {
    fPhi=phi; fTheta=theta; fPsi=psi; 
  }

  /**
     Get the components phi, theta, psi into three Scalars.
    */
  void GetComponents(Scalar & phi, Scalar & theta, Scalar & psi) const {
    phi=fPhi; theta=fTheta; psi=fPsi;
  }

  /**
      Return Phi Euler angle
  */
  Scalar Phi() const { return fPhi; }

  /**
      Return Theta Euler angle
  */
  Scalar Theta() const { return fTheta; }

  /**
      Return Psi Euler angle
  */
  Scalar Psi() const { return fPsi; }

  // =========== operations ==============

  /**
     Rotation operation on a cartesian vector
   */
  DisplacementVector3D< ROOT::Math::Cartesian3D<double> >
  operator() (const DisplacementVector3D< ROOT::Math::Cartesian3D<double> > & v) const;

  /**
     Rotation operation on a displacement vector in any coordinate system
   */
  template <class CoordSystem>
  DisplacementVector3D<CoordSystem>
  operator() (const DisplacementVector3D<CoordSystem> & v) const {
    DisplacementVector3D< Cartesian3D<double> > xyz(v);
    DisplacementVector3D< Cartesian3D<double> > Rxyz = operator()(xyz);
    return DisplacementVector3D<CoordSystem> ( Rxyz );
  }

  /**
     Rotation operation on a position vector in any coordinate system
   */
  template <class CoordSystem>
  PositionVector3D<CoordSystem>
  operator() (const PositionVector3D<CoordSystem> & v) const {
    DisplacementVector3D< Cartesian3D<double> > xyz(v);
    DisplacementVector3D< Cartesian3D<double> > Rxyz = operator()(xyz);
    return PositionVector3D<CoordSystem> ( Rxyz );
  }

  /**
     Rotation operation on a Lorentz vector in any 4D coordinate system
   */
  template <class CoordSystem>
  LorentzVector<CoordSystem>
  operator() (const LorentzVector<CoordSystem> & v) const {
    DisplacementVector3D< Cartesian3D<double> > xyz(v.Vec());
    xyz = operator()(xyz);
    LorentzVector< PxPyPzE4D<double> > xyzt (xyz.X(), xyz.Y(), xyz.Z(), v.E());
    return LorentzVector<CoordSystem> ( xyzt );
  }

  /**
     Rotation operation on an arbitrary vector v.
     Preconditions:  v must implement methods x(), y(), and z()
     and the arbitrary vector type must have a constructor taking (x,y,z)
   */
  template <class ForeignVector>
  ForeignVector
  operator() (const  ForeignVector & v) const {
    DisplacementVector3D< Cartesian3D<double> > xyz(v);
    DisplacementVector3D< Cartesian3D<double> > Rxyz = operator()(xyz);
    return ForeignVector ( Rxyz.X(), Rxyz.Y(), Rxyz.Z() );
  }

  /**
     Overload operator * for rotation on a vector
   */
  template <class AVector>
  inline
  AVector operator* (const AVector & v) const
  {
    return operator()(v);
  }

  /**
      Invert a rotation in place
   */
  void Invert() {Scalar tmp = -fPhi; fPhi = -fPsi; fTheta = -fTheta; fPsi=tmp;}

  /**
      Return inverse of a rotation
   */
  EulerAngles Inverse() const { return EulerAngles(-fPsi, -fTheta, -fPhi); }

  // ========= Multi-Rotation Operations ===============

  /**
     Multiply (combine) two rotations
   */
  EulerAngles operator * (const Rotation3D  & r) const;
  EulerAngles operator * (const AxisAngle   & a) const;
  EulerAngles operator * (const EulerAngles & e) const;
  EulerAngles operator * (const Quaternion  & q) const;
  EulerAngles operator * (const RotationX  & rx) const;
  EulerAngles operator * (const RotationY  & ry) const;
  EulerAngles operator * (const RotationZ  & rz) const;

  /**
     Post-Multiply (on right) by another rotation :  T = T*R
   */
  template <class R>
  EulerAngles & operator *= (const R & r) { return *this = (*this)*r; }

  /**
     Distance between two rotations
   */
  template <class R>
  Scalar Distance ( const R & r ) {return gv_detail::dist(*this,r);}

  /**
     Equality/inequality operators
   */
  bool operator == (const EulerAngles & rhs) {
    if( fPhi   != rhs.fPhi   ) return false;
    if( fTheta != rhs.fTheta ) return false;
    if( fPsi   != rhs.fPsi   ) return false;
    return true;
  }
  bool operator != (const EulerAngles & rhs) {
    return ! operator==(rhs);
  }

private:

  double fPhi;
  double fTheta;
  double fPsi;

  static double Pi() { return 3.14159265358979323; }

};  // EulerAngles

/**
   Distance between two rotations
 */
template <class R>
inline
typename EulerAngles::Scalar
Distance ( const EulerAngles& r1, const R & r2) {return gv_detail::dist(r1,r2);}

/**
   Multiplication of an axial rotation by an AxisAngle
 */
EulerAngles operator* (RotationX const & r1, EulerAngles const & r2);
EulerAngles operator* (RotationY const & r1, EulerAngles const & r2);
EulerAngles operator* (RotationZ const & r1, EulerAngles const & r2);

/**
   Stream Output and Input
 */
  // TODO - I/O should be put in the manipulator form 

std::ostream & operator<< (std::ostream & os, const EulerAngles & e);

} // namespace Math
} // namespace ROOT


#endif /* ROOT_Math_GenVector_EulerAngles  */

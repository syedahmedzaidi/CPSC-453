#include "a2.h"

// Return a matrix to represent a counterclockwise rotation of "angle"
// degrees around the axis "axis", where "axis" is one of the
// characters 'x', 'y', or 'z'.
Matrix4x4 rotation(double angle, char axis)
{
  Matrix4x4 rotate;
  // Fill me in!

  Vector4D r1,r2,r3,r4;

  double cosine = cos(angle);
  double sine = sin(angle);

  switch(axis) {
  	case 'x':
  		r1 = Vector4D(cosine, -sine, 0, 0);
  		r2 = Vector4D(sine, cosine, 0, 0);
  		r3 = Vector4D(0, 0, 1, 0);
  		r4 = Vector4D(0, 0, 0, 1);
  		break;
	case 'y':
  		r1 = Vector4D(1, 0, 0, 0);
  		r2 = Vector4D(0, cosine, -sine, 0);
  		r3 = Vector4D(0, sine, cosine, 0);
  		r4 = Vector4D(0, 0, 0, 1);
  		break;
	case 'z':
  		r1 = Vector4D(cosine, 0, sine, 0);
  		r2 = Vector4D(0, 1, 0, 0);
  		r3 = Vector4D(-sine, 0, cosine, 0);
  		r4 = Vector4D(0, 0, 0, 1);
  		break;
  }

  rotate = Matrix4x4(r1,r2,r3,r4);
  return rotate;
}

// Return a matrix to represent a displacement of the given vector.
Matrix4x4 translation(const Vector3D& displacement)
{
  Matrix4x4 translate;
  // Fill me in!
  double x = displacement[0];
  double y = displacement[1];
  double z = displacement[2];

  Vector4D r1(1, 0, 0, x);
  Vector4D r2(0, 1, 0, y);
  Vector4D r3(0, 0, 1, z);
  Vector4D r4(0, 0, 0, 1);

  translate= Matrix4x4(r1,r2,r3,r4);
  return translate;
}

// Return a matrix to represent a nonuniform scale with the given factors.
Matrix4x4 scaling(const Vector3D& scale)
{
  Matrix4x4 scal;
  // Fill me in!
  double x = scale[0];
  double y = scale[1];
  double z = scale[2];

  Vector4D r1(x, 0, 0, 0);
  Vector4D r2(0, y, 0, 0);
  Vector4D r3(0, 0, z, 0);
  Vector4D r4(0, 0, 0, 1);

  scal = Matrix4x4(r1,r2,r3,r4);
  return scal;
}

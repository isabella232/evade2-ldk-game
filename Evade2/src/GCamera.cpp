#include "GCamera.h"
//
//
//


TFloat GCamera::mX = 0;
TFloat GCamera::mY = 0;
TFloat GCamera::mZ = 0;

TFloat GCamera::mVX = 0;
TFloat GCamera::mVY = 0;
TFloat GCamera::mVZ = CAMERA_VZ;

//
//Camera::Camera() {
//  mX = mY = mZ = 0;
//  mVX = mVY = mVZ = 0;
//}
//
//public:
//    static TFloat mX, mY, mZ;
//    static TFloat mVX, mVY, mVZ;
//
//    static void Move() {
//      mX += mVX;
//      mY += mVY;
//      mZ += mVZ;
//    }
//
//};

//extern Camera *gCamera;

//TFloat Camera::mX = 0;
//TFloat Camera::mY = 0;
//TFloat Camera::mZ = 0;
//
//TFloat Camera::mVX = 0;
//TFloat Camera::mVY = 0;
//TFloat Camera::mVZ = 0;
//
//
//void Camera::Move() {
//  Camera::mX += Camera::mVX;
//  Camera::mY += Camera::mVY;
//  Camera::mZ += Camera::mVZ;
//}
////
//TBool Camera::CollidesWith(GVectorSprite *aVSprite) {
//  // If enemy bullet collides with player
//  if (abs(aVSprite->mZ - Camera::mZ) < abs(aVSprite->mVZ) && abs(aVSprite->mX - Camera::mX) < 64 && abs(aVSprite->mY - Camera::mY) < 64) {
//    return ETrue;
//  }
//  return EFalse;
//}
//
//extern Camera *gCamera;

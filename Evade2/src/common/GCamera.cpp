#include "GCamera.h"

GCamera *gCamera;

GCamera::GCamera() {
  x = 0;
  y = 0;
  z = 0;

  vx = 0;
  vy = 0;
  vz = CAMERA_VZ;
}



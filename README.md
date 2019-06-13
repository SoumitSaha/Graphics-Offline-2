# Graphics-Offline-2

Task:
1. Wheel (OpenGL.exe)

Wheel (2.exe)
w - move forward
s - move backward
a - rotate left
d - rotate right

Use arrow keys to move the camera.

Keep in mind that a full (360 degree) rotation of the wheel moves the wheel linearly by a length
equal to the perimeter of the wheel.
It is not required to strictly imitate the color patterns, but the shape of the wheel must be
discernible.
For Camera, use the 2nd out of the three gluLookAt statements in the demo code.
  gluLookAt(200*cos(cameraAngle), 200*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);

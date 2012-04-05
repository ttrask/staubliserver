#ifndef _P5JOYSTICK_H_
#define _P5JOYSTICK_H_


extern BOOL  wJoyState;
extern BOOL  wJoyMode;
extern BOOL  wJoyZeroOrient;

extern char  bJoyThumb;
extern char  bJoyIndex;
extern char  bJoyMiddle;
extern char  bJoyRing;
extern char  bJoyPinky;
extern float fJoy_x;
extern float fJoy_y;
extern float fJoy_z;
extern float fJoyLast_x;
extern float fJoyLast_y;
extern float fJoyLast_z;
extern float fJoyDeadband_x;
extern float fJoyDeadband_y;
extern float fJoyDeadband_z;


BOOL GetJoystickState();
void SetJoystickState(P5BOOL state, CP5DLL *handle);
void SetZeroBasedReset(BOOL isZeroBased);
void SetJoystickGesture(int thumb, int index, int middle, int ring, int pinky);
BOOL CheckJoystickGesture();
void SetJoystickDeadband(float xdeadband, float ydeadband, float zdeadband);
void ProcessJoystickEmulation();
void ProcessJoystickNoneZeroEmulation();
void ProcessJoyData();

#endif

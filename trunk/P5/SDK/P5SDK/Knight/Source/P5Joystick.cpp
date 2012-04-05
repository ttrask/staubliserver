#include "stdafx.h"
#include "p5dll.h"
#include "P5Joystick.h"
#include <math.h>
#include <stdio.h>

BOOL  wJoyState;
BOOL  wJoyMode;
BOOL  wJoyZeroOrient;

char  bJoyThumb;
char  bJoyIndex;
char  bJoyMiddle;
char  bJoyRing;
char  bJoyPinky;
float fJoy_x;
float fJoy_y;
float fJoy_z;
float fJoyLast_x;
float fJoyLast_y;
float fJoyLast_z;
float fJoyDeadband_x;
float fJoyDeadband_y;
float fJoyDeadband_z;
CP5DLL *P5Handle;

void SetJoystickState(P5BOOL state, CP5DLL *handle)
{
	wJoyState = state;
	P5Handle = handle;

	if(state)
	{
//		wJoyState	   = TRUE;
		wJoyZeroOrient = TRUE;
		wJoyMode       = TRUE;

		bJoyThumb      = NULL;
		bJoyIndex      = NULL;
		bJoyMiddle     = NULL;
		bJoyRing       = NULL;
		bJoyPinky      = NULL;
		fJoy_x         = 0.f;
		fJoy_y         = 0.f;
		fJoy_z         = 0.f;
		fJoyLast_x     = 0.f;
		fJoyLast_y     = 0.f;
		fJoyLast_z	   = 0.f;
		fJoyDeadband_x = 0.f;
		fJoyDeadband_y = 0.f;
		fJoyDeadband_z = 0.f;
	}
}


BOOL GetJoystickState()
{
	return wJoyState;
}


/************************************
// Glove x,y,z is reset to zero(TRUE/FALSE)?
************************************/
void SetZeroBasedReset(BOOL isZeroBased)
{
	wJoyZeroOrient = isZeroBased;
}


/************************************
// Sets fingers to be ignored and how much bending
// to be used. Ex. "...ture(-1,-1,-1,10,10);"
// - ring and pinky bended over 10 will set the
//   joystick mode temporary OFF
************************************/
void SetJoystickGesture(int thumb, int index, int middle, int ring, int pinky)
{
	bJoyThumb  = thumb;
	bJoyIndex  = index;
	bJoyMiddle = middle;
	bJoyRing   = ring;
	bJoyPinky  = pinky;
}


BOOL CheckJoystickGesture()
{
	//Check to see if joystick mode has been activated
	int state=0;

	if (bJoyThumb != -1)
	{
		if (P5Handle->m_byDistalSensor_Data[P5_THUMB] < bJoyThumb)
		{
			state += 1;
		}
	}
	else
		state += 1;

	if (bJoyIndex != -1)
	{
		if (P5Handle->m_byDistalSensor_Data[P5_INDEX] < bJoyIndex)
		{
			state += 1;
		}
	}
	else
		state += 1;

	if (bJoyMiddle != -1)
	{
		if (P5Handle->m_byDistalSensor_Data[P5_MIDDLE] < bJoyMiddle)
		{
			state += 1;
		}
	}
	else
		state += 1;

	if (bJoyRing != -1)
	{
		if (P5Handle->m_byDistalSensor_Data[P5_RING] < bJoyRing)
		{
			state += 1;
		}
	}
	else
		state += 1;

	if (bJoyPinky != -1)
	{
		if (P5Handle->m_byDistalSensor_Data[P5_PINKY] < bJoyPinky)
		{
			state += 1;
		}
	}
	else
		state += 1;

	if (state == 5)
		return TRUE;
	else
		return FALSE;
}


/************************************
// Sets x,y,z area to be dead (zero)
// Ex. "..band(10,10,10);" - makes this:
// --+-- while glove is inside of
// |   |  10,10,10 square - x,y,z values
// |   |  are going to be zero
// --+--   
************************************/
void SetJoystickDeadband(float xdeadband, float ydeadband, float zdeadband)
{
	fJoyDeadband_x = xdeadband;
	fJoyDeadband_y = ydeadband;
	fJoyDeadband_z = zdeadband;
}


void ProcessJoystickEmulation()
{
	if (wJoyState == TRUE)
	{
		if (wJoyMode == TRUE)
		{
			//Joystick mode activated

			if (CheckJoystickGesture() == FALSE)
			{
				//Joystick gesture has been deactivated
				wJoyMode = FALSE;
				fJoy_x = 0.0f;
				fJoy_y = 0.0f;
				fJoy_z = 0.0f;
				return;
			}

			if ( fabs(P5Handle->m_fx) > fJoyDeadband_x)
			{
				if (P5Handle->m_fx<0.0f)
					fJoy_x = P5Handle->m_fx+fJoyDeadband_x;
				else
					fJoy_x = P5Handle->m_fx-fJoyDeadband_x;
			}
			else
				fJoy_x = 0.0f;

			if ( fabs(P5Handle->m_fy) > fJoyDeadband_y)
			{
				if (P5Handle->m_fy<0.0f)
					fJoy_y = P5Handle->m_fy+fJoyDeadband_y;
				else
					fJoy_y = P5Handle->m_fy-fJoyDeadband_y;
			}
			else
				fJoy_y = 0.0f;
	
			if ( fabs(P5Handle->m_fz) > fJoyDeadband_z)
			{
				if (P5Handle->m_fz<0.0f)
					fJoy_z = P5Handle->m_fz+fJoyDeadband_z;
				else
					fJoy_z = P5Handle->m_fz-fJoyDeadband_z;
			}
			else
				fJoy_z = 0.0f;
		}
		else
		{
			//Check to see if joystick mode has been activated

			if (CheckJoystickGesture() == TRUE)
			{
				//Joystick gesture has been performed
				wJoyMode = TRUE;
				P5Handle->m_fx=0.0f;
				P5Handle->m_fy=0.0f;
				P5Handle->m_fz=0.0f;
			}
		}
	}
}


void ProcessJoystickNoneZeroEmulation()
{
	if (wJoyState == TRUE)
	{
		if (wJoyMode == TRUE)
		{
			//Joystick mode activated

			if (CheckJoystickGesture() == FALSE)
			{
				//Joystick gesture has been deactivated
				wJoyMode = FALSE;
				fJoy_x = fJoyLast_x;
				fJoy_y = fJoyLast_y;
				fJoy_z = fJoyLast_z;
				return;
			}

			if ( fabs(P5Handle->m_fx) > fJoyDeadband_x)
			{
				if (P5Handle->m_fx<0.0f)
					fJoy_x = P5Handle->m_fx+fJoyDeadband_x;
				else
					fJoy_x = P5Handle->m_fx-fJoyDeadband_x;
			}
			else
				fJoy_x = fJoyLast_x;

			if ( fabs(P5Handle->m_fy) > fJoyDeadband_y)
			{
				if (P5Handle->m_fy<0.0f)
					fJoy_y = P5Handle->m_fy+fJoyDeadband_y;
				else
					fJoy_y = P5Handle->m_fy-fJoyDeadband_y;
			}
			else
				fJoy_y = fJoyLast_y;
	
			if ( fabs(P5Handle->m_fz) > fJoyDeadband_z)
			{
				if (P5Handle->m_fz<0.0f)
					fJoy_z = P5Handle->m_fz+fJoyDeadband_z;
				else
					fJoy_z = P5Handle->m_fz-fJoyDeadband_z;
			}
			else
				fJoy_z = fJoyLast_z;

			fJoyLast_x = fJoy_x;
			fJoyLast_y = fJoy_y;
			fJoyLast_z = fJoy_z;
		}
		else
		{
			//Check to see if joystick mode has been activated

			if (CheckJoystickGesture() == TRUE)
			{
				//Joystick gesture has been performed
				wJoyMode = TRUE;
				P5Handle->m_fx=fJoyLast_x;
				P5Handle->m_fy=fJoyLast_y;
				P5Handle->m_fz=fJoyLast_z;
			}
		}
	}
}

/**/
void ProcessJoyData()
{
	if(wJoyState)
	{
		if(wJoyZeroOrient)
		{
			ProcessJoystickEmulation();
		}
		else
		{
			ProcessJoystickNoneZeroEmulation();
		}
	}
}
/**/
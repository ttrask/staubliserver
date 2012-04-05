#include "Glove.h"
#include "P5Motion.h"

CP5DLL P5Device;
BOOL P5Present=false;

extern float hand_yaw;
extern BOOL g_bMode;

float zeroyaw=0.0f, zeropitch=0.0f, zeroroll=0.0f;

HRESULT ReadGloveValues(float &x, float &y, float &z, 
	float &yaw, float &pitch, float &roll, 
	float &thumb, float &index, float &middle, float &ring, float &pinky)
{
	// Av: copy glove output values into these locations
	//
	// x = handx;
	// y = handy;
	// etc.
	//
	// these raw values can be scaled and biased by editing the hand.ini file
	// Note:	scale and bias are applied to raw input value
	//				min and max are enforced after scale and bias are applied

	//
	// test code: delete me!
	//

	if (P5Present==false)
	{
		static float testInput = 0.0f;
		static float delta = 0.01f;

		// test curling the index finger
		index = testInput;
//		x=testInput;
//		roll = hand_yaw;

		//roll = 0.25f;
		roll = hand_yaw;


		testInput += delta;
		if( ( testInput > 1.0f ) || ( testInput < 0.0f ) )
			delta = -delta;

		thumb  = 0.0f;
		index  = 0.0f;
		middle = 0.0f;
		ring   = 0.0f;
		pinky  = 0.0f;

	}
	else
	{
		P5Motion_Process();

		thumb=(float)(P5Device.m_P5Devices[0].m_byBendSensor_Data[0])/63.0f;
		index=(float)(P5Device.m_P5Devices[0].m_byBendSensor_Data[1])/63.0f;
		middle=(float)(P5Device.m_P5Devices[0].m_byBendSensor_Data[2])/63.0f;
		ring=(float)(P5Device.m_P5Devices[0].m_byBendSensor_Data[3])/63.0f;
		pinky=(float)(P5Device.m_P5Devices[0].m_byBendSensor_Data[4])/63.0f;

		x = (-nXPos / 256.f) - 1.f;
		y = -nYPos / 256.f;
		z = -nZPos / 1024.f;

		if(x>1.0f) x=1.f;
		if(x<-1.0f) x=-1.f;
		
		if(y>1.0f) y=1.f;
		if(y<-1.0f) y=-1.f;
		
		if(z>1.0f) z=1.f;
		if(z<-1.0f) z=-1.f;
	}

	return S_OK;
}

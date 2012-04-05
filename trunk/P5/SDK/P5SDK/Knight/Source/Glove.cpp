#include "Glove.h"
#include "p5dll.h"
#include "P5Motion.h"


CP5DLL P5Device;
BOOL P5Present=false;
float preset_yaw=0.0; 

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
		x=testInput;
		yaw=0.5;

		testInput += delta;
		if( ( testInput > 1.0f ) || ( testInput < 0.0f ) )
			delta = -delta;
	}
	else
	{
		static float testInput = 0.0f;
		testInput+=0.01f;
		if( ( testInput > 1.0f ) )
			testInput=-1.0f;

//		z=testInput;

		thumb=(float)(P5Device.m_P5Devices[0].m_byBendSensor_Data[P5_THUMB])/63.0f;
		index=(float)(P5Device.m_P5Devices[0].m_byBendSensor_Data[P5_INDEX])/63.0f;
		middle=(float)(P5Device.m_P5Devices[0].m_byBendSensor_Data[P5_MIDDLE])/63.0f;
		ring=(float)(P5Device.m_P5Devices[0].m_byBendSensor_Data[P5_RING])/63.0f;
		pinky=(float)(P5Device.m_P5Devices[0].m_byBendSensor_Data[P5_PINKY])/63.0f;

		x=(float)(nXPos / 256.f);
		x=x/20.0f;

		y=(float)(nYPos / 256.f);
		y=y/10.0f;
		y=0.0f;

		z=(float)(nYPos / 256.f);
		z=z / -10.0f;

		if(x>1.0f) x=1.f;
		if(x<-1.0f) x=-1.f;
		
		if(y>1.0f) y=1.f;
		if(y<-1.0f) y=-1.f;
		
		if(z>1.0f) z=1.f;
		if(z<-1.0f) z=-1.f;

		yaw=0.0f;
		pitch=preset_yaw;
		roll=0.0f;
	}

	return S_OK;
}

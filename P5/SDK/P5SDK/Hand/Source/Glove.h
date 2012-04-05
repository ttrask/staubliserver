#include "C3DFont.h"
#include "KeyboardControl.h"
#include "C3DFrameObject.h"
#include "p5dll.h"

#define UI_FONT_NAME _T("Century Gothic")
#define UI_FONT_SIZE 12
#define UI_FONT_STYLE 0	// C3DFONT_BOLD

#define UI_HAND_RED 255
#define UI_HAND_GREEN 255
#define UI_HAND_BLUE 0
#define UI_HAND_ALPHA 255

#define UI_FIRST_LINE_Y 20.0f
#define UI_OFFSET_X 8.0f
#define UI_GROUP_OFFSET_Y 10.0f

extern CP5DLL P5Device;
extern BOOL P5Present;

HRESULT ReadGloveValues(float &x, float &y, float &z, 
	float &yaw, float &pitch, float &roll, 
	float &thumb, float &index, float &middle, float &ring, float &pinky);
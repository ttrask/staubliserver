#ifndef KeyboardControl_H
#define KeyboardControl_H

class KeyboardControl;

class KeyValue
{
public:
	KeyValue(KeyboardControl *pKeyCtrl, char *groupName);
	~KeyValue();
	float GetMinValue() {return minValue;}
	float GetMaxValue() {return maxValue;}
	float GetValue() {return value;}
	float GetRawValue() {return rawValue;}
	float *GetValuePtr()	{return &value;}
	float GetDelta() {return delta;}
	float GetScale()	{return scale;}
	float GetBias()	{return bias;}
	unsigned int GetKeyIncrease() {return keyIncrease;}
	unsigned int GetKeyDecrease() {return keyDecrease;}
	char *GetName() {return nameValue;}
	void SetValue(float fValue);
	void SetRawValue( float fRawValue );

	void Increment();
	void Decrement();

private:
	// disable default constructor
	KeyValue();
	
	float minValue;
	float maxValue;
	float value;
	float rawValue;
	float delta;
	float scale;
	float bias;
	unsigned int keyIncrease;
	unsigned int keyDecrease;
	char *nameValue;
};

typedef KeyValue* PKeyValue;

class KeyboardObject
{
public:
	KeyboardObject(KeyboardControl *pKeyCtrl, char *groupName);
	~KeyboardObject();

	int GetNumValues();
	KeyValue *GetKeyValue(int index);
	const char *GetName()	{ return nameObject; }
	const char *GetAnimation()	{ return animationName; }
	const char *GetType()	{ return controlType; }
	const char *GetDisplayText()	{ return displayText; }
	const char *GetValueName(int index);
	float *GetValuePtr(int index);
	int GetValueIndex( const char *szName );

private:
	// disable default constructor
	KeyboardObject();

	char *nameObject;
	char *controlType;
	char *animationName;
	char *displayText;
	int numValues;
	KeyValue **keyValues;
};
typedef KeyboardObject* PKeyboardObject;

class KeyboardControl
{
	// letting the controller peek at the values avoids linear search to look up a value
	friend class C3DController6DOF;
public:
	KeyboardControl(const char *iniName, const char *baseGroupName);
	~KeyboardControl();
	int GetNumObjects()	{ return numObjects; }
	KeyboardObject *GetObjectPtr( int i );

	float GetFloat(const char *group, const char *entry, float defaultValue);
	const char *GetString(const char *group, const char *entry, const char *defaultValue);
	int GetInt(const char *group, const char *entry, int defaultValue);

	float GetValue(const char *name);
	float GetRawValue(const char *name);
	float *GetValuePtr(const char *name);
	float GetMinValue(const char *name);
	float GetMaxValue(const char *name);
	void SetValue(const char *name, float value);
	void SetRawValue( const char *name, float rawValue );

	bool HandleKey(unsigned int keyCode);

private:
	KeyValue *GetKeyValue(const char *name);
	int numObjects;
	KeyboardObject **keyObjects;
	char *nameOfIniFile;
};

#endif
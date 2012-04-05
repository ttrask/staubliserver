
#include "KeyboardControl.h"
#include <windows.h>
#include <stdio.h>

//////////////////////////////////////////
// Class KeyValue implementation
//
KeyValue::KeyValue(KeyboardControl *pKeyCtrl, char *groupName) :
	nameValue( NULL )
{
	minValue = pKeyCtrl->GetFloat(groupName,"Min",0.0f);
	maxValue = pKeyCtrl->GetFloat(groupName,"Max",1.0f);
	delta = pKeyCtrl->GetFloat(groupName,"Delta",0.1f);
	rawValue = pKeyCtrl->GetFloat(groupName,"Init",0.0f);
	scale = pKeyCtrl->GetFloat(groupName,"Scale",1.0f);
	if( scale == 0.0f )
		scale = 1.0f;	// prevent divzero
	bias = pKeyCtrl->GetFloat(groupName,"Bias",0.0f);
	minRawValue = ( minValue - bias ) / scale;
	maxRawValue = ( maxValue - bias ) / scale;

	SetRawValue( rawValue );
	char buffer[256];
	strcpy(buffer,pKeyCtrl->GetString(groupName,"KeyIncrease","1"));
	keyIncrease = buffer[0];
	strcpy(buffer,pKeyCtrl->GetString(groupName,"KeyDecrease","Q"));
	keyDecrease = buffer[0];
	nameValue = new char[strlen(groupName)+1];
	strcpy(nameValue,groupName);
}

KeyValue::~KeyValue()
{
	delete []nameValue;
}

void KeyValue::Increment()
{
	rawValue += delta;
	SetRawValue( rawValue );
}

void KeyValue::Decrement()
{
	rawValue -= delta;
	SetRawValue( rawValue );
}

void KeyValue::SetValue(float fValue)
{
	value = fValue;
	if (value < minValue)
	{
		value = minValue;
	}
	else if (value > maxValue)
	{
		value = maxValue;
	}
}

void KeyValue::SetRawValue(float fRawValue)
{
	rawValue = fRawValue;
	if( rawValue < minRawValue )
		rawValue = minRawValue;
	else if( rawValue > maxRawValue )
		rawValue = maxRawValue;
	
	value = rawValue * scale + bias;
	if (value < minValue)
	{
		value = minValue;
	}
	else if (value > maxValue)
	{
		value = maxValue;
	}
}


//////////////////////////////////////////////////////

KeyboardObject::KeyboardObject(KeyboardControl *pKeyCtrl, char *groupName) :
	nameObject( NULL ),
	controlType( NULL ),
	animationName( NULL ),
	displayText( NULL ),
	numValues( 0 ),
	keyValues( NULL )
{
	numValues = pKeyCtrl->GetInt(groupName,"NumVar",0);
	keyValues = new PKeyValue[numValues];
	nameObject = new char[strlen(groupName)+1];
	strcpy(nameObject,groupName);
	char buffer[256];
	strcpy(buffer,pKeyCtrl->GetString(groupName,"Type",""));
	controlType = new char[strlen(buffer)+1];
	strcpy(controlType,buffer);
	strcpy(buffer,pKeyCtrl->GetString(groupName,"Animation",""));
	animationName = new char[strlen(buffer)+1];
	strcpy(animationName,buffer);
	strcpy(buffer,pKeyCtrl->GetString(groupName,"Display",""));
	displayText = new char[strlen(buffer)+1];
	strcpy(displayText,buffer);
	for(int i = 0; i < numValues; i++)
	{
		char buffer[256];
		wsprintf(buffer,"Var%i",i+1);
		char keyName[256];
		strcpy(keyName,pKeyCtrl->GetString(groupName,buffer,""));
		keyValues[i] = new KeyValue(pKeyCtrl,keyName);
	}
}

KeyboardObject::~KeyboardObject()
{
	for(int i=0; i < numValues; i++)
	{
		delete keyValues[i];
	}
	delete []keyValues;
	delete []nameObject;
	delete []displayText;
}

int KeyboardObject::GetNumValues()
{
	return numValues;
}

KeyValue *KeyboardObject::GetKeyValue(int index)
{
	return keyValues[index];
}

const char *KeyboardObject::GetValueName(int index)
{
	return keyValues[index]->GetName();
}

float *KeyboardObject::GetValuePtr(int index)
{
	return keyValues[index]->GetValuePtr();
}

int KeyboardObject::GetValueIndex( const char *szName )
{
	for(int i = 0; i < numValues; i++)
	{
		if( 0 == stricmp( szName, keyValues[i]->GetName() ) )
			return i;
	}
	return -1;
}

KeyboardControl::KeyboardControl(const char *iniName,const char *baseGroupName)
{
	nameOfIniFile = new TCHAR[strlen(iniName)+1];
	strcpy(nameOfIniFile,iniName);
	numObjects = GetInt(baseGroupName,"NumParts", 0);
	if( numObjects )
	{
		keyObjects = new PKeyboardObject[numObjects];
		for(int i=0; i < numObjects; i++)
		{
			char buffer[256];
			wsprintf(buffer,"Part%i",i+1);
			char keyName[256];
			strcpy(keyName,GetString(baseGroupName,buffer,""));
			keyObjects[i] = new KeyboardObject(this,keyName);
		}
	}
	else
		keyObjects = NULL;
}

KeyboardControl::~KeyboardControl()
{
}

KeyboardObject *KeyboardControl::GetObjectPtr( int i )
{
	if( i >= 0 && i < numObjects )
		return keyObjects[i];
	else
		return NULL;
}

float KeyboardControl::GetFloat(const char *group, const char *entry, float defaultValue)
{
	char buffer[256];
	char defFloat[256];
	sprintf(defFloat,"%f",defaultValue);
	strcpy(buffer,GetString(group,entry,defFloat));
	return (float)atof(buffer);
}

const char *KeyboardControl::GetString(const char *group, const char *entry, const char *defaultValue)
{
	static char buffer[256];
	GetPrivateProfileString(group,entry,defaultValue,buffer,sizeof(buffer),nameOfIniFile);
	return buffer;
}

int KeyboardControl::GetInt(const char *group, const char *entry, int defaultValue)
{
	int value = GetPrivateProfileInt(group,entry,defaultValue,nameOfIniFile);
	return value;
}

float KeyboardControl::GetValue(const char *name)
{
	KeyValue *key = GetKeyValue(name);
	if (key)
		return key->GetValue();
	else
		return 0.0f;
}

float KeyboardControl::GetRawValue(const char *name)
{
	KeyValue *key = GetKeyValue(name);
	if (key)
		return key->GetRawValue();
	else
		return 0.0f;
}

void KeyboardControl::SetValue(const char *name, float value)
{
	KeyValue *key = GetKeyValue(name);
	if (key)
		key->SetValue(value);
}

void KeyboardControl::SetRawValue(const char *name, float rawValue)
{
	KeyValue *key = GetKeyValue(name);
	if (key)
		key->SetRawValue(rawValue);
}

float *KeyboardControl::GetValuePtr(const char *name)
{
	KeyValue *key = GetKeyValue(name);
	if (key)
		return key->GetValuePtr();
	else
		return NULL;
}

float KeyboardControl::GetMinValue(const char *name)
{
	KeyValue *key = GetKeyValue(name);
	if (key)
		return key->GetMinValue();
	else
		return 0.0f;
}

float KeyboardControl::GetMaxValue(const char *name)
{
	KeyValue *key = GetKeyValue(name);
	if (key)
		return key->GetMaxValue();
	else
		return 0.0f;
}

KeyValue *KeyboardControl::GetKeyValue(const char *name)
{
	for (int o = 0; o < numObjects; o++)
	{
		KeyboardObject *obj = keyObjects[o];
		if (obj)
		{
			int numVars = obj->GetNumValues();
			for (int n = 0; n < numVars; n++)
			{
				KeyValue *key = obj->GetKeyValue(n);
				if (0 == strcmpi(key->GetName(),name))
				{
					return key;
				}
			}
		}
	}
	return NULL;
}

bool KeyboardControl::HandleKey(unsigned int keyCode)
{
	bool retVal = false;
	for (int o = 0; o < numObjects; o++)
	{
		KeyboardObject *obj = keyObjects[o];
		if (obj)
		{
			int numVars = obj->GetNumValues();
			for (int n = 0; n < numVars; n++)
			{
				KeyValue *key = obj->GetKeyValue(n);
				if (keyCode == key->GetKeyDecrease())
				{
					key->Decrement();
					retVal = true;
				}
				else if (keyCode == key->GetKeyIncrease())
				{
					key->Increment();
					retVal = true;
				}
			}
		}
	}
	return retVal;
}

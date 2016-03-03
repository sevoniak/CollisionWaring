#include "InputClass.h"


InputClass::InputClass(void)
{
	resetInput();
}

InputClass::~InputClass(void)
{
}

void InputClass::resetInput()
{
	for (int i = 0; i < 256; i++)
	{
		m_keys[i] = false;
		m_spec[i] = false;
	}
}

void InputClass::keyDown(unsigned int input) 
{ 
	if (input < OFFSET)
		m_keys[input] = true; 
	else
		m_spec[input-OFFSET] = true;
}

void InputClass::keyUp(unsigned int input) 
{ 
	if (input < OFFSET)
		m_keys[input] = false; 
	else
		m_spec[input-OFFSET] = false;
}

bool InputClass::isKeyDown(unsigned int key) 
{ 
	if (key < OFFSET)
		return m_keys[key]; 
	else
		return m_spec[key-OFFSET];
}
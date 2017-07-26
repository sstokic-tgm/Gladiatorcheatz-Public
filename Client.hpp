#pragma once

#include <Windows.h>
#include "PrimitiTabella.hpp"

class Client
{
public:

	char *getName();
	PrimitiTabella *getPrimitiTabella();
	Client *getNext();
	int getClsId();
};
#pragma once

#include "Client.hpp"

class BaseClientDll
{
public:

	Client *getAll();
};

extern BaseClientDll *p_Client;
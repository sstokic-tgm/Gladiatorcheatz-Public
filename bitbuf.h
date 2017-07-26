#ifndef BITBUF_H
#define BITBUF_H

#ifdef _WIN32
#pragma once
#endif

#include "harCs.hpp"

// (c) Beakers @ uc

class bf_read
{
public:
	
	unsigned int base_address;
	unsigned int cur_offset;

	bf_read(unsigned int addr);
	void setOffset(unsigned int offset);
	void skip(unsigned int length);
	int readByte();
	bool readBool();
	std::string readString();
};

#endif
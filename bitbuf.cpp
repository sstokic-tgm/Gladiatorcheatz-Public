#include "bitbuf.h"

bf_read::bf_read(unsigned int addr)
{
	base_address = addr;
	cur_offset = 0;
}

void bf_read::setOffset(unsigned int offset)
{
	cur_offset = offset;
}

void bf_read::skip(unsigned int length)
{
	cur_offset += length;
}

int bf_read::readByte()
{
	auto val = *reinterpret_cast<char*>(base_address + cur_offset);
	++cur_offset;
	return val;
}

bool bf_read::readBool()
{
	auto val = *reinterpret_cast<bool*>(base_address + cur_offset);
	++cur_offset;
	return val;
}

std::string bf_read::readString()
{
	char buffer[256];

	auto str_length = *reinterpret_cast<char*>(base_address + cur_offset);
	++cur_offset;

	std::memcpy(buffer, reinterpret_cast<void*>(base_address + cur_offset), str_length > 255 ? 255 : str_length);

	buffer[str_length > 255 ? 255 : str_length] = '\0';
	cur_offset += str_length + 1;

	return std::string(buffer);
}
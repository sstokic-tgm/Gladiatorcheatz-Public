#include "VT.hpp"
#include "BaseClientDll.hpp"

Client *BaseClientDll::getAll()
{
	typedef Client*(__thiscall *o_getAll)();
	return VT::vfunc<o_getAll>(this, 8)();
}
#pragma once

#include <unordered_map>

#include "XorStr.hpp"

#include "BaseClientDll.hpp"

class DVariant
{
public:

	union
	{
		float m_Float;
		long m_Int;
		char *m_pString;
		void *m_pData;
		float m_Vector[3];
	};
};

class PrimitiProxyData
{
public:

	const PrimitiImovina *m_pRecvProp;
	DVariant m_Value;
	int m_iElement;
	int m_ObjectID;
};

typedef void(*PrimitiVarProxyFn)(const PrimitiProxyData *pData, void *pStruct, void *pOut);

namespace NetMngr
{
	class Mngr
	{
	public:

		~Mngr() { this->tabelle.clear(); }

		void init();
		int getOffs(const char *tabellaIme, const char *imovIme);
		bool hookProp(const char *tabellaIme, const char *imovIme, PrimitiVarProxyFn func);
		bool hookProp(const char *tabellaIme, const char *imovIme, PrimitiVarProxyFn func, PrimitiVarProxyFn &orig);
		void dump();

	private:

		std::unordered_map<std::string, PrimitiTabella*> tabelle;

		int getImov(const char *tabellaIme, const char *imovIme, PrimitiImovina **imov = 0);
		int getImov(PrimitiTabella *primitiTabella, const char *imovIme, PrimitiImovina **imov = 0);
		PrimitiTabella *getTabella(const char *tabellaIme);
		void dumpTabella(PrimitiTabella *tabella, std::string tabs);
		std::string type2str(Type t);
	};
}

extern NetMngr::Mngr *netMngr;

//extern void __cdecl print(const char * format, ...);
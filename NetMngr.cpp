#include "NetMngr.hpp"

namespace NetMngr
{
	void Mngr::init()
	{
		tabelle.clear();

		Client *client = p_Client->getAll();
		if (!client)
			return;

		while (client)
		{
			auto primitiTabella = client->getPrimitiTabella();

			if (primitiTabella)
				tabelle.emplace(std::string(client->getName()), primitiTabella);

			client = client->getNext();
		}
	}

	int Mngr::getOffs(const char *tabellaIme, const char *imovIme)
	{
		int offs = this->getImov(tabellaIme, imovIme);

		if (!offs)
			return 0;

		return offs;
	}

	bool Mngr::hookProp(const char *tabellaIme, const char *imovIme, PrimitiVarProxyFn func)
	{
		PrimitiImovina *imov = nullptr;

		this->getImov(tabellaIme, imovIme, &imov);

		if (!imov)
			return false;

		imov->m_ProxyFn = func;

		return true;
	}

	bool Mngr::hookProp(const char *tabellaIme, const char *imovIme, PrimitiVarProxyFn func, PrimitiVarProxyFn &orig)
	{
		PrimitiImovina *imov = nullptr;

		this->getImov(tabellaIme, imovIme, &imov);

		if (!imov)
			return false;

		orig = reinterpret_cast<PrimitiVarProxyFn>(imov->m_ProxyFn);

		imov->m_ProxyFn = func;

		return true;
	}

	int Mngr::getImov(const char *tabellaIme, const char *imovIme, PrimitiImovina **imov)
	{
		PrimitiTabella *primitiTabella = this->getTabella(tabellaIme);

		if (!primitiTabella)
			return 0;

		int offs = this->getImov(primitiTabella, imovIme, imov);

		if (!offs)
			return 0;

		return offs;
	}

	int Mngr::getImov(PrimitiTabella *primitiTabella, const char *imovIme, PrimitiImovina **imov)
	{
		int extrOffs = 0;

		for (int i = 0; i < primitiTabella->m_nProps; i++)
		{
			auto *primitiImovina = &primitiTabella->m_pProps[i];
			auto primitiDete = primitiImovina->m_pDataTable;

			if (primitiDete && (primitiDete->m_nProps > 0))
			{
				int tmp = this->getImov(primitiDete, imovIme, imov);

				if (tmp)
					extrOffs += (primitiImovina->m_Offset + tmp);
			}

			if (strcmp(primitiImovina->m_pVarName, imovIme))
				continue;

			if (imov)
				*imov = primitiImovina;

			return (primitiImovina->m_Offset + extrOffs);
		}
		return extrOffs;
	}

	PrimitiTabella *Mngr::getTabella(const char *tabellaIme)
	{
		if (tabelle.empty())
			return 0;

		for (auto tabella : tabelle)
		{
			if (strcmp(tabella.first.c_str(), tabellaIme) == 0)
				return tabella.second;
		}

		return 0;
	}

	void Mngr::dump()
	{
		for (Client *client = p_Client->getAll(); client != NULL; client = client->getNext())
		{
			auto tabella = client->getPrimitiTabella();
			//print("%s", client->getName());
			this->dumpTabella(tabella, "\t");
		}
	}

	void Mngr::dumpTabella(PrimitiTabella *tabella, std::string tabs)
	{
		for (int i = 0; i < tabella->m_nProps/* tabella->numImov() */; i++)
		{
			auto primitiImovina = &tabella->m_pProps[i]/* tabella->getImov(i) */;

			if (primitiImovina->m_RecvType/*primitiImovina->getType()*/ == Type::DPT_Array)
				continue;

			//print("%s\tName: %s Offset: 0x%X Type: %s", tabs.c_str(), primitiImovina->m_pVarName, primitiImovina->m_Offset, this->type2str(primitiImovina->m_RecvType).c_str());

			if (primitiImovina->m_RecvType/*primitiImovina->getType()*/ == Type::DPT_DataTable)
			{
				this->dumpTabella(primitiImovina->m_pDataTable/*primitiImovina->getTabella()*/, tabs + "\t");
			}
		}
	}

	std::string Mngr::type2str(Type t)
	{
		switch (t)
		{
		case DPT_Int:
			return XorStr("Int");
		case DPT_Float:
			return XorStr("Float");
		case DPT_Vector:
			return XorStr("Vector");
		case DPT_VectorXY:
			return XorStr("VectorXY");
		case DPT_String:
			return XorStr("String");
		case DPT_Array:
			return XorStr("Array");
		case DPT_DataTable:
			return XorStr("DataTable");
		case DPT_Int64:
			return XorStr("Int64");
		case DPT_NUMSendPropTypes:
			return XorStr("NUMSendPropTypes");
		default:
			return XorStr("Unknown Type");
		}
	}
}

//void __cdecl print(const char * format, ...)
//{
//	va_list argptr;
//
//	char szBuff[1024];
//
//	va_start(argptr, format);
//	wvsprintf(szBuff, format, argptr);
//	va_end(argptr);
//
//	strcat_s(szBuff, "\n");
//
//	char fileToWrite[MAX_PATH] = { 0 };
//
//	char buf[MAX_PATH];
//	GetTempPath(MAX_PATH, buf);
//
//	strcpy_s(fileToWrite, buf);
//	strcat_s(fileToWrite, XorStr("harCs.log"));
//
//	HANDLE hFile = CreateFile(fileToWrite, GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
//
//	SetFilePointer(hFile, 0, 0, FILE_END);
//
//	DWORD dummy = 0;
//	WriteFile(hFile, szBuff, (DWORD)strlen(szBuff), &dummy, NULL);
//	CloseHandle(hFile);
//}
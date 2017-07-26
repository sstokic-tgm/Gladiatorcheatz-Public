#pragma once

#include <Windows.h>

class PrimitiTabella;

typedef enum
{
	DPT_Int = 0,
	DPT_Float,
	DPT_Vector,
	DPT_VectorXY,
	DPT_String,
	DPT_Array,
	DPT_DataTable,
	DPT_Int64,
	DPT_NUMSendPropTypes
} Type;

class PrimitiImovina
{
public:

	char *m_pVarName;
	Type m_RecvType;
	int m_Flags;
	int m_StringBufferSize;
	bool m_bInsideArray;
	const void *m_pExtraData;
	PrimitiImovina *m_pArrayProp;
	void *m_ArrayLengthProxy;
	void *m_ProxyFn;
	void *m_DataTableProxyFn;
	PrimitiTabella *m_pDataTable;
	int m_Offset;
	int m_ElementStride;
	int m_nElements;
	const char *m_pParentArrayPropName;
};
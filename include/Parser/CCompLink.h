//#pragma once
#if !defined(_CCompLink_H_)
#define _CCompLink_H_

#include <cstdlib>
#include <cstdio>
#include <math.h>
#include "..\CDevString.h"
#include "CCompiler.h"
#include <vector>
namespace CSageScript{

class CCompiler;
class CCompLink
{
public:
struct Value_t
	{
		enum class Type
		{
			Num,
			String,
		};
		CDevString csName;
		CDevString csValue;
		Type type;
		int iValue;
	};
private:
	int			m_iNumValues = 0;
	

	std::vector<Value_t> m_vValues;
	CCompiler * m_cCompiler = nullptr;
	struct stExport_t
	{
	
		union val_t
		{
			int iReturn;
			char * sReturn;
		};

		val_t val;
		char sName[100];
	};
	stExport_t stExport; 


public:
	CCompLink();
	~CCompLink();
	CSageScript::CCompiler & GetCompiler();
	void SetExportedValue(char * sName,int iValue);
	void SetExportedValue(char * sName,char * sValue);
	bool GetIntVar(const char * sVar,int & iValue);
	bool GetStrVar(const char * sVar,char * & sValue);
	const char * GetStrVar(const char * sVar);
	int GetIntVar(const char * sVar)
	{
		int iValue = MININT;
		GetIntVar(sVar,iValue);
		return iValue;
	}
	void AddVar(const char * sVar,int iValue);
	void AddValue(const char * sName,const char * sValue);
	void AddValue(const char * sName,int iValue);
	bool Run(const char * sCode);
	Value_t * FindValue(const char * sName);
	void ClearValues()
	{
		m_iNumValues = 0;
 		m_vValues.clear();
	}
};
}; // CSageScript
#endif // _CCompLink_H_

#pragma once

#include "MorWin.h"

class GDIPlusManager
{
public:
	GDIPlusManager();
	~GDIPlusManager();
private:
	static int RefCount;
	static ULONG_PTR token;
};
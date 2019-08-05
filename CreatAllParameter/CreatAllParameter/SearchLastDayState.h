#pragma once
#ifndef SEARCHLASTDAYSTATE_H
#define SEARCHLASTDAYSTATE_H

#include "number/NumberBase.h"
#include "number/NumberInter.h"
class SearchLastDayState
{
public:
	SearchLastDayState();
	~SearchLastDayState();
	bool Inter(CIndicatorsInterface& daynumber, CIndicatorsInterface& shnumber);
};
#endif


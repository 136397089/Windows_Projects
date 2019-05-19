#pragma once
#ifndef PYTHONINTERFACE_H
#define PYTHONINTERFACE_H
enum SuccessEnum { FAILURE, SUCCESS };

class CPythonInterface
{
public:
	CPythonInterface();
	~CPythonInterface();

	SuccessEnum Inition();

	void Inition2();
};

#endif

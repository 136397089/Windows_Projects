#include "stdafx.h"
#include "DFT.h"


CDFT::CDFT()
{
}


CDFT::~CDFT()
{
}



DFTDataList CDFT::DFT(DFTDataList& theData)
{
	// Define the Size of the read in vector
	const int S = theData.size();

	// Initalise new vector with size of S
	DFTDataList out(S, 0);
	for (unsigned i = 0; (i < S); i++)
	{
		out[i] = complex<DFTBaseData>(0.0, 0.0);
		for (unsigned j = 0; (j < S); j++)
		{
			out[i] += theData[j] * polar<DFTBaseData>(1.0, -2 * PI * i * j / S);
		}
	}

	return out;
}

DFTDataList CDFT::DFT(const VStockData& theData)
{
	DFTDataList numbers;

	for (int i = 0; i < theData.size(); i++)
	{
		numbers.push_back(theData[i]);
	}

	return DFT(numbers);
}

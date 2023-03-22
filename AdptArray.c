#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "AdptArray.h"

typedef struct AdptArray_
{
	int ArrSize;
	PElement *pElemArr;
	DEL_FUNC delFunc;
	COPY_FUNC copyFunc;
	PRINT_FUNC printFunc;
} AdptArray, *PAdptArray;

PAdptArray CreateAdptArray(COPY_FUNC copyFunc_, DEL_FUNC delFunc_, PRINT_FUNC printFunc)
{
	PAdptArray pArr = (PAdptArray)malloc(sizeof(AdptArray));
	if (pArr == NULL)
		return NULL;
	pArr->ArrSize = 1;
	pArr->delFunc = delFunc_;
	pArr->copyFunc = copyFunc_;
	pArr->printFunc = printFunc;
	pArr->pElemArr = (PElement *)malloc(sizeof(PElement));
	if (!pArr->pElemArr)
	{
		free(pArr);
		return NULL;
	}
	for (int i = 0; i < pArr->ArrSize; i++)
	{
		pArr->pElemArr[i] = NULL;
	}
	return pArr;
}

void DeleteAdptArray(PAdptArray pArr)
{
	int i;
	if (pArr == NULL)
		return;

	for (i = 0; i < pArr->ArrSize; ++i)
	{
		if (pArr->pElemArr[i] != NULL)
		{
			pArr->delFunc((pArr->pElemArr)[i]);
		}
	}
	free(pArr->pElemArr);
	free(pArr);
}

Result SetAdptArrayAt(PAdptArray pArr, int idx, PElement pNewElem)
{
	PElement *newpElemArr;
	if (pArr == NULL || pNewElem == NULL || idx < 0)
		return FAIL;

	if (idx > pArr->ArrSize)
	{
		// Extend Array
		if ((newpElemArr = (PElement *)calloc((idx + 1), sizeof(PElement))) == NULL)
			return FAIL;
		memcpy(newpElemArr, pArr->pElemArr, (pArr->ArrSize) * sizeof(PElement));
		free(pArr->pElemArr);
		pArr->pElemArr = newpElemArr;
		if (pArr->pElemArr[idx] != NULL)
		{
			pArr->delFunc((pArr->pElemArr)[idx]);
		}

		(pArr->pElemArr)[idx] = pArr->copyFunc(pNewElem);

		// Update Array Size
		pArr->ArrSize = (idx > pArr->ArrSize) ? (idx + 1) : pArr->ArrSize;
		return SUCCESS;
	}
}

PElement GetAdptArrayAt(PAdptArray pArr, int idex)
{
	if (pArr == NULL || idex < 0)
		return NULL;
	if (pArr->ArrSize <= idex)
		return NULL;
	if (pArr->pElemArr[idex] == NULL)
		return NULL;
	PElement newPelem = pArr->copyFunc((pArr->pElemArr)[idex]);
	return newPelem;
}

int GetAdptArraySize(PAdptArray pArr)
{
	return (pArr == NULL) ? -1 : pArr->ArrSize;
}

void PrintDB(PAdptArray pArr)
{
	if (!pArr)
		return;

	for (int i = 0; i < pArr->ArrSize; ++i)
	{
		if (pArr->pElemArr[i] != NULL)
		{
			pArr->printFunc(pArr->pElemArr[i]);
		}
	}
}

#ifndef UBARRAY_H
#define UBARRAY_H

#include "WCS_Pointer.h"
using namespace std;
//V3 Of Custom Unbounded Array. Some Optimization Has Been Done To Significantly Reduce The Size Of This Module.
//All Try-Catch Loops Have Been Removed And Replaced With Cout Statements, Thereby Boosting Performance A Little.
//All Index Types Support The Following Comparison Operators: ==, !=, <, <=, >, >=
//This Custom Array Supports Any Index And Object Type, Which Makes This Particulary Useful For Applications That Do Hashing

template <typename DATA, typename INDEX>
class UBArray
{
private:
	int currentindex;
	int size;
	struct Node: public RefCount
	{
		INDEX				Index;
		DATA				Data;
		WCS_Pointer <Node>	pNext;
		WCS_Pointer <Node>	pPrev;

		Node(const INDEX &i) : Index(i){};
	};

		UBArray <DATA, INDEX> &	operator =	(const UBArray <DATA, INDEX> &);//Assignment operator
		WCS_Pointer <Node>			pHead;
		WCS_Pointer <Node>			pTail;
		WCS_Pointer <Node>			pCurrent;
public:
		UBArray<DATA, INDEX>()
		{
			size = 0;
			currentindex = -1;
		}
		UBArray(const UBArray <DATA, INDEX> &)
		{
			size = 0;
			currentindex = -1;
		}
		~UBArray<DATA, INDEX>()
		{
			Clear();
		}
		void Clear()
		{
			if (!pHead) {}//If there is no head, do nothing
			else
			{
				pHead = (*pHead).pNext;
				while (pHead)
				{
					(*pHead).pPrev.SetToNull();
					pHead = (*pHead).pNext;
				}
				pHead.SetToNull();
				pTail.SetToNull();
				pCurrent.SetToNull();//Just in case it remains, make sure it's destroyed	
			}
			size = 0;
		}
		DATA GetFirst()
		{
			if (size == 0)
				return "There Are No Elements In The Array. Error Code 0";
			else
			{
				currentindex = 0;//Will set index pointer back to the head when this method is called
				pCurrent = pHead;
				return (*pHead).Data;
			}
		}
		DATA GetLast()
		{
			if (size == 0)
				return "There Are No Elements In The Array. Error Code 0";
			else
			{
				pCurrent = pTail;
				currentindex = size - 1;//Set index pointer to last index
				return (*pTail).Data;
			}
		}
		DATA GetNext()//Will execute from starting point if GetFirst wasn't called yet
		{
			if (!pHead)
				return "There Are No Elements In The Array. Error Code 0";
			else
			{
				currentindex++;
				if (currentindex > size - 1)
				{
					return "There Is Not Another Element In The Array. Error Code 1.";
					currentindex--;
				}
				else if (currentindex == 0)
				{
					pCurrent = pHead;
					return (*pCurrent).Data;
				}
				else
				{
					pCurrent = (*pCurrent).pNext;
					return (*pCurrent).Data;
				}
			}
		}
		DATA GetPrev()//Will cause an error if called after GetFirst is called
		{
			if (!pHead)
				return "There Are No Elements In The Array. Error Code 0";
			else
			{
				currentindex--;
				if (currentindex < 0)
				{
					return "There Is Not A Previous Element In The Array. Error Code 2.";
					currentindex++;
				}
				else if (currentindex == 0)
				{
					pCurrent = pHead;
					return (*pCurrent).Data;
				}
				else
				{
					pCurrent = (*pCurrent).pPrev;
					return (*pCurrent).Data;
				}
			}
		}
		void Remove(const INDEX &i)
		{
			WCS_Pointer <Node>		pTemp;
			WCS_Pointer <Node>		pTemp2;
			WCS_Pointer <Node>		pTemp3;
			if (!pHead)
				cout << "There Are No Elements In The Array. Error Code 0" << endl;
			else if (i > (*pTail).Index)
				cout << "No Such Element Exists. Error Code 3" << endl;
			else if (i < (*pHead).Index)
				cout << "No Such Element Exists. Error Code 3" << endl;
			else if (size == 1)
			{
				size--;
				(*pHead).pNext.SetToNull();
				pHead.SetToNull();
				(*pTail).pPrev.SetToNull();
				pTail.SetToNull();
			}
			else
			{
				size--;
				if (i == (*pHead).Index)
				{
					pTemp = pHead;
					pTemp2 = (*pHead).pNext;
					(*pTemp2).pPrev.SetToNull();
					pHead = pTemp2;
					pTemp.SetToNull();
				}
				else if (i == (*pTail).Index)
				{
					pTemp = pTail;
					pTemp2 = (*pTail).pPrev;
					(*pTemp2).pNext.SetToNull();
					pTail = pTemp2;
					pTemp.SetToNull();
				}
				else
				{
					for (pTemp = (*pHead).pNext; i > (*pTemp).Index; pTemp = (*pTemp).pNext);
					if (i == (*pTemp).Index)
					{
						pTemp2 = pTemp;
						pTemp3 = pTemp;
						pTemp2 = (*pTemp).pNext;
						pTemp3 = (*pTemp).pPrev;
						pTemp.SetToNull();
						(*pTemp3).pNext = pTemp2;
						(*pTemp2).pPrev = pTemp3;
					}
				}
			}
		}
		int Size()
		{
			return size;
		}
		DATA & At(const INDEX &i)//Does the same thing as the operator[] function. Just a different name in case someone prefers this name instead.
		{
			return operator[] (i);
		}
		const DATA & At(const INDEX &i) const//Read only version of At function (can only retrieve elements from arrays, cannot write to an array)
		{
			return const operator[] (i);
		}
		DATA & operator[] (const INDEX &i)
		{
			WCS_Pointer <Node>		pTemp;
			WCS_Pointer <Node>		pTemp2;

			if (!pHead)
			{			// this processes if the list was empty
				pHead = WCS_Pointer <Node>(new Node(i));
				pTail = pHead;
				size++;
				return (*pHead).Data;
			}
			else
				if (i == (*pHead).Index)		// processes if the index sought was the first index
					return (*pHead).Data;
				else
					if (i < (*pHead).Index)
					{		// processes if the index sought is in front of the first index
						pTemp = WCS_Pointer <Node>(new Node(i));
						(*pTemp).pNext = pHead;
						(*pHead).pPrev = pTemp;
						pHead = pTemp;
						size++;
						return (*pHead).Data;
					}
					else
						if (i == (*pTail).Index)
							return (*pTail).Data;
						else
							if (i > (*pTail).Index)
							{
								pTemp = WCS_Pointer <Node>(new Node(i));
								(*pTemp).pPrev = pTail;
								(*pTail).pNext = pTemp;
								pTail = pTemp;
								size++;
								return (*pTail).Data;
							}
							else
							{
								for (pTemp = (*pHead).pNext; i > (*pTemp).Index; pTemp = (*pTemp).pNext);
								if (i == (*pTemp).Index)
									return (*pTemp).Data;
								else
								{
									pTemp2 = WCS_Pointer <Node>(new Node(i));
									(*pTemp2).pNext = pTemp;
									(*pTemp2).pPrev = (*pTemp).pPrev;
									(*pTemp).pPrev = pTemp2;
									(*(*pTemp2).pPrev).pNext = pTemp2;
									size++;
									return (*pTemp2).Data;
								}
							}
		}
		const DATA & operator[] (const INDEX &i) const//Read only version of operator[] function (can only retrieve elements from arrays, cannot write to an array)
		{
			WCS_Pointer <Node>		pTemp;
			if (!pHead)
				return "There Are No Elements In The Array. Error Code 0.";
			else if (i > (*pTail).Index)
				return "No Such Element Exists. Error Code 3.";
			else if (i < (*pHead).Index)
				return "No Such Element Exists. Error Code 3.";
			else if (i == (*pHead).Index)// processes if the index sought was the first index
				return (*pHead).Data;
			else
			{
				for (pTemp = (*pHead).pNext; i >(*pTemp).Index; pTemp = (*pTemp).pNext);
				if (i == (*pTemp).Index)
					return (*pTemp).Data;
				else if (i < (*pTemp).Index)
					return "No Such Element Exists. Error Code 3.";
			}
		}
};
#endif

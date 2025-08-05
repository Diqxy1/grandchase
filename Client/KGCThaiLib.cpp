#if defined(NATION_THAILAND)

int __declspec(dllimport) WordWrapTH(const wchar_t *s, int pos[], size_t n);

#define LIMIT_FIND	20
#define MAXLINELENGTH 1000


std::wstring BreakStringTH(std::wstring strOriginalstring, int iLimitsize)
{
	int pt = 0;
	int fpt = 0;
	int iLeftChar = (int)strOriginalstring.length();
	std::wstring strW = strOriginalstring;
	std::wstring strR = L"";

	int pos[MAXLINELENGTH];
	int numCut;

	//최소 돌아가기 폭보다 적은 스트링에 대해서 워드랩을 할려는 경우.
	if (iLimitsize<=LIMIT_FIND)
		return strOriginalstring;

	//스트링폭 권장 25..
	if (iLimitsize<=25)
	{
#if defined(_DEBUG)
		return L"Assert";
#endif
	}

	numCut = WordWrapTH(strOriginalstring.c_str(),pos,MAXLINELENGTH);//th_wbrk(strOriginalstring.c_str(),pos,MAXLINELENGTH);

	const WCHAR *pChar = &strW[pt];
	for (int i=0;i<iLimitsize;i++)
	{
		pChar=GCUTIL_STR_THAI::CharNextTh(pChar);
	}
	pt = (int)(pChar - &strW[pt]);

	int iCharLimit=0;
	do 
	{
		bool bFind=false;
		if (pt>iLeftChar)
		{
			strR += strW.substr(fpt,iLeftChar);
			break;
		}

		for (int i=0;i< numCut;i++)
		{
			if (pos[i]==pt)
			{
				bFind=true;
				break;

			}
		}

		if (strW[pt] < 3424 || strW[pt] > 3424+255)
		{
			bFind=true;
		}


		//20칸 이상 되돌아 왔는데도 찾지 못하는 경우
		if (iCharLimit>LIMIT_FIND)
		{
			//그냥 단어단위로 짤라버린다.
			bFind=true;
		}

		if (bFind==true)
		{
			strR += strW.substr(fpt,pt-fpt);
			strR +=L"\n";
			//iLeftChar-=(pt-fpt);
			fpt=pt;
			//pt+=nSize;
			pChar = &strW[pt];
			for (int i=0;i<iLimitsize;i++)
			{
				pChar=GCUTIL_STR_THAI::CharNextTh(pChar);
			}
			pt += (int)(pChar - &strW[pt]);
			iCharLimit=0;
		}
		else
		{
			iCharLimit++;
			pt--;
		}
	} while (1);
	return strR;
}

// Cutting 가능한 위치백터 리턴하는 함수
//주의 pos 배열의 크기는 반드시 1000개 지키도록 할 것.
std::vector<bool> BreakStringTH(std::wstring strOriginalstring)
{
	int pt = 0;
	int fpt = 0;
	int iLeftChar = (int)strOriginalstring.length();
	int numCut;
	std::vector<bool> vecbCut;
	int pos[MAXLINELENGTH];
	vecbCut.clear();

	numCut = WordWrapTH(strOriginalstring.c_str(),pos,MAXLINELENGTH);//th_wbrk(strOriginalstring.c_str(),pos,MAXLINELENGTH);

	for (int i=0;i<(int)strOriginalstring.size();i++)
	{
		bool bFind=false;
		for (int j=0;j< numCut;j++)
		{
			if (i==pos[j])
			{
				bFind=true;
				break;

			}
		}

		if (strOriginalstring[i] < 3424 || strOriginalstring[i] > 3424+255)
		{
			bFind=true;
		}

		vecbCut.push_back(bFind);
	}
	return vecbCut;
}

#endif
#include "CArabicString.h"
#include "tools.h"

CArabicString::CArabicString(const string &str)
:m_strText(str)
{
	
}

bool isArabic(char c)
{
	if( ((unsigned char)c >= 128) && ((unsigned char)c <= 255) )
	{
		return true;
	}
	return false;
}

bool isAdditional(char c)
{
	if((c == ' ' || c == '\t') || c == '-' || c == '.' || c == '+' || c == '*')
	{
		return true;
	}
	return false;
}

std::list<string> splitLines(string text, char ch)
{
	string line;
	std::list<string> lines;

	for(int i = 0; i < text.length(); i++)
	{
		if(text.buffer()[i] == ch)
		{
			lines.push_back(line);
			line = "";
		}
		else
		{
			line += text.buffer()[i];
		}
	}
	if(text.buffer()[text.length()-1] != ch)
	{
		lines.push_back(line);
	}
	return lines;
}

string removeChar(string text, char ch)
{
	string result;

	for(int i = 0; i < text.length(); i++)
	{
		if(text.buffer()[i] != ch) result+=text.buffer()[i];
	}

	return result;
}

string CArabicString::rightToLeft(int length) const
{
	/*QString strText = QString::fromStdString(m_strText);

	QByteArray arrText(m_strText.data(),m_strText.length());
	QByteArray arrResult;

	for(int i = 0; i < arrText.size(); i++)
	{
		if(arrText[])
	}*/

	//string strResult = m_strText;
	/*int i = 0;
	string strResult;
	while(i < m_strText.length())
	{
		if(((unsigned char)m_strText[i] >= 128 && (unsigned char)m_strText[i] <= 255) || (m_strText[i]==' ' || m_strText[i]=='\t'))
		{
			int nStartPos = i;
			while(((unsigned char)m_strText[i] >= 128 && (unsigned char)m_strText[i] <= 255 && i < m_strText.length()) || (m_strText[i]==' ' || m_strText[i]=='\t'))
			{
				i++;
			}
			int nFinishPos = i-1;

			for(int j = nFinishPos; j >= nStartPos; j--)
			{
				strResult += m_strText[j];
			}

		}
		else
		{
			strResult += m_strText[i];
			i++;
		}
		
	}*/

	/*std::list<string> listTexts;
	
	while(i < m_strText.length())
	{
		if(((unsigned char)m_strText[i] >= 128 && (unsigned char)m_strText[i] <= 255) || (m_strText[i]==' ' || m_strText[i]=='\t'))
		{

		}
		i++;
	}*/

	//DBGLine(" CArabicString::rightToLeft ");
	//int i = 0;
	//string strResult;
	//bool b = false;
	//for(int i = 0; i < m_strText.length(); i++)
	//{
	//	DBGLine(" CArabicString::rightToLeft 1 %d",i);
	//	if(isArabic(m_strText.buffer()[i]))
	//	{
	//		DBGLine(" CArabicString::rightToLeft 2 %d",i);
	//		b = true;
	//		break;
	//	}
	//}
	//if(!b)
	//{
	//	DBGLine(" CArabicString::rightToLeft 3");
	//	return m_strText;
	//}

	//DBGLine(" CArabicString::rightToLeft 4");
	//std::list<string> lines = splitLines(removeChar(m_strText,'\r'),'\n');

	//DBGLine(" CArabicString::rightToLeft 5");
	//while(lines.size() > 0)
	//{
	//	DBGLine(" CArabicString::rightToLeft 6");
	//	string line = lines.front();
	//	lines.pop_front();

	//	DBGLine(" CArabicString::rightToLeft 7");
	//	string resultLine = "";
	//	i = 0;
	//	while(i < line.length())
	//	{
	//		DBGLine(" CArabicString::rightToLeft 8");
	//		char c = line.buffer()[i];
	//		if(isArabic(c) || isAdditional(c))
	//		{

	//			DBGLine(" CArabicString::rightToLeft 9");
	//			resultLine = c;
	//			resultLine += resultLine;
	//			i++;
	//		}
	//		else
	//		{
	//			DBGLine(" CArabicString::rightToLeft 10");
	//			string strTemp;
	//			while(i < line.length())
	//			{
	//				DBGLine(" CArabicString::rightToLeft 11");
	//				if(!(isArabic(line.buffer()[i]) || isAdditional(line.buffer()[i])))
	//				{
	//					strTemp += line.buffer()[i];

	//				}
	//				else
	//				{
	//					break;
	//				}
	//				i++;
	//			}
	//			resultLine = strTemp;
	//			resultLine += resultLine;

	//		}

	//	}
	//	/*while(resultLine.length() < length)
	//	{
	//		DBGLine(" CArabicString::rightToLeft 12");
	//		resultLine = ' ' + resultLine;
	//	}*/
	//	strResult += resultLine;
	//	strResult += '\n';

	//	DBGLine(" CArabicString::rightToLeft 13");
	//}

	//

	////QByteArray arrText(strResult.data(),strResult.size());

	//DBGLine(" CArabicString::rightToLeft ");
// 	int i = 0;
// 	string strResult;
// 	for (i=0;i<length;i++) strResult+=" ";
// 	string strch;
// 
// 	for (i=0;(i<length)&&(i<m_strText.length());i++)
// 	{
// 		strResult.buffer()[length-1-i]=m_strText.buffer()[i];
// 		//DBGLine(" strch     \"%s\"",strch.buffer());
// 		//DBGLine(" strResult \"%s\"",strResult.buffer());
// 	}
// 	return strResult;




// 	// Урезание пробелов слева
// 	// Поиск первого непробельного символа
// 	int nPosSymbol = 0;
// 	for (int idx = 0; idx < m_strText.length(); idx++)
// 	{
// 		if (m_strText[idx] != ' ')
// 		{
// 			nPosSymbol = idx;
// 			break;
// 		}
// 	}
// 
// 	// Копируем до последнего непробельного символа
// 	char* strText = new char[nPosSymbol + 2];
// 	memset(strText, 0, nPosSymbol + 2);
// 	memcpy(strText, m_strText.buffer(), nPosSymbol + 1);
// 
// 	// Переворачиваем строку
// 	string strResult(strText);
// 	for (int idx = 0; idx < strlen(strText); idx++)
// 	{
// 		strResult.buffer()[idx] = strText[strlen(strText) - idx - 1];
// 	}
// 	DELETE_POINTER_ARR(strText);
// 
// 	return strResult;
// 
// 	return m_strText;

	char* pStrResult = new char[length + 1];
	memset(pStrResult, ' ', length);
	pStrResult[length] = 0;

	int idxSrc = m_strText.length() - 1;
	for (int idxResult = length - 1; idxResult >= 0; idxResult--)
	{
		if (idxSrc < 0)
			break;
		pStrResult[idxResult] = m_strText.buffer()[idxSrc];
		idxSrc--;
	}
	string strResult(pStrResult);
	DELETE_POINTER_ARR(pStrResult);
	return strResult;
}

CArabicString::~CArabicString()
{

}

#include "CArabicEncoder.h"
#include "tools.h"

#define UNDEF_SYMBOL 0

CArabicEncoder::CArabicEncoder(const string &text)
:m_strText((text))
{

}

char *ToArabic(char* Striem);

string CArabicEncoder::encode() const
{
	string strResult = m_strText;
	DBGBuf("Arabic encoder in", (BYTE*) strResult.buffer(), strResult.length());
	ToArabic(strResult.buffer());
	DBGBuf("Arabic encoder out", (BYTE*) strResult.buffer(), strResult.length());
	return strResult;
};

CArabicEncoder::~CArabicEncoder()
{

}














const struct {
	int Graph;
	int CharNumber;
	char Alpha1;
	char Alpha2;
	char Alpha3;
	char Alpha4;
} CharDataBease[] = {
	/*(Key) , (Code) (Start) (Mean) (End) (Alone)*/
	{'_', 95, NULL, NULL, NULL, NULL},
	{'h', 104, 0xB0, 0xB1, 0xB1, 0xB0}, /*?*/
	{'f', 102, 0xB3, 0xB3, 0xB2, 0xB2}, /*?*/
	{'j', 106, 0xB7, 0xB7, 0xB6, 0xB6}, /*?*/
	{'e', 101, 0xB9, 0xB9, 0xB8, 0xB8}, /*?*/
	{'[', 91, 0xBB, 0xBB, 0xBA, 0xBA}, /*?*/
	{'p', 112, 0xBF, 0xBF, 0xBE, 0xBE}, /*?*/
	{'o', 111, 0xC1, 0xC1, 0xC0, 0xC0}, /*?*/
	{']', 93, 0xC2, 0xC2, 0xC2, 0xC2}, /*?*/
	{'`', 96, 0xC3, 0xC3, 0xC3, 0xC3}, /*?*/
	{'v', 118, 0xC4, 0xC4, 0xC4, 0xC4}, /*?*/
	{'.', 46, 0xC5, 0xC5, 0xC5, 0xC5}, /*?*/
	{'s', 115, 0xC8, 0xC8, 0xC7, 0xC7}, /*?*/
	{'a', 97, 0xCA, 0xCA, 0xC9, 0xC9}, /*?*/
	{'w', 119, 0xCC, 0xCC, 0xCB, 0xCB}, /*?*/
	{'q', 113, 0xCE, 0xCE, 0xCD, 0xCD}, /*?*/
	{'\'', 39, 0xCF, 0xCF, 0xCF, 0xCF}, /*?*/
	{'/', 47, 0xD0, 0xD0, 0xD0, 0xD0}, /*?*/
	{'u', 117, 0xD4, 0xD3, 0xD2, 0xD1}, /*?*/
	{'y', 121, 0xD8, 0xD7, 0xD6, 0xD5}, /*?*/
	{'t', 116, 0xDA, 0xDA, 0xD9, 0xD9}, /*?*/
	{'r', 114, 0xDC, 0xDC, 0xDB, 0xDB}, /*?*/
	{';', 59, 0xDE, 0xDE, 0xDD, 0xDD}, /*?*/
	{'g', 103, 0xE3, 0xE3, 0xE1, 0xE1}, /*?*/
	{'l', 108, 0xE5, 0xE5, 0xE4, 0xE4}, /*?*/
	{'k', 107, 0xE7, 0xE7, 0xE6, 0xE6}, /*?*/
	{'i', 105, 0xEB, 0xEA, 0xE9, 0xE9}, /*?*/
	{'m', 109, 0xB7, 0xB7, 0xE9, 0xE9}, /*?*/
	{',', 44, 0xE8, 0xE8, 0xE8, 0xE8}, /*?*/
	{'d', 100, 0xEE, 0xEE, 0xFA, 0xFA}, /*?*/
	{'b', 98, 0xE2, 0xE2, 0xE2, 0xE2}, /*??*/
	{'x', 120, 0xAF, 0xAF, 0xAF, 0xAF}, /*?*/
	{'z', 122, 0xAE, 0xAE, 0xFA, 0xFA}, /*??*/
	{'N', 78, 0xAD, 0xB1, 0xB1, 0xAD}, /*?*/
	{'n', 110,0xFA, 0xFA, 0xFA, 0xFA}, /*?*/	
	{'J', 74, 0, 0xAB, 0, 0}, /*?*/
	{'?', 63, 0xAC, 0xAC, 0xAC, 0xAC}, /*?*/
	{'!', 33, 0x21, 0x21, 0x21, 0x21}, /*!*/
	{' ', 32, 0x20, 0x20, 0x20, 0x20}, /* Space */
	{'0', 48, 0xA0, 0xA0, 0xA0, 0xA0}, /*0*/
	{'1', 49, 0xA1, 0xA1, 0xA1, 0xA1}, /*1*/
	{'2', 50, 0xA2, 0xA2, 0xA2, 0xA2}, /*2*/
	{'3', 51, 0xA3, 0xA3, 0xA3, 0xA3}, /*3*/
	{'4', 52, 0xA4, 0xA4, 0xA4, 0xA4}, /*4*/
	{'5', 53, 0xA5, 0xA5, 0xA5, 0xA5}, /*5*/
	{'6', 54, 0xA6, 0xA6, 0xA6, 0xA6}, /*6*/
	{'7', 55, 0xA7, 0xA7, 0xA7, 0xA7}, /*7*/
	{'8', 56, 0xA8, 0xA8, 0xA8, 0xA8}, /*8*/
	{'9', 57, 0xA9, 0xA9, 0xA9, 0xA9}, /*9*/
	{'c', 99, 0xFB, 0xFC, 0xFC, 0xFB}, /*?*/
	{0, 0, NULL, NULL, NULL, NULL},
};

char check_char[] = {120, 93, 96, 46, 118, 44, 104, 98, 0};

char *ToArabic(char* Striem) {

	int i = 0, x = 0, z, test, slen = strlen(Striem);
	char N_Char = 0, P_Char = 0;
	char *p1, *p2;
	char localarray[128] = {0};
	char *localptr = localarray + 1;
	int xOld;

	if (slen <= 0)
		return NULL;
	//Reverce String
	for (p1 = Striem, p2 = Striem + slen - 1; p2 > p1; ++p1, --p2) {
		*p1 ^= *p2;
		*p2 ^= *p1;
		*p1 ^= *p2;
	}
	//Get Char Number
looping:
	xOld = x;
	for (i = 0; CharDataBease[i].Graph; i++) {

		if (Striem[x] != '\0') {
			if (CharDataBease[i].Graph == Striem[x]) {
				test = CharDataBease[i].CharNumber;
				localptr[x] = test;
				x += 1;
				i = 0;
			}
		} else {
			break;
		}

	}
	if (x != slen) {
		if (xOld == x)
		{
			x++;
			xOld = x;
		}
		goto looping;
	} else {

		localptr[x] = '\0';

	}

	z = x = 0;

	//Convert Number To Arabic Char
	for (i = 0; i <= slen; i++) {
Strarting:
		x = 0;
		P_Char = localptr[i - 1];
		N_Char = localptr[i + 1];

		if (N_Char == '\0') {
			//Yes
AlonArea:
			if (P_Char == '\0') {
				//Yes
				for (x; CharDataBease[x].CharNumber; x++) {
					if (CharDataBease[x].CharNumber == localptr[i]) {
						Striem[i] = CharDataBease[x].Alpha4; //Alon
						x = 0;
						break;
					}
				}
			} else {
				//No
StartArea:
				for (x = 0; CharDataBease[x].CharNumber; x++) {
					if (CharDataBease[x].CharNumber == localptr[i]) {
						Striem[i] = CharDataBease[x].Alpha1; //Start
						x = 0;
						break;
					}
				}
			}
		} else {
			//NO
			if (N_Char == 32) {
				//Yes
				goto AlonArea;
			} else {
				//No
				// NCC Area
				for (x = 0; x <= strlen(check_char); x++) {
					if (check_char[x] == N_Char) {
						// Yes NCC Fund Char
						for (x = 0; CharDataBease[x].CharNumber; x++) {
							if (P_Char == '\0') {
								//Yes No Previos
								if (CharDataBease[x].CharNumber == localptr[i]) {
									if (localptr[i] == 117 || localptr[i] == 121)
										Striem[i] = CharDataBease[x].Alpha4; //Alon
									else
										Striem[i] = CharDataBease[x].Alpha3; //End
									x = 0;
									break;
								}
							} else {
								goto StartArea;
								// Striem[i] = CharDataBease[x].Alpha1; //Start
								// x = 0;
								break;
							}
						}

						i += 1;
						goto Strarting;
						break;
					}
				}
				x = 0;
				//Normal Exit : Not Found NCC Char
				if (P_Char == '\0') {
					//Yes
EndArea:
					for (x; CharDataBease[x].CharNumber; x++) {
						if (CharDataBease[x].CharNumber == localptr[i]) {
							if (localptr[i] == 120 || localptr[i] == 93 || localptr[i] == 96 || localptr[i] == 46 || localptr[i] == 118 || localptr[i] == 44 || localptr[i] == 104 || localptr[i] == 98) {
								Striem[i] = CharDataBease[x].Alpha2; //Mean
							} else {
								Striem[i] = CharDataBease[x].Alpha3; //End
							}
LocalEnd:
							x = 0;
							break;
						}//End If
					}//End For
				} else {
					//No
					if (P_Char == 32) {
						//Yes
						goto EndArea;
					} else {
						//No
						for (x; CharDataBease[x].CharNumber; x++) {
							if (CharDataBease[x].CharNumber == localptr[i]) {
								if (i == 0) {
									Striem[i] = CharDataBease[x].Alpha3; //End
								} else if (i == slen) {
									Striem[i] = CharDataBease[x].Alpha1; //Start
								} else if (i < slen) {
									if ( N_Char == 'b' || N_Char == 'N' || N_Char == '\0') {
										Striem[i] = CharDataBease[x].Alpha1; //Start
									} else {
										Striem[i] = CharDataBease[x].Alpha2; //Mean
									}
								}
								x = 0;
								break;
							}
						}
					}
				}
			}
		}
	}
	return Striem;

}

char *ToArabicOld(char* Striem) {

	int i = 0, x = 0, z, test, slen = strlen(Striem);
	char N_Char = NULL, P_Char = NULL;
	char *p1, *p2;
	char localarray[128];
	char *localptr = localarray;
	int xOld;


	if (slen <= 0)
		return NULL;

	//Reverce String
	for (p1 = Striem, p2 = Striem + slen - 1; p2 > p1; ++p1, --p2) {
		*p1 ^= *p2;
		*p2 ^= *p1;
		*p1 ^= *p2;
	}

	//Get Char Number
looping:
	xOld = x;
	for (i = 0; CharDataBease[i].Graph; i++) {

		if (Striem[x] != '\0') {
			if (CharDataBease[i].Graph == Striem[x]) {
				test = CharDataBease[i].CharNumber;
				localptr[x] = test;
				x += 1;
				i = 0;
			}
		} else {
			break;
		}

	}
	if (x != slen) {
		if (xOld == x)
		{
			x++;
			xOld = x;
		}
		goto looping;
	} else {

		localptr[x + 1] = '\000';

	}

	i = z = x = 0;

	//Convert Number To Arabic Char
	for (i = 0; i <= slen; i++) {
Strarting:


		P_Char = localptr[i - 1];
		N_Char = localptr[i + 1];

		if (N_Char == '\0') {
			if (P_Char == '\0' || P_Char == 32) {
				for (x; CharDataBease[x].CharNumber; x++) {
					if (CharDataBease[x].CharNumber == localptr[i]) {
						Striem[i] = CharDataBease[x].Alpha4; //Alone
						x = 0;
						break;
					}
				}
			} else {
				for (x; CharDataBease[x].CharNumber; x++) {
					if (CharDataBease[x].CharNumber == localptr[i]) {
						Striem[i] = CharDataBease[x].Alpha1; //Start
						x = 0;
						break;
					}
				}
			}
		} else if (N_Char == 32) {
			if (P_Char == '\0' || P_Char == 32) {
				for (x; CharDataBease[x].CharNumber; x++) {
					if (CharDataBease[x].CharNumber == localptr[i]) {
						Striem[i] = CharDataBease[x].Alpha4; //Alone
						x = 0;
						break;
					}
				}
			} else {
				for (x; CharDataBease[x].CharNumber; x++) {
					if (CharDataBease[x].CharNumber == localptr[i]) {
						Striem[i] = CharDataBease[x].Alpha1; //Start
						x = 0;
						break;
					}
				}
			}
		} else {
			//NCC
			for (x = 0; x <= strlen(check_char); x++) {
				if (check_char[x] == N_Char) {
					for (x = 0; CharDataBease[x].CharNumber; x++) {
						if (CharDataBease[x].CharNumber == localptr[i]) {
							Striem[i] = CharDataBease[x].Alpha1; //Start
							x = 0;
							break;
						}
					}
					i += 1;
					goto Strarting;
					break;
				}
			}




			if (P_Char == '\0' || P_Char == NULL) {
				for (x = 0; CharDataBease[x].CharNumber; x++) {
					if (CharDataBease[x].CharNumber == localptr[i]) {
						Striem[i] = CharDataBease[x].Alpha3; //End
						x = 0;
						break;
					}
				}
			} else if (P_Char == 32) {
				for (x = 0; CharDataBease[x].CharNumber; x++) {
					if (CharDataBease[x].CharNumber == localptr[i]) {
						Striem[i] = CharDataBease[x].Alpha3; //End
						x = 0;
						break;
					}
				}
			} else {

				for (x = 0; CharDataBease[x].CharNumber; x++) {
					if (CharDataBease[x].CharNumber == localptr[i]) {
						if (i == 0) {
							Striem[i] = CharDataBease[x].Alpha3; //End
						} else if (i == slen) {
							Striem[i] = CharDataBease[x].Alpha1; //Start
						}else{
							Striem[i] = CharDataBease[x].Alpha2; //Start
						}
						x = 0;
						break;
					}
				}
			}
		}
	}
	return Striem;

}


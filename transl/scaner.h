#ifndef __SCANER
#define __SCANER

#include "defs.h"

class TScaner{
private:
	TypeMod t; // �������� �����	
	int stroka;
	int simvol;
public :
	int uk; // ��������� ������� ������� � �������� ������
public:
	void PutUK (int i);
	int GetUK (void);
	void PutSimvol (int i);
	int GetSimvol (void);
	void PutStroka (int i);
	int GetStroka  (void);
	void PrintError(char *, char *);
	void PrintError (char * err, int uk);
	int Scaner (TypeLex l);
	void GetData(char *);
	TScaner(char *);
	~TScaner() {}
};
#endif
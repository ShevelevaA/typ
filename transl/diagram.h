// ����� �������������� ��������

#include "scaner.h"
#include "treeSemant.h"

#ifndef __DIAGRAM
#define __DIAGRAM


class TDiagram
{
private:
	TScaner *sc;
public:
	Tree *tr;
	Tree *pt;
	Tree * pr;
	bool FlInt;
	TDiagram(void) {pt = tr = new Tree(sc);}
	TDiagram(TScaner * s) {sc=s;pt = tr = new Tree(sc);}
	~TDiagram(){}
	void Progr(); // ���������
	void GlavProgr(); // ������� ���������
	void Block(); // ����
	void BodyBlock(); // ���� �����
	void Operat(); // ��������
	//void Operats(); // ��������
	void OpisPerem(); // �������� ����������
	void Spisok(DataType ); // ������
	void Perem(DataType ); // ����������
	TData * Vyrazh(); // ���������
	TData * Slagaem(); // ���������
	TData * Mnozhit(); // ���������
	TData * ElemVyrazh(); // ������������ ���������
	void Func(); // �������
	void BodyFunc();
	int SpisokFormPerem(); // ������ ���������� ����������
	/*TData * */ void Prisvaiv(); // ������������
	void VyzovFunc(); // ����� �������
	int SpisokFactPerem(char []); // ������ ����������� ����������
	void Switch_(); // switch
	void Case_(); // case
	void TipDan(); // ��� ������
};
#endif
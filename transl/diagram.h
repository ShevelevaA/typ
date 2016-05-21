// Класс синтаксических диаграмм

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
	void Progr(); // программа
	void GlavProgr(); // главная программа
	void Block(); // блок
	void BodyBlock(); // тело блока
	void Operat(); // оператор
	//void Operats(); // ператоры
	void OpisPerem(); // описание переменных
	void Spisok(DataType ); // список
	void Perem(DataType ); // переменная
	TData * Vyrazh(); // выражение
	TData * Slagaem(); // слагаемое
	TData * Mnozhit(); // множитель
	TData * ElemVyrazh(); // элементарное выражение
	void Func(); // функция
	void BodyFunc();
	int SpisokFormPerem(); // список формальных переменных
	/*TData * */ void Prisvaiv(); // присваивание
	void VyzovFunc(); // вызов функции
	int SpisokFactPerem(char []); // список фактических переменных
	void Switch_(); // switch
	void Case_(); // case
	void TipDan(); // тип данных
};
#endif
#include "stdafx.h"

#include "diagram.h"
#include "defs.h"
#include "Scaner.h"


#include <fstream>
#include <iostream>
using namespace std;


void TDiagram::Progr(){ // программа

	//           ____ |функция| ______    void
	//          |__ |опис. перем| ____|   int, double идентиф
	//          |__ |главная прогр| __|   int main
	// ________\|/____________________|____


	TypeLex l; int t,uk1,simv,str;

	uk1=sc->GetUK(); t=sc->Scaner(l); //sc->PutUK(uk1);

	str=sc->GetStroka();
	simv=sc->GetSimvol();

	while((t==Type_Int) || (t==Type_Double) || (t==Type_Void)) {
			if((t==Type_Int) || (t==Type_Double)){
				/*if(t==Type_Int){
					//sc->PutUK(uk1);//uk1=sc->GetUK(); 
					
					//else{
						//cout<<"Я мб описание"<<endl; 
						sc->PutUK(uk1);
						sc->PutStroka(str);
							sc->PutSimvol(simv);
						OpisPerem();
					//}
				}*/
				//else{
					//cout<<"Я мб описание"<<endl; 
					sc->PutUK(uk1);
					sc->PutStroka(str);
							sc->PutSimvol(simv);
					OpisPerem();
			//	}

			}
			else
				if(t==Type_Void){
					t=sc->Scaner(l); 
					if(t==Type_Main){
						//cout<<"Я мб главная функция"<<endl; 
						sc->PutUK(uk1);
						sc->PutStroka(str);
						sc->PutSimvol(simv);
						if (!pt -> sem_override("main"))
						{
							pt -> set_Left(new Node("main", type_void));
							pt = pt -> Left;
						}
						GlavProgr();
					}
					else{
						//cout<<"Я мб функция"<<endl; 
						sc->PutUK(uk1);
						sc->PutStroka(str);
						sc->PutSimvol(simv);
						Func();
					}
				}
				
				uk1=sc->GetUK(); t=sc->Scaner(l);// sc->PutUK(uk1);
				str=sc->GetStroka();
				simv=sc->GetSimvol();
	}

}

//робит
void TDiagram::GlavProgr(){ // главная программа

	// ---- void --- main --- ( ---- ) --- |блок| -----

	TypeLex l; int t, uk1;

	t=sc->Scaner(l);
	if(t!=Type_Void) sc->PrintError("ожидался тип void",l);

	t=sc->Scaner(l);
	if(t!=Type_Main) sc->PrintError("ожидался идентификатор main",l);

	t=sc->Scaner(l);
	if(t!=Type_Skobka_Op) sc->PrintError("ожидался символ (",l);

	t=sc->Scaner(l);
	if(t!=Type_Skobka_Cl) sc->PrintError("ожидался символ )",l);
	
	Tree * pt_old = pt;
	pt -> set_Right(new Node());
	pt = pt -> Right;
	
	Block();
	pt = pt_old;
}

//роби
void TDiagram::Block(){ // блок

	//  ---- { ----- |тело блока| ---- } ------

	TypeLex l; int t,uk1, str,simv;

	t=sc->Scaner(l);
	/*Tree * pt_old = pt;
	pt -> set_Right(new Node());
	pt = pt -> Right;
	*/
	if(t!=Type_Open_Blok) sc->PrintError("ожидался символ {",l);
	uk1=sc->GetUK();
		str=sc->GetStroka();
	simv=sc->GetSimvol();
	t=sc->Scaner(l);
	sc->PutUK(uk1);
	sc->PutStroka(str);
							sc->PutSimvol(simv);

	Tree * pt_old = pt;
	pt -> set_Right(new Node());
	pt = pt -> Right;

	BodyBlock();

	uk1=sc->GetUK();
	t=sc->Scaner(l);
		str=sc->GetStroka();
	simv=sc->GetSimvol();
	if(t!=Type_Close_Blok) sc->PrintError("ожидался символ }",l);
	pt = pt_old;
}



void TDiagram::BodyBlock(){ // тело блока

	//                 _______ |оператор| _______
	//            |---|____ |описание перем| ____|---|
	// __________\|/_________________________________|_________

	TypeLex l; int t,uk1, str, simv;

	char typebuf[MAX_LEX];
	
	typebuf[0] = '\0';
	
	uk1=sc->GetUK(); t=sc->Scaner(l);
		str=sc->GetStroka();
	simv=sc->GetSimvol();
	while((t==Type_Int) || (t==Type_Double) || (t==Type_Open_Blok) 
		|| (t==Type_Switch) || (t==Type_Id) || (t==Type_End_Oper) ) {

			strcpy_s(typebuf, l);

			sc->PutUK(uk1);
			sc->PutStroka(str);
							sc->PutSimvol(simv);
			if((t==Type_Int) || (t==Type_Double))
				OpisPerem();
			else
				Operat();
			uk1=sc->GetUK(); t=sc->Scaner(l);
				str=sc->GetStroka();
	simv=sc->GetSimvol();
	}
	sc->PutUK(uk1);
	sc->PutStroka(str);
	sc->PutSimvol(simv);
}


void TDiagram::Operat(){ //оператор

	//         ___ |присваивание| ___         идент =
	//        |______ |switch| ______|         switch
	//        |___ |вызов функц| ____|         идент (
	//   -----|_______ |блок| _______|------   {
	//        |__________ ; _________|     
	
	TypeLex l; int t,uk1, str, simv;
	uk1=sc->GetUK(); t=sc->Scaner(l);
		str=sc->GetStroka();
	simv=sc->GetSimvol();

	if (t==Type_End_Oper) return; // пустой оператор

	if(t==Type_Switch) { 
		sc->PutUK(uk1); 
		sc->PutStroka(str);
							sc->PutSimvol(simv);
		Switch_(); 
		return;
	}

	if(t==Type_Open_Blok) {

		sc->PutUK(uk1); 
		sc->PutStroka(str);
		sc->PutSimvol(simv);

		pt -> set_Left(new Node());
		pt = pt -> Left;
		Tree *pt_old = pt;
		pt -> set_Right(new Node());
		pt = pt -> Right;

		Block(); 

		pt = pt_old;

		//return;
	}

	//определение присваивания и вызова функции
	//uk1=sc->GetUK();
	

	else if (t == Type_Id)
	{
		pt -> sem_var_declared(l);
		/*if (pt -> sem_check_func(l))
		{
			sc -> PrintError("Функции присваивать нельзя!", l);
		}*/
		t = sc -> Scaner(l);
		 sc->PutUK(uk1);
	 sc->PutStroka(str);
							sc->PutSimvol(simv);

		if (t==Type_Equall)	{
		Prisvaiv();
		}
		else{
	//t=sc->Scaner(l);
	 VyzovFunc();
	}

		
	}

	/*
	pt -> sem_var_declared(l);
	
	t=sc->Scaner(l);
	 sc->PutUK(uk1);
	 sc->PutStroka(str);
							sc->PutSimvol(simv);
	if (t==Type_Equall)	{
		Prisvaiv();
		//t=sc->Scaner(l);
		//if (t!=Type_End_Oper) sc->PrintError("ожидался символ ;",l);
	
	}
	else{
	t=sc->Scaner(l);
	 VyzovFunc();
	}*/
} 


/*void TDiagram::Operats(){ //операторы
	
	//         ___ оператор_____
	//        |                 |
	// ______\|/________________|______

	TypeLex l; int t,uk1;
	uk1=sc->GetUK(); t=sc->Scaner(l); sc->PutUK(uk1);

	while( (t==Type_Id) || (t==Type_Switch) || (t==Type_End_Oper)
		|| (t==Type_Open_Blok)){
			Operat();
			uk1=sc->GetUK(); t=sc->Scaner(l); sc->PutUK(uk1);
	}
}
*/


//робит
// не считывает указатель перед началом
//не возвращает указатель на след-ий символ
void TDiagram::OpisPerem(){ //описание переменных

	//         ___ int ____
	//  ----- |__ double __| --- |список| ---- ; -----

	TypeLex l; 
	int t,uk1, str, simv;

	DataType type;

	char typebuf[MAX_LEX];
	typebuf[0] = '\0';

	//uk1=sc->GetUK(); 
	t=sc->Scaner(l);

	if ( (t!=Type_Int) && (t!=Type_Double) ) sc->PrintError("ожидался тип данных int или double",l);

	strcpy_s(typebuf, l);
	
	if (t == Type_Int)
		type = type_int;
	else if (t == Type_Double)
		type = type_double;

	uk1=sc->GetUK();
		str=sc->GetStroka();
	simv=sc->GetSimvol();
	
	t=sc->Scaner(l);
	sc->PutUK(uk1);
	sc->PutStroka(str);
	sc->PutSimvol(simv);

	Spisok(type);

	uk1=sc->GetUK();
		str=sc->GetStroka();
	simv=sc->GetSimvol();
	t=sc->Scaner(l);
	if (t!=Type_End_Oper) sc->PrintError("ожидался символ ;",l);
	//sc->PutUK(uk1);

}

//робит
// считывает указатель перед началом (там дб начало переменнной, те индентификатор)
//возвращает указатель на след-ий символ (он дб равен ;)
void TDiagram::Spisok(DataType type){ //список

	//         _________ , _________
	//        |                     |
	// ______\|/__ |переменная| ____|_______

	TypeLex l; int t, uk1,str, simv;

	

	uk1=sc->GetUK();
		str=sc->GetStroka();
	simv=sc->GetSimvol();
	do {
		Perem(type);
		uk1=sc->GetUK(); t=sc->Scaner(l);
			str=sc->GetStroka();
	simv=sc->GetSimvol();
	} while (t==Type_Zapyat);
	sc->PutUK(uk1);
	sc->PutStroka(str);
							sc->PutSimvol(simv);
}

//робит
//возвращает указатель на след-ий символ (либо тоЮ что возвр-ет выр-ие)
void TDiagram::Perem(DataType  type){

	//                         ____ = ____ |выражение| ___ /*;*/ ___
	// ---- идентификатор ----|_________________________________|-------

	TypeLex l; int t, uk1, str, simv;

	char typebuf[MAX_LEX];
	typebuf[0] = '\0';

	t=sc->Scaner(l);
	if (t!=Type_Id) sc->PrintError("ожидался идентификатор",l);

	if (! pt -> sem_override(l))
		pt = pt -> sem_add_var(l, type);

	uk1=sc->GetUK(); t=sc->Scaner(l);
		str=sc->GetStroka();
	simv=sc->GetSimvol();
	
	if(t==Type_Equall){
		uk1=sc->GetUK();
		str=sc->GetStroka();
		simv=sc->GetSimvol();
		t=sc->Scaner(l);
		sc->PutUK(uk1);
		sc->PutStroka(str);
		sc->PutSimvol(simv);

		TData * exp = Vyrazh();

		if (exp != NULL)
			{
				if (exp -> dataType == type_double && type == type_int) 
					sc -> PrintError("Присваивание невозможно!", sc -> uk);	
				if (type == type_int)
				{
					pt -> n -> dataValue.isInt = exp -> dataValue.isInt;
					if (FlInt)
						cout << pt -> n -> id << " = " << pt -> n -> dataValue.isInt << endl;
				}
				else 
				{
					if (pt -> n -> type == type_double && exp ->dataType == type_int)
					{
						pt -> n -> dataValue.isDouble = exp -> dataValue.isInt;
						if (FlInt)
							cout << pt -> n -> id << " = " << pt -> n -> dataValue.isDouble << endl;
					}
					else
					{
						pt -> n -> dataValue.isDouble = exp -> dataValue.isDouble;
						if (FlInt)
							cout << pt -> n -> id << " = " << pt -> n -> dataValue.isDouble << endl;
					}
				}	
		}

			//t=sc->Scaner(l);
			//if(t!=Type_End_Oper) sc->PrintError("ожидался символ ;",l);
	}
	else
		sc->PutUK(uk1);
	sc->PutStroka(str);
							sc->PutSimvol(simv);
}

//считывает указ-ль перед началом
// возвращает ук-ль на следующий
//робит
TData * TDiagram::Vyrazh(){

	//                                                         __ > ___
	//                                                        |__ < ___|
	//                                                        |__ >= __|
	//                                                        |__ <= __|
	//                                                        |__ == __|
	//                                      __ |слагаемое| ___|__ != __|___
	//       __ + __                       |                               |
	// _____|_______|____ |слагаемое| ____\|/______________________________|___________
	//      |__ - __|

	TypeLex l; int t,uk1, str, simv;

	TData * op1 = NULL;
	TData * op2 = new TData();

//	char typebuf[MAX_LEX];
//	typebuf[0] = '\0';
	

	uk1=sc->GetUK(); 
	str=sc->GetStroka();
	simv=sc->GetSimvol();
	t=sc->Scaner(l);
	if ( (t!=Type_Plus) && (t!=Type_Minus) )
		sc->PutUK(uk1);
	sc->PutStroka(str);
	sc->PutSimvol(simv);

	op2 = Slagaem();
	op1 = op2;

	uk1=sc->GetUK(); t=sc->Scaner(l); 
	str=sc->GetStroka();
	simv=sc->GetSimvol();
	while ((t>=Type_More) && (t<=Type_No_Eq)) { // знаки сравнения стоят подряд
				
			op2 = Slagaem();    /// он дб здесь или в конце ????

			
		switch (t)
				{
				case Type_Little:
					op1 = pt -> performCondition(op1, op2, Less);
					break;
				case Type_Little_Eq:
					op1 = pt -> performCondition(op1, op2, eqLess);
					break;
				case Type_More:
					op1 = pt -> performCondition(op1, op2, More);
					break;
				case Type_More_Eq:
					op1 = pt -> performCondition(op1, op2, eqMore);
					break;
				case Type_Eq_Eq:
					op1 = pt -> performCondition(op1, op2, eqeq);
					break;
				case Type_No_Eq:
					op1 = pt -> performCondition(op1, op2, neq);
					break;
				}	
		
			uk1=sc->GetUK(); t=sc->Scaner(l);
			str=sc->GetStroka();
			simv=sc->GetSimvol();
		
	}
	sc->PutUK(uk1);
	sc->PutStroka(str);
	sc->PutSimvol(simv);

	return op1;
}

//сразу уходит на множитель, сохраняет свои знаки и опять на множитель
//робит
TData * TDiagram::Slagaem(){ //слагаемое

	//                                          __ + __
	//                     _____ множитель ___ |__ - __|___
	//                    |                                |
	//____ множитель ____\|/_______________________________|______

	TypeLex l; int t,uk1, str, simv;

	TData * op1 = NULL;
	TData * op2 = new TData();

	char typebuf[MAX_LEX];
	typebuf[0] = '\0';

	op2 = Mnozhit();
	op1 = op2;

	uk1=sc->GetUK(); t=sc->Scaner(l);
	str=sc->GetStroka();
	simv=sc->GetSimvol();
	while ((t==Type_Plus) || (t==Type_Minus))
	{
		op2 = Mnozhit();
		if (t == Type_Plus)
			{
				op1 = pt -> performOperation(op1, op2, add);
			}
			else
			{
				op1 = pt -> performOperation(op1, op2, substract);
			}

		uk1=sc->GetUK(); t=sc->Scaner(l);
		str=sc->GetStroka();
		simv=sc->GetSimvol();
	}
	sc->PutUK(uk1);
	sc->PutStroka(str);
	sc->PutSimvol(simv);

	return op1;
}

//сразу уходит на элемент выр-ие, сохраняет свои знаки и опять на эл-ое выр-ие
//робит
TData * TDiagram::Mnozhit(){

	//                                                  __ * __
	//                          ___ элем.выражение ___ |__ / __|__
	//                         |                       |__ % __|  | 
	//____ элем.выражение ____\|/_________________________________|______

	TypeLex l; int t,uk1, str, simv;

	TData * op1 = NULL;
	TData * op2 = new TData();

	///char typebuf[MAX_LEX];
	//typebuf[0] = '\0';

	op2 = ElemVyrazh();
	op1 = op2;

	uk1=sc->GetUK(); t=sc->Scaner(l);
	str=sc->GetStroka();
	simv=sc->GetSimvol();
	while ((t==Type_Div) || (t==Type_Multiplic) ||(t==Type_Mod))
	{
		op2 = ElemVyrazh();

		if (t == Type_Multiplic)
			{
				op1 = pt -> performOperation(op1, op2, multy);
			}
			else if (t == Type_Div)
			{
				op1 = pt -> performOperation(op1, op2, divide);
			}
			else
			{
				op1 = pt -> performOperation(op1, op2, percent);
			}

		uk1=sc->GetUK(); t=sc->Scaner(l);
		str=sc->GetStroka();
		simv=sc->GetSimvol();
	}
	sc->PutUK(uk1);
	sc->PutStroka(str);
	sc->PutSimvol(simv);

	return op1;
}

//в выражение передает ук-ль на символ после (, после работы выр-ия ожидает ) (вроде возвращает)
//считывает указ-ль перед началом (сам берет след символ после переданного) и ничего не сохраняет после работы
//робит
TData * TDiagram::ElemVyrazh(){ //элементарное выражение

	//        _______ идентификатор ________
	// ______|_____ символьная const _______|________
	//       |___ целая числовая const _____|
	//       |__ веществ.экспоненциальная __|
	//       |__ ( ___ |выражение| ___ ) ___|

	TypeLex l, l1; 
	int t, uk1, uk2, t1, str, simv;

	/*char typebuf[MAX_LEX];
	typebuf[0] = '\0';*/

	TData * data = new TData();

	Tree * ftr;

	uk1=sc->GetUK(); t=sc->Scaner(l);
	str=sc->GetStroka();
	simv=sc->GetSimvol();

	if (t==Type_Const){  
		data -> dataType = type_int;
		data -> dataValue.isInt = atoi(l);
		return data;
	}

	else if (t == Type_Const_Simb)
	{
		data -> dataType = type_char;
		data -> dataValue.isChar = l[0];
		return data;
	}

	if (t==Type_Const_E) {
		data -> dataType = type_double;
		//data -> dataValue.isDouble = atof(l);
		sscanf_s(l, "%lf", &(data -> dataValue.isDouble));
		return data;
	}

	if (t==Type_Id) {
		
		ftr = pt -> sem_var_declared(l);
		if (pt -> sem_check_func(l))
		{
			sc -> PrintError("Это функция!", l);
		}
		else
		{
			///uk2 = sc -> GetUK();
			//t1 = sc -> Scaner(l1);
//			sc -> PutUK(uk2);
			data -> dataType = ftr -> n -> type;
			if (data -> dataType == type_int)
				data -> dataValue.isInt = ftr -> n -> dataValue.isInt;
			else if (data -> dataType == type_double)
				data -> dataValue.isDouble = ftr -> n -> dataValue.isDouble;
			return data;
		}
		
	} 

	if (t==Type_Skobka_Op)	{
		uk1=sc->GetUK();
		str=sc->GetStroka();
		simv=sc->GetSimvol();
		t=sc->Scaner(l); 
		sc->PutUK(uk1);
		sc->PutStroka(str);
		sc->PutSimvol(simv);

		data = Vyrazh(); 
		t=sc->Scaner(l);
		if (t!=Type_Skobka_Cl) sc->PrintError("ожидался символ )",l);

		return data;
	}
	else sc->PrintError("Неправильный синтаксис (элемент. выражение)",l);

}

// робит с пустым блоком !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//считывает указ-ль перед началом и сохраняет после( уходит на блок, так что сохранение символа будет в нем
void TDiagram::Func(){ //функция

	// --- void --- идентификатор --- ( -- |список форм.переменных| -- ) --- |блок| ---

	/*TypeLex l; int t, uk1, str, simv;
	int i;

	DataType type;

	Tree * pt1 = NULL;
	FlInt = 0;

	char// typebuf[MAX_LEX], 
		idbuf[MAX_LEX];

	uk1=sc->GetUK();
		str=sc->GetStroka();
	simv=sc->GetSimvol();

	t=sc->Scaner(l);
	
	idbuf[0] = '\0';
	

	if (t!=Type_Void) sc->PrintError("ожидался тип void",l);

	type = type_void;

	t=sc->Scaner(l);	

	if (t!=Type_Id) sc->PrintError("ожидался идентификатор",l);
	
	strcpy_s(idbuf, l);

	if (!pt -> sem_override(l))
	{
		pt -> set_Left(new Node(l, type, true));
		pt = pt -> Left;
		pr = pt;
		pt1 = pt;
	}


	t=sc->Scaner(l);
	if (t!=Type_Skobka_Op) sc->PrintError("ожидался символ (",l);

	//добавление пар-ров функции в ( )
	Tree * pt_old = pt;
	pt -> set_Right(new Node());
	pt = pt -> Right;

	uk1=sc->GetUK();
		str=sc->GetStroka();
	simv=sc->GetSimvol();
	t=sc->Scaner(l); 
	sc->PutUK(uk1);
	sc->PutStroka(str);
							sc->PutSimvol(simv);

	i = SpisokFormPerem(); // нужно чтобы возвращал указатель на скобку )

	

	uk1=sc->GetUK();
		str=sc->GetStroka();
	simv=sc->GetSimvol();
	t=sc->Scaner(l); 
	pt_old -> SemSetParam(idbuf, i);
	if (t!=Type_Skobka_Cl) sc->PrintError("ожидался символ )",l);
	//pt_old -> SemSetParam(idbuf, i);
	
	pt -> set_Left(new Node());
	pt = pt -> Left;
	t = sc -> Scaner(l);

	if (t != Type_Open_Blok)
		sc -> PrintError("Ожидался символ {", l);

	pt -> set_Right(new Node());
	pt = pt -> Right;
	pt -> n -> position = sc -> GetUK();	//запомнить указатель в теле функции

	BodyBlock();

	uk1 = sc -> GetUK();
	t = sc -> Scaner(l);

	if (t != Type_Close_Blok)
		sc -> PrintError("Ожидался символ }", l);
	

	pt = pt_old;*/

	
	TypeLex l;
	int t, uk1, str, simv;
	t = sc -> Scaner(l);
	DataType type;
	Tree * pt1 = NULL;
	FlInt = 0;
	char idbuf[MAX_LEX];
	idbuf[0] = '\0';
	
		type = type_void;
	
	t = sc -> Scaner(l);
	strcpy_s(idbuf, l);
	if (!pt -> sem_override(l))
	{
		pt -> set_Left(new Node(l, type, true));
		pt = pt -> Left;
		pr = pt;
		pt1 = pt;
	}
	int i = 0;
	uk1 = sc -> GetUK();
	str=sc->GetStroka();
	simv=sc->GetSimvol();
	t = sc -> Scaner(l);
	if (t != Type_Skobka_Op)
		sc -> PrintError("Ожидался символ (", l);
	Tree * pt_old = pt;
	pt -> set_Right(new Node());
	pt = pt -> Right;
	uk1 = sc -> GetUK();
	str=sc->GetStroka();
	simv=sc->GetSimvol();
	t = sc -> Scaner(l);
	if (t != Type_Skobka_Cl)
	{
		sc -> PutUK(uk1);
		sc->PutStroka(str);
		sc->PutSimvol(simv);

		i = SpisokFormPerem();

		t = sc -> Scaner(l);
	}
	pt_old -> SemSetParam(idbuf, i);
	if (t != Type_Skobka_Cl)
		sc -> PrintError("Ожидался символ )", l);
	pt -> set_Left(new Node());
	pt = pt -> Left;
	t = sc -> Scaner(l);
	if (t != Type_Open_Blok)
		sc -> PrintError("Ожидался символ {", l);
	pt -> set_Right(new Node());
	pt = pt -> Right;
	pt -> n -> position = sc -> GetUK();	//запомнить указатель в теле функции
	BodyFunc();
	uk1 = sc -> GetUK();
	str=sc->GetStroka();
	simv=sc->GetSimvol();
	t = sc -> Scaner(l);
	if (t != Type_Close_Blok)
		sc -> PrintError("Ожидался символ }", l);
	pt = pt_old;

}

void TDiagram::BodyFunc()
{
	/*TypeLex l;
	int t, uk1, str, simv;
	TData * op = NULL;
	bool localFlInt = false;
	uk1 = sc -> GetUK();
	str=sc->GetStroka();
	simv=sc->GetSimvol();
	t = sc -> Scaner(l);
	sc -> PutUK(uk1);
	sc->PutStroka(str);
	sc->PutSimvol(simv);
	while (t != Type_Close_Blok)
	{
		if (t == Type_Int || t == Type_Double)
			OpisPerem();
		else
			Operat();
		uk1 = sc -> GetUK();
		str=sc->GetStroka();
		simv=sc->GetSimvol();
		t = sc -> Scaner(l);
		sc -> PutUK(uk1);
		sc->PutStroka(str);
		sc->PutSimvol(simv);
	*/
	/*	if (strcmp(l, "return") == 0)
			localFlInt = true;*/
//	}
	/*if (localFlInt)
	{
		if (op -> dataType = type_int)		
			pt -> n -> dataValue.isInt = op -> dataValue.isInt;
		else if (op -> dataType = type_double)		
			pt -> n -> dataValue.isDouble = op -> dataValue.isDouble;
	}*/
	//return op;


	TypeLex l;
	int t, uk1, str,simv;
	TData * op = NULL;
	bool localFlInt = false;
	uk1 = sc -> GetUK();
	str=sc->GetStroka();
	simv=sc->GetSimvol();

	t = sc -> Scaner(l);

	sc -> PutUK(uk1);
	sc->PutStroka(str);
	sc->PutSimvol(simv);

	while (t != Type_Close_Blok)
	{
		if (t == Type_Int || t == Type_Double)
			OpisPerem();
		else
			 Operat();

		uk1 = sc -> GetUK();
		str=sc->GetStroka();
		simv=sc->GetSimvol();

		t = sc -> Scaner(l);
		
		sc -> PutUK(uk1);
		sc->PutStroka(str);
		sc->PutSimvol(simv);
		//if (strcmp(l, "return") == 0)
		//	localFlInt = true;
	}
}


// робит, считывает указ-ль перед началом и сохраняет после( будет сохранять скобку)
int TDiagram::SpisokFormPerem(){ //список формальныхх переменных

	//         __________________ , _________________
	//        |    __ int ___                        |
	//_______\|/__|          |____ идентификатор ____|__
	//            |_ double _|

/*	TypeLex l; 
	int t,uk1, str, simv;

	DataType type;

	char typebuf[MAX_LEX];
	typebuf[0] = '\0';
	int i=0;
	uk1=sc->GetUK();
		str=sc->GetStroka();
	simv=sc->GetSimvol();
		
	do {
		
		//sc->PutUK(uk1);
		t=sc->Scaner(l);
		if ( (t!=Type_Int) && (t!=Type_Double) )
			sc->PrintError("ожидался тип данных int или double",l);

		if(t==Type_Int)
			type = type_int;
		else
			type = type_double;

		t=sc->Scaner(l);
		if(t!=Type_Id) sc->PrintError("ожидался идентификатор",l);

		//добавить параметр в ф-ю
		if (! pt -> sem_override(l))
			pt = pt -> sem_add_var(l, type);

		uk1=sc->GetUK(); t=sc->Scaner(l);
			str=sc->GetStroka();
	simv=sc->GetSimvol();

	i++;
	} while (t==Type_Zapyat);

	sc->PutUK(uk1);
	sc->PutStroka(str);
	sc->PutSimvol(simv);
	
	return i;*/


	TypeLex l;
	int t, uk1,  str, simv;
	int i = 0;
	DataType type;
	uk1 = sc -> GetUK();	

	str=sc->GetStroka();
	simv=sc->GetSimvol();

	do{		
		t = sc -> Scaner(l);
		if (t == Type_Int)
			type = type_int;
		else if (t == Type_Double)
			type = type_double;
		else
			sc -> PrintError("Ожидался тип (int или double)", l);
		t = sc -> Scaner(l);
		if (t != Type_Id)
			sc -> PrintError("Ожидался идентификатор", l);
		if (! pt -> sem_override(l))
			pt = pt -> sem_add_var(l, type);
		uk1 = sc -> GetUK();
		t = sc -> Scaner(l);
		i++;
	}
	while(t == Type_Zapyat);
	sc -> PutUK(uk1);
	sc->PutStroka(str);
	sc->PutSimvol(simv);
	return i;

	
}

// не считывает ук-ль, перед вызовом выражения сохраняем указатель
//робит
/*TData * */ void TDiagram::Prisvaiv(){ //присваивание

	// ------ идентификатор ----- = ------ |выражение| ---- ; -

	TypeLex l; int t, uk1, str, simv;

	char typebuf[MAX_LEX];
	typebuf[0] = '\0';


	t=sc->Scaner(l);

	Tree * tree = pt -> sem_var_declared(l);

	if (t!=Type_Id) sc->PrintError("ожидался идентификатор",l);
	if (pt -> sem_check_func(l))
		{
			sc -> PrintError("Функции присваивать нельзя!", sc -> uk);
		}
	t=sc->Scaner(l);
	if (t!=Type_Equall) sc->PrintError("ожидался знак =",l);
	uk1=sc->GetUK();
		str=sc->GetStroka();
	simv=sc->GetSimvol();
	t=sc->Scaner(l); 
	sc->PutUK(uk1); 
	sc->PutStroka(str);
		sc->PutSimvol(simv);
		
		TData * assign = Vyrazh();

		if (tree -> n -> type == type_double && assign -> dataType == type_int)
		{
			tree -> n -> dataValue.isDouble = assign -> dataValue.isInt;
			//tree -> n -> type = type_double;
			if (FlInt)
			cout<<tree -> n-> id<<" = "<<tree->n->dataValue.isDouble<<endl;
		}
		else
		if (assign -> dataType == type_int)
		{
			tree -> n -> dataValue.isInt = assign -> dataValue.isInt;
			if (FlInt)
			cout<<tree -> n-> id<<" = "<<tree->n->dataValue.isInt<<endl;
			//tree -> n -> type = type_int;
		}
	
		else if (assign -> dataType == type_double)
		{
			tree -> n -> dataValue.isDouble = assign -> dataValue.isDouble;
			//tree -> n -> type = type_double;
			if (FlInt)
			cout<<tree -> n-> id<<" = "<<tree->n->dataValue.isDouble<<endl;
		}

	uk1=sc->GetUK();
		str=sc->GetStroka();
	simv=sc->GetSimvol();
	t=sc->Scaner(l); 
	if (t!=Type_End_Oper) sc->PrintError("ожидался символ ;",l);

//	return data;
	
}


void TDiagram::VyzovFunc(){ //вызов функции

	// ---- идентификатор --- ( --- |список фактических переменных| ---) --- ; ----
	
	TypeLex l; int t, uk1, str, simv;

//	int t, uk1;

	Tree * pt_old;
	Tree * tr1;
	TData * f;
	int position;
	
	int num = 0;
	char varname[MAX_LEX];
	varname[0] = '\0';

	t=sc->Scaner(l);
	if (t!=Type_Id) sc->PrintError("ожидался идентификатор",l);

	strcpy_s(varname, l);

	t=sc->Scaner(l);
	if (t!=Type_Skobka_Op) sc->PrintError("ожидался символ (",l);
	uk1=sc->GetUK();
	str=sc->GetStroka();
	simv=sc->GetSimvol();
		str=sc->GetStroka();
	simv=sc->GetSimvol();
	t=sc->Scaner(l); 
	sc->PutUK(uk1); 
	sc->PutStroka(str);
	sc->PutSimvol(simv);

	
	
	num = SpisokFactPerem(varname);

	//pt=pt_old;

	uk1=sc->GetUK();
		str=sc->GetStroka();
	simv=sc->GetSimvol();
	t=sc->Scaner(l); 
	if (t!=Type_Skobka_Cl) sc->PrintError("ожидался символ )",l);
	position = sc -> GetUK();

	//pt -> SemControlParam(varname, num);

	pt -> SemControlParam(varname, num);
	pt_old = pt;
	tr1 = pt -> sem_copy_tree_of_func(varname);
	Tree * zam;
	zam = pr;
	pr = pt -> sem_var_declared(varname);
	sc -> PutUK(tr1 -> n -> position);
	pt = tr1;
	//FlInt = 1;

	//t=sc->Scaner(l);
	/*Tree * pt_old = pt;
	pt -> set_Right(new Node());
	pt = pt -> Right;
	*/
	/////////
	//uk1=sc->GetUK();
		//str=sc->GetStroka();
	//simv=sc->GetSimvol();
	//t=sc->Scaner(l); 
	//t = sc -> Scaner(l);
	//if(t!=Type_Open_Blok) sc->PrintError("ожидался символ {",l);
/*	uk1=sc->GetUK();
		str=sc->GetStroka();
	simv=sc->GetSimvol();
	t=sc->Scaner(l);
	sc->PutUK(uk1);
	sc->PutStroka(str);
							sc->PutSimvol(simv);

	

	 BodyBlock();
	 //////
	 uk1=sc->GetUK();
	t=sc->Scaner(l);
		str=sc->GetStroka();
	simv=sc->GetSimvol();
	if(t!=Type_Close_Blok) sc->PrintError("ожидался символ }",l);*/
	//////
	BodyFunc();

	pt = pt_old;
	pr = zam;
	uk1 = position;
	sc -> PutUK(uk1);

	t=sc->Scaner(l);
	if (t!=Type_End_Oper) sc->PrintError("ожидался символ ;",l);

	pt -> SemControlParam(varname, num);
}

// робит
int TDiagram::SpisokFactPerem(char varname[]){ //список фактических переменных

	//          _________ , _________
	//         |                     |
	// _______\|/___ |выражение| ____|_______

	TypeLex l; int t,uk1, str,simv;
	TData * op2 = new TData();
	int num=0;

	uk1=sc->GetUK();
		str=sc->GetStroka();
	simv=sc->GetSimvol();
	Tree * tr1, *pt_old;

	pt_old = pt;

	pt = pt->find_Up(varname);

	tr1 = pt -> sem_get_first_perem_of_func(pt->n->id);
	//int i =0;
	pt = pt_old;
	do {
		

		// без проверки типа
		if(!num){
			op2 = Vyrazh();
			tr1->Left->n->dataValue = op2->dataValue;
			
			//t = sc -> Scaner(l);
		}

		uk1 = sc -> GetUK();
		str=sc->GetStroka();
		simv=sc->GetSimvol();
	/*	if(!i)
			t = sc -> Scaner(l);
*/
		
		
		if(num){
			op2 = Vyrazh();
			tr1->Left->n->dataValue = op2->dataValue;
			
		}
		t = sc -> Scaner(l);
	/*	sc -> PutUK(uk1);
		sc->PutStroka(str);
		sc->PutSimvol(simv);*/
		tr1=tr1->Left;
		num++;

	} while (t==Type_Zapyat);
	sc->PutUK(uk1+1);
	sc->PutStroka(str);
							sc->PutSimvol(simv);

							return num;
}


TData * znach = new TData();

void TDiagram::Switch_(){ //switch

	// --- switch ---- ( --- |выражение| --- ) --- { --- |case| --- } ----

	TypeLex l; int t,uk1, str, simv;
	t=sc->Scaner(l);
	if (t!=Type_Switch) sc->PrintError("ожидался switch",l);
	t=sc->Scaner(l);

	int flag_old = FlInt;
	if (t!=Type_Skobka_Op) sc->PrintError("ожидался символ (",l);
	uk1=sc->GetUK();
		str=sc->GetStroka();
	simv=sc->GetSimvol();
	t=sc->Scaner(l); 
	sc->PutUK(uk1); 
	sc->PutStroka(str);
	sc->PutSimvol(simv);
	
	

	znach = Vyrazh();

	uk1=sc->GetUK();
		str=sc->GetStroka();
	simv=sc->GetSimvol();
	t=sc->Scaner(l);
	if (t!=Type_Skobka_Cl) sc->PrintError("ожидался символ )",l);
	t=sc->Scaner(l);
	if (t!=Type_Open_Blok) sc->PrintError("ожидался символ {",l);
	uk1=sc->GetUK();
		str=sc->GetStroka();
	simv=sc->GetSimvol();
	t=sc->Scaner(l); 
	sc->PutUK(uk1); 
	sc->PutStroka(str);
							sc->PutSimvol(simv);
	FlInt = 1;

	Case_();

	FlInt = flag_old;

	uk1=sc->GetUK();
		str=sc->GetStroka();
	simv=sc->GetSimvol();
	t=sc->Scaner(l);
	if (t!=Type_Close_Blok) sc->PrintError("ожидался символ }",l);
}

void TDiagram::Case_(){ //case

	        
	//    __ ; _ break _ |оператор| _ : _ |выражение| _ case_                                                                      
	// _\|/ _________________________________________________|__ default _ : _ |оператор| _ break _ ; _________
	//                                                         |____________________________________________|
	                                   

	TypeLex l; int t, uk1, str, simv;

	t=sc->Scaner(l);
	FlInt = 0;
	bool localFlInt = FlInt;

	TData * zn = new TData();

	if(t==Type_Case){
		do{
			uk1=sc->GetUK();
				str=sc->GetStroka();
	simv=sc->GetSimvol();
			t=sc->Scaner(l); 
			sc->PutUK(uk1);
			sc->PutStroka(str);
							sc->PutSimvol(simv);

			zn = Vyrazh();

			uk1=sc->GetUK();
				str=sc->GetStroka();
	simv=sc->GetSimvol();
			t=sc->Scaner(l);
			if( localFlInt && FlInt){

				if(zn == znach)
					localFlInt = 1;

			if (t!=Type_2_Point) sc->PrintError("ожидался символ :",l);
			uk1=sc->GetUK();
				str=sc->GetStroka();
	simv=sc->GetSimvol();
			t=sc->Scaner(l); 
			sc->PutUK(uk1); 
			sc->PutStroka(str);
							sc->PutSimvol(simv);

			Operat();
			uk1=sc->GetUK();
				str=sc->GetStroka();
	simv=sc->GetSimvol();
			t=sc->Scaner(l);
			if (t!=Type_Break) sc->PrintError("ожидался break",l);
			t=sc->Scaner(l);
			if (t!=Type_End_Oper) sc->PrintError("ожидался символ ;",l);
			uk1=sc->GetUK(); t=sc->Scaner(l);
				str=sc->GetStroka();
	simv=sc->GetSimvol();
			}
		}while(t==Type_Case);
		sc->PutUK(uk1);
		sc->PutStroka(str);
							sc->PutSimvol(simv);
	}
	    uk1=sc->GetUK(); t=sc->Scaner(l);
			str=sc->GetStroka();
	simv=sc->GetSimvol();
		if(t==Type_Default){
			t=sc->Scaner(l);
			if (t!=Type_2_Point) sc->PrintError("ожидался символ :",l);
			uk1=sc->GetUK();
				str=sc->GetStroka();
	simv=sc->GetSimvol();
			t=sc->Scaner(l); 
			sc->PutUK(uk1); 
			sc->PutStroka(str);
							sc->PutSimvol(simv);
			Operat();
			uk1=sc->GetUK();
				str=sc->GetStroka();
	simv=sc->GetSimvol();
			t=sc->Scaner(l);
			if (t!=Type_Break) sc->PrintError("ожидался break",l);
			t=sc->Scaner(l);
			if (t!=Type_End_Oper) sc->PrintError("ожидался символ ;",l);
			uk1=sc->GetUK(); t=sc->Scaner(l);sc->PutUK(uk1);

				str=sc->GetStroka();
	simv=sc->GetSimvol();
	sc->PutStroka(str);
							sc->PutSimvol(simv);
		}
		//uk1=sc->GetUK();
		t=sc->Scaner(l);sc->PutUK(uk1);
		sc->PutStroka(str);
							sc->PutSimvol(simv);
		/*else{
			sc->PrintError("ожидался default или case",l);
		}*/
	/*do {
		t=sc->Scaner(l);
		if (t!=Type_Default) sc->PrintError("ожидался default",l);
		t=sc->Scaner(l);
		if (t!=Type_2_Point) sc->PrintError("ожидался символ :",l);
		Operat();
		t=sc->Scaner(l);
		if (t!=Type_End_Oper) sc->PrintError("ожидался символ ;",l);
		t=sc->Scaner(l);
		if (t!=Type_Break) sc->PrintError("ожидался break",l);
		t=sc->Scaner(l);
		if (t!=Type_End_Oper) sc->PrintError("ожидался символ ;",l);
		uk1=sc->GetUK(); t=sc->Scaner(l);
		if (t==Type_Case)
		{
			Vyrazh();
			t=sc->Scaner(l);
			if (t!=Type_2_Point) sc->PrintError("ожидался символ :",l);
			Operat();
			t=sc->Scaner(l);
			if (t!=Type_End_Oper) sc->PrintError("ожидался символ ;",l);
			t=sc->Scaner(l);
			if (t!=Type_Break) sc->PrintError("ожидался break",l);
			t=sc->Scaner(l);
			if (t!=Type_End_Oper) sc->PrintError("ожидался символ ;",l);
			uk1=sc->GetUK(); t=sc->Scaner(l);
		}
	} while(t==Type_Case);*/
	//sc->PutUK(uk1);
}

//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <math.h>
#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;

bool has_sin = false;
bool has_comma = false;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------


void __fastcall TForm1::ButtonEqualClick(TObject *Sender)
{
	AnsiString equation = Edit1->Text;
	if(!equation.IsEmpty()){

		AnsiString LastCh = AnsiLastChar(equation);
		if (has_comma) { equation += '0'; }
		if (has_sin) {
			if (LastCh == '(') {
					equation += '0';
			}
			equation += ')';
		}
		if (LastCh == '+' || LastCh == '-') equation += '0';
		else if(LastCh == '/' || LastCh == '*') equation += '1';

		// Вычисление синусов
		int temp=1;
		while (temp){
			temp = equation.AnsiPos("s");
			if (temp) {
				AnsiString prev_substr = equation.SubString(0, temp-1);
				AnsiString next_substr = equation.SubString(equation.AnsiPos(")")+1, equation.Length());
				double f; AnsiString number = equation.SubString(equation.AnsiPos("(")+1, equation.AnsiPos(")") - equation.AnsiPos("(")-1);
				if (TryStrToFloat(number, f)) {
					double s;
					if ((f/180==(int)f/180) && int(f)%180==0) s = 0;
					else s = sin(M_PI * f / 180);
					equation = prev_substr + s+ next_substr;
				}
			}
			has_sin = false;
			has_comma = false;
		}

		// Обработка минусов
		equation = StringReplace(equation, "--", "+", TReplaceFlags() << rfReplaceAll);
		for (temp = 2; temp <= equation.Length(); temp++) {
			if (equation[temp]=='-' && std::isdigit(equation[temp-1])) {
				equation = equation.SubString(0, temp-1) + "+-" + equation.SubString(temp+1, equation.Length());
				temp++;
			}
		}

		// Обработка операций
		AnsiString num_temp =""; char LastOper = '+';
		double S=0; double P=1;
		for (temp = 1; temp <= equation.Length(); temp++) {
			if (std::isdigit(equation[temp]) || equation[temp]=='-' || equation[temp]==',') {
				num_temp+= equation[temp];
			}
			else {
				if ((equation[temp]=='*' || equation[temp]=='/')&&LastOper == '+') {
					P=StrToFloat(num_temp)*P;
				}
				else if((equation[temp]=='*' || equation[temp]=='/')&&LastOper == '*'){
					P=StrToFloat(num_temp)*P;
				}
				else if((equation[temp]=='*' || equation[temp]=='/')&&LastOper == '/'){
					P=P/StrToFloat(num_temp);
				}
				else{
					if(LastOper == '*'){
						P=StrToFloat(num_temp)*P;
						S+=P;
						P=1;
					}
					else if(LastOper == '/'){
						P=P/StrToFloat(num_temp);
						S+=P;
						P=1;
					}
					else{
						S+=StrToFloat(num_temp);
					}
				}
				num_temp ="";
				LastOper = equation[temp];
			}
		}

		// Обработка последней операции
		if(LastOper == '*'){
			P=StrToFloat(num_temp)*P;
			S+=P;
			P=1;
		}
		else if(LastOper == '/'){
			P=P/StrToFloat(num_temp);
			S+=P;
			P=1;
		}
		else{
			S+=StrToFloat(num_temp);
		}

		Edit1->Text=S;
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::NumberPress(TObject *Sender)
{
	TButton * button = dynamic_cast<TButton*>(Sender);
	Edit1->Text+= button->Caption;
}
//---------------------------------------------------------------------------


void __fastcall TForm1::OperationPress(TObject *Sender)
{
	AnsiString equation = Edit1->Text;
	AnsiString LastCh = AnsiLastChar(equation);
	TButton * button = dynamic_cast<TButton*>(Sender);
	if (!equation.IsEmpty()) {
		if (LastCh == '+' || LastCh == '-' || LastCh == '/' || LastCh == '*') {
			equation.SetLength(equation.Length()-1);
			Edit1->Text=equation;
		}
		if (has_comma && equation[equation.Length()]==',') {
			equation += '0';
			Edit1->Text=equation;
			has_comma = false;
		}
		if (has_sin) {
			if (LastCh == '(') {
				if (button->Caption!='-') {
					equation += '0';
				}
				else{
					Edit1->Text+= button->Caption;
					return;
				}
			}
			equation += ')';
			has_sin = false;
			Edit1->Text=equation;
		}

		Edit1->Text+= button->Caption;
	}
	else {
		TButton * button = dynamic_cast<TButton*>(Sender);
		if (button->Caption == '-') {
			Edit1->Text+= "0-";
		}
	}

	if (has_comma) has_comma = false;


}
//---------------------------------------------------------------------------

void __fastcall TForm1::BackspacePress(TObject *Sender)
{
	AnsiString equation = Edit1->Text;
    AnsiString LastCh = AnsiLastChar(equation);
	if (LastCh==')') {
		has_sin = true;
	}
	if (LastCh==',') {
		has_comma = false;
	}
	equation.SetLength(equation.Length()-1);
	Edit1->Text=equation;

}
//---------------------------------------------------------------------------

void __fastcall TForm1::ClearAll(TObject *Sender)
{
	AnsiString equation = Edit1->Text;
	equation.SetLength(0);
	Edit1->Text=equation;
	has_sin = false;
	has_comma = false;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SinPress(TObject *Sender)
{
	AnsiString equation = Edit1->Text;
	AnsiString LastCh = AnsiLastChar(equation);
	if (equation.IsEmpty() || LastCh == '+' || LastCh == '-' || LastCh == '/' || LastCh == '*') {
		equation += "sin(";
		has_sin = true;
	}
	else{
		if (has_comma) {
			equation += "0*sin(";
			has_comma = false;
		}
		else if (has_sin) {
			if (LastCh == '(') {
				equation += '0';
			}
			equation += ")*sin(";
			Edit1->Text=equation;
		}
		else equation += "*sin(";
		has_sin = true;
	}
	Edit1->Text=equation;

}
//---------------------------------------------------------------------------

void __fastcall TForm1::CommaPress(TObject *Sender)
{
	AnsiString equation = Edit1->Text;
	AnsiString LastCh = AnsiLastChar(equation);
	if (!has_comma) {
		if (equation.IsEmpty() || LastCh == '+' || LastCh == '-' || LastCh == '/' || LastCh == '*'|| LastCh == '(') {
			equation += "0,";
			Edit1->Text=equation;
		}
		else if (LastCh != ','){
			equation += ',';
		}
		Edit1->Text=equation;
		has_comma = true;
	}

}
//---------------------------------------------------------------------------


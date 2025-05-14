//---------------------------------------------------------------------------
#include <vcl.h>
#include <math.h>
#include <stack>
#include <algorithm>
#pragma hdrstop
#include "Unit1.h"
#include <System.StrUtils.hpp>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;


//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner) : TForm(Owner) {
	fs.DecimalSeparator = '.'; // Инициализация в конструкторе
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ButtonEqualClick(TObject *Sender) {
	AnsiString expression = Edit1->Text;

	if (expression.IsEmpty()) return;

	// Замена запятых на точки перед обработкой
	expression = StringReplace(expression, ",", ".", TReplaceFlags() << rfReplaceAll);

    // Добавление закрывающих скобок
    int openBrackets = 0, closeBrackets = 0;
    for (int i = 1; i <= expression.Length(); ++i) {
        if (expression[i] == '(') openBrackets++;
        if (expression[i] == ')') closeBrackets++;
    }
    while (openBrackets > closeBrackets) {
        expression += ")";
        closeBrackets++;
    }

    // Обработка acos
    int posAc = 1;
    while (posAc) {
        posAc = expression.Pos("acos");
        if (posAc) {
            int endPos = PosEx(")", expression, posAc);
            if (!endPos) {
                expression += ")";
                endPos = expression.Length();
            }

            AnsiString argStr = expression.SubString(posAc + 5, endPos - (posAc + 5));
            double arg = StrToFloatDef(argStr, 0.0);

            if (arg < -1.0 || arg > 1.0) {
                ShowMessage("Ошибка: аргумент acos должен быть в [-1, 1]");
                return;
            }

            double result = acos(arg);
            expression = expression.SubString(1, posAc - 1) +
                       FloatToStr(result, fs) + // Используем TFormatSettings
					   expression.SubString(endPos + 1, expression.Length());
        }
	}

    std::stack<double> values;
    std::stack<char> ops;

    auto applyOp = [&values, &ops]() {
        if (values.size() < 2 || ops.empty()) return;

        double b = values.top(); values.pop();
        double a = values.top(); values.pop();
        char op = ops.top(); ops.pop();

		switch(op) {
            case '+': values.push(a + b); break;
            case '-': values.push(a - b); break;
            case '*': values.push(a * b); break;
            case '/':
                if (b == 0) {
                    ShowMessage("Ошибка: деление на ноль!");
                    values.push(0);
                    return;
                }
                values.push(a / b);
                break;
        }
	};

    for (int i = 1; i <= expression.Length(); ++i) {
        char c = expression[i];

        if (isdigit(c) || c == '.') {
            AnsiString numStr;
			while (i <= expression.Length() && (isdigit(expression[i]) || expression[i] == '.')) {
                numStr += expression[i++];
            }
            i--;
            values.push(StrToFloat(numStr));
        }
        else if (c == '(') {
            ops.push(c);
        }
        else if (c == ')') {
            while (!ops.empty() && ops.top() != '(') {
                applyOp();
            }
            if (!ops.empty()) ops.pop();
        }
        else if (c == '+' || c == '-' || c == '*' || c == '/') {
            while (!ops.empty() && (
			   ((c == '+' || c == '-') && (ops.top() == '*' || ops.top() == '/')) ||
			   ((c == '*' || c == '/') && (ops.top() == '*' || ops.top() == '/'))
			))  {
                applyOp();
            }
            ops.push(c);
        }
    }

    while (!ops.empty()) {
        applyOp();
    }

    if (!values.empty()) {
        Edit1->Text = FloatToStr(values.top(), fs); // Форматируем с fs
    } else {
        Edit1->Text = "Ошибка";
    }
    hasDecimalPoint = false;
}
//---------------------------------------------------------------------------

// Обработчик цифровых кнопок
void __fastcall TForm1::NumberPress(TObject *Sender)
{
    TButton *button = dynamic_cast<TButton*>(Sender);
    Edit1->Text += button->Caption;
}
//---------------------------------------------------------------------------

// Обработчик кнопки "."
void __fastcall TForm1::CommaPress(TObject *Sender) {
    if (!hasDecimalPoint) {
        Edit1->Text += ".";
        hasDecimalPoint = true;
    }
}
//---------------------------------------------------------------------------

// Обработчик операторов + - * /
void __fastcall TForm1::OperationPress(TObject *Sender)
{
    TButton *button = dynamic_cast<TButton*>(Sender);
    AnsiString expr = Edit1->Text;

    if (!expr.IsEmpty()) {
        // Получаем последний символ как AnsiString
        AnsiString lastChar = expr.SubString(expr.Length(), 1);

		// Сравниваем через оператор AnsiString
        if (lastChar == "+" ||
            lastChar == "-" ||
            lastChar == "*" ||
            lastChar == "/")
        {
            expr.SetLength(expr.Length() - 1);
        }
        Edit1->Text = expr + button->Caption;
    }
    hasDecimalPoint = false;
}
//---------------------------------------------------------------------------

// Обработчик кнопки "C" (очистка)
void __fastcall TForm1::ClearAll(TObject *Sender)
{
    Edit1->Text = "";
    hasDecimalPoint = false;
}
//---------------------------------------------------------------------------

// Обработчик кнопки "Backspace"
void __fastcall TForm1::BackspacePress(TObject *Sender)
{
    AnsiString expr = Edit1->Text;
    if (!expr.IsEmpty())
    {
        AnsiString lastChar = AnsiLastChar(expr);
		if (lastChar == "+" || lastChar == "-" || lastChar == "*" || lastChar == "/")
        expr.SetLength(expr.Length() - 1);
        Edit1->Text = expr;
    }
}
//---------------------------------------------------------------------------


void __fastcall TForm1::AcosPress(TObject *Sender) {
    AnsiString expr = Edit1->Text;
    AnsiString lastChar;

    if (!expr.IsEmpty()) {
        lastChar = expr.SubString(expr.Length(), 1); // Получаем последний символ
    }

    // Корректное сравнение через AnsiString
    if (expr.IsEmpty() ||
        lastChar == "+" ||
        lastChar == "-" ||
        lastChar == "*" ||
        lastChar == "/")
    {
        Edit1->Text = expr + "acos(";
    } else {
        Edit1->Text = expr + "*acos(";
    }
}

//---------------------------------------------------------------------------



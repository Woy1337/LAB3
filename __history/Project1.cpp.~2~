//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
//---------------------------------------------------------------------------
USEFORM("Unit1.cpp", Form1);
//---------------------------------------------------------------------------
int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	try
	{
		Application->Initialize();
		Application->MainFormOnTaskBar = true;
		Application->CreateForm(__classid(TForm1), &Form1);
		Application->Run();
	}
	catch (Exception &e)
	{
		Application->ShowException(&e);
	}
	catch (...)
	{
		try
		{
			throw Exception("Unknown exception occurred.");
		}
		catch (Exception &e)
		{
			Application->ShowException(&e);
		}
	}
	return 0;
}
//---------------------------------------------------------------------------

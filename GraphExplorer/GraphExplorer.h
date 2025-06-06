
// GraphExplorer.h: главный файл заголовка для приложения PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить pch.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// CGraphExplorerApp:
// Сведения о реализации этого класса: GraphExplorer.cpp
//

class CGraphExplorerApp : public CWinApp
{
public:
	CGraphExplorerApp();

// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация

	DECLARE_MESSAGE_MAP()
};

extern CGraphExplorerApp theApp;

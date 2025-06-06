#pragma once

#include "afxwin.h" // Для CEdit, CListBox

// Диалоговое окно CGraphExplorerDlg
class CGraphExplorerDlg : public CDialogEx
{
public:
    CGraphExplorerDlg(CWnd* pParent = nullptr); // стандартный конструктор

#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_GRAPHEXPLORER_DIALOG };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX); // поддержка DDX/DDV
    HICON m_hIcon;

    // Обработчики сообщений
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();

    afx_msg void OnBnClickedButtonLoad(); // обработчик кнопки "Загрузить граф"
    afx_msg void OnBnClickedButtonRun();  // обработчик кнопки "Выполнить обход"

    DECLARE_MESSAGE_MAP()

private:
    // Элементы управления
    CEdit m_editStartVertex;    // Поле ввода начальной вершины
    CListBox m_listResult;      // Список результатов BFS

    CStringA m_graphPath;       // Путь к файлу графа (ANSI строка)
};

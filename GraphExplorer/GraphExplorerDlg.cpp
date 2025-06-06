#include "pch.h"
#include "framework.h"
#include "GraphExplorer.h"
#include "GraphExplorerDlg.h"
#include "afxdialogex.h"
#include "Graph.h"
#include "Logger.h"
#include <fstream>
#include <atlbase.h>
#include <atlconv.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

class CAboutDlg : public CDialogEx
{
public:
    CAboutDlg();
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_ABOUTBOX };
#endif
protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX) {}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

CGraphExplorerDlg::CGraphExplorerDlg(CWnd* pParent)
    : CDialogEx(IDD_GRAPHEXPLORER_DIALOG, pParent)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGraphExplorerDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EDIT_START, m_editStartVertex);
    DDX_Control(pDX, IDC_LIST_RESULT, m_listResult);
}

BEGIN_MESSAGE_MAP(CGraphExplorerDlg, CDialogEx)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTON_LOAD, &CGraphExplorerDlg::OnBnClickedButtonLoad)
    ON_BN_CLICKED(IDC_BUTTON_RUN, &CGraphExplorerDlg::OnBnClickedButtonRun)
END_MESSAGE_MAP()

BOOL CGraphExplorerDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
    ASSERT(IDM_ABOUTBOX < 0xF000);
    CMenu* pSysMenu = GetSystemMenu(FALSE);
    if (pSysMenu != nullptr)
    {
        CString strAboutMenu;
        strAboutMenu.LoadString(IDS_ABOUTBOX);
        if (!strAboutMenu.IsEmpty())
        {
            pSysMenu->AppendMenu(MF_SEPARATOR);
            pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
        }
    }
    SetIcon(m_hIcon, TRUE);
    SetIcon(m_hIcon, FALSE);
    return TRUE;
}

void CGraphExplorerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    if ((nID & 0xFFF0) == IDM_ABOUTBOX)
    {
        CAboutDlg dlgAbout;
        dlgAbout.DoModal();
    }
    else
    {
        CDialogEx::OnSysCommand(nID, lParam);
    }
}

void CGraphExplorerDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this);
        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialogEx::OnPaint();
    }
}

HCURSOR CGraphExplorerDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}

void CGraphExplorerDlg::OnBnClickedButtonLoad()
{
    CFileDialog dlg(TRUE, _T("txt"), NULL, OFN_FILEMUSTEXIST, _T("Text Files (*.txt)|*.txt||"));
    if (dlg.DoModal() == IDOK)
    {
        CString filePath = dlg.GetPathName();
        m_graphPath = filePath;

        USES_CONVERSION;
        Logger::GetInstance().LogInfo(std::string(T2A(filePath)));

        MessageBox(_T("Файл графа загружен."), _T("Успешно"), MB_OK | MB_ICONINFORMATION);
    }
}

void CGraphExplorerDlg::OnBnClickedButtonRun()
{
    CString startStr;
    m_editStartVertex.GetWindowTextW(startStr);
    int startVertex = _ttoi(startStr);

    if (startStr.IsEmpty() || startVertex < 0)
    {
        MessageBox(_T("Укажите корректную начальную вершину."), _T("Ошибка"), MB_ICONERROR);
        return;
    }

    if (m_graphPath.IsEmpty())
    {
        MessageBox(_T("Сначала загрузите граф."), _T("Ошибка"), MB_ICONERROR);
        return;
    }

    try
    {
        Graph g;
        USES_CONVERSION;
        if (!g.LoadFromFile(std::string(T2A(m_graphPath))))
        {
            MessageBox(_T("Ошибка при загрузке графа."), _T("Ошибка"), MB_ICONERROR);
            return;
        }

        auto layers = g.BFS(startVertex);
        m_listResult.ResetContent();

        std::ofstream out("layers.txt");
        for (size_t i = 0; i < layers.size(); ++i)
        {
            CString layerLine;
            layerLine.Format(_T("Слой %zu:"), i);
            m_listResult.AddString(layerLine);

            Logger::GetInstance().LogInfo(std::string(T2A(layerLine)));

            out << "Layer " << i << ": ";
            for (const auto& v : layers[i])
            {
                CString vStr;
                vStr.Format(_T("%d "), v);
                m_listResult.AddString(_T("  ") + vStr);
                out << v << " ";
            }
            out << "\n";
        }

        Logger::GetInstance().LogInfo("BFS traversal complete.");
        MessageBox(_T("Граф обработан. Слои сохранены в layers.txt"), _T("Готово"), MB_OK);
    }
    catch (const std::exception& ex)
    {
        CString error;
        error.Format(_T("%hs"), ex.what());
        MessageBox(error, _T("Ошибка"), MB_ICONERROR);
    }
}
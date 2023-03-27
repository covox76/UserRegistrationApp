#include "pch.h"
#include "UserRegistrationApp.h"

#include "AppDefs.h"
#include "UserRegistrationDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CUserRegistrationApp, CWinApp)
  ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

CUserRegistrationApp::CUserRegistrationApp()
{
}

CUserRegistrationApp theApp;

BOOL CUserRegistrationApp::InitInstance()
{
  INITCOMMONCONTROLSEX InitCtrls {};

  InitCtrls.dwSize = sizeof(InitCtrls);
  InitCtrls.dwICC = ICC_WIN95_CLASSES;

  InitCommonControlsEx(&InitCtrls);

  CWinApp::InitInstance();

  if (!AfxSocketInit())
  {
    AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
    return FALSE;
  }

  auto shellManager = new CShellManager;

  CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

  SetRegistryKey(AppName);

  CUserRegistrationDlg dlg;
  m_pMainWnd = &dlg;
  auto response = dlg.DoModal();

  if (response == IDOK)
  {
  }
  else if (response == IDCANCEL)
  {
  }
  else if (response == -1)
  {
    TRACE(traceAppMsg, 0, "Warning: dialog creation failed, so application is terminating unexpectedly.\n");
    TRACE(traceAppMsg, 0, "Warning: if you are using MFC controls on the dialog, you cannot #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS.\n");
  }

  if (shellManager != nullptr)
  {
    delete shellManager;
  }

#if !defined(_AFXDLL) && !defined(_AFX_NO_MFC_CONTROLS_IN_DIALOGS)
  ControlBarCleanUp();
#endif

  return FALSE;
}


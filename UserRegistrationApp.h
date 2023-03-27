#pragma once

#ifndef __AFXWIN_H__
  #error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"

class CUserRegistrationApp : public CWinApp
{
public:
  CUserRegistrationApp();

public:
  BOOL InitInstance() override;

  DECLARE_MESSAGE_MAP()
};

extern CUserRegistrationApp theApp;

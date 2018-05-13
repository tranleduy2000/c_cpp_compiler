#include "callbacks.h"
#include "resource.h"

// Window procedure for our main window.
LRESULT CALLBACK MainWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  static HINSTANCE hInstance;

  switch (msg)
  {  
    case WM_COMMAND:
    {
      switch (LOWORD(wParam))
      {
        case ID_HELP_ABOUT:
        {
          DialogBox(hInstance, MAKEINTRESOURCE(IDD_ABOUTDIALOG), hWnd, &AboutDialogProc);
          return 0;
        }

        case ID_FILE_EXIT:
        {
          DestroyWindow(hWnd);
          return 0;
        }
      }
      break;
    }

    case WM_GETMINMAXINFO:
    {
      MINMAXINFO *minMax = (MINMAXINFO*) lParam;
      minMax->ptMinTrackSize.x = 220;
      minMax->ptMinTrackSize.y = 110;

      return 0;
    }

    case WM_SYSCOMMAND:
    {
      switch (LOWORD(wParam))
      {
        case ID_HELP_ABOUT:
        {
          DialogBox(hInstance, MAKEINTRESOURCE(IDD_ABOUTDIALOG), hWnd, &AboutDialogProc);
          return 0;
        }
      }
      break;
    }
    
    case WM_CREATE:
    {
      hInstance = ((LPCREATESTRUCT) lParam)->hInstance;
      return 0;
    }

    case WM_DESTROY:
    {
      PostQuitMessage(0);
      return 0;
    }
  }

  return DefWindowProc(hWnd, msg, wParam, lParam);
}


// Dialog procedure for our "about" dialog.
INT_PTR CALLBACK AboutDialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  switch (uMsg)
  {
    case WM_COMMAND:
    {
      switch (LOWORD(wParam))
      {
        case IDOK:
        case IDCANCEL:
        {
          EndDialog(hwndDlg, (INT_PTR) LOWORD(wParam));
          return (INT_PTR) TRUE;
        }
      }
      break;
    }

    case WM_INITDIALOG:
      return (INT_PTR) TRUE;
  }

  return (INT_PTR) FALSE;
}

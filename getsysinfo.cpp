#include <windows.h>
#include <tchar.h>
#include <stdio.h>

const TCHAR* envVarStrings[] =
{
  TEXT("OS         = %OS%"),
  TEXT("PATH       = %PATH%"),
  TEXT("HOMEPATH   = %HOMEPATH%"),
  TEXT("TEMP       = %TEMP%")
};
#define  ENV_VAR_STRING_COUNT  (sizeof(envVarStrings)/sizeof(TCHAR*))
#define INFO_BUFFER_SIZE 32767
TCHAR  infoBuf[INFO_BUFFER_SIZE] = {'\0'};
void printError(const TCHAR* msg );

void main( )
{
  DWORD i = 0;
  DWORD  bufCharCount = INFO_BUFFER_SIZE;
 
  // Get and display the name of the computer.
  if( !::GetComputerName( infoBuf, &bufCharCount ) )
    printError( TEXT("GetComputerName") ); 
  _tprintf( TEXT("\nComputer name:      %s"), infoBuf ); 
 
  // Get and display the user name. 
  bufCharCount = INFO_BUFFER_SIZE;
  if( !::GetUserName( infoBuf, &bufCharCount ) )
    printError( TEXT("GetUserName") ); 
  _tprintf( TEXT("\nUser name:          %s"), infoBuf ); 
 
  // Get and display the system directory. 
  if( !::GetSystemDirectory( infoBuf, INFO_BUFFER_SIZE ) )
    printError( TEXT("GetSystemDirectory") ); 
  _tprintf( TEXT("\nSystem Directory:   %s"), infoBuf ); 
 
  // Get and display the Windows directory. 
  if( !::GetWindowsDirectory( infoBuf, INFO_BUFFER_SIZE ) )
    printError( TEXT("GetWindowsDirectory") ); 
  _tprintf( TEXT("\nWindows Directory:  %s"), infoBuf ); 
 
  // Expand and display a few environment variables. 
  _tprintf( TEXT("\n\nSmall selection of Environment Variables:") ); 
  for( i = 0; i < ENV_VAR_STRING_COUNT; ++i )
  {
    bufCharCount = ::ExpandEnvironmentStrings(envVarStrings[i], infoBuf,
        INFO_BUFFER_SIZE ); 
    if( bufCharCount > INFO_BUFFER_SIZE )
      _tprintf( TEXT("\n\t(Buffer too small to expand: \"%s\")"), 
              envVarStrings[i] );
    else if( !bufCharCount )
      printError( TEXT("ExpandEnvironmentStrings") );
    else
      _tprintf( TEXT("\n   %s"), infoBuf );
  }
  _tprintf( TEXT("\n\n"));
}

void printError(const TCHAR* msg )
{
  TCHAR sysMsg[MAX_PATH] = {'\0'};
  TCHAR* p = sysMsg;
  DWORD eNum = ::GetLastError();

  ::FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM | 
         FORMAT_MESSAGE_IGNORE_INSERTS,
         nullptr, eNum,
         MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
         sysMsg, MAX_PATH, nullptr );

  // Trim the end of the line and terminate it with a null
  // 9 - \t (horizontal tab)
  // [0 - 32) - All characters in this area excepting 9
  // 46 - . (dot)
  while (*p++)
  {
      if ((*p != 9 && *p < 32) || *p == 46)
      {
          *p = 0;
          break;
      }
  }	

  // Display the message
  _tprintf( TEXT("\n\t%s failed with error %d (%s)"), msg, eNum, sysMsg );

  p = nullptr;
}

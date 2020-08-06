#include <iostream>
#include <windows.h>
#include <conio.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnarrowing"
auto BlockAPI (HANDLE hProcess, const char* libName, const char* apiName) -> DWORD {
   CHAR pReturn[]={0x31, 0xC0, // XOR eax, eax
                   0xC3}; // RET
   DWORD bReturn = 0;
   size_t* szReturn = 0;
 
   HINSTANCE hLibrary = LoadLibrary(libName);
   if (hLibrary) {
       VOID* pAddress = (VOID*)GetProcAddress (hLibrary, apiName);
       if (pAddress) {
           if (WriteProcessMemory (hProcess, (LPVOID)pAddress, (LPCVOID)pReturn, sizeof(pReturn), szReturn)) {
              if (szReturn) {
                 bReturn = 1;
              }
           }
       }
       FreeLibrary (hLibrary);
   }
   return bReturn;
}

#define VK_ENTER 13
int main(int argc, char** argv) {
	const char* library = "ntdll";
	const char* api = "LdrLoadDll";
	printf("writing bytes to (%s => %s)", library, api);	
	do {
		BlockAPI(GetCurrentProcess(), library, api);
		Sleep(200);
	} while (_getch() != VK_ENTER); // Break "do" loop on ENTER key
	return 0;
}

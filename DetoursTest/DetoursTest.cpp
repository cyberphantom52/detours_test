#include <iostream>
#include <Windows.h>

#include "detours.h"
#pragma comment(lib, "detours.lib")
using namespace std;

void (_stdcall *OldGetLocalTime)(LPSYSTEMTIME) = GetLocalTime;

void NewGetLocalTime(LPSYSTEMTIME lpSystemTime) {
	OldGetLocalTime(lpSystemTime);
	lpSystemTime->wMinute = 52;
}

void StartHook() {
	DetourTransactionBegin();

	DetourUpdateThread(GetCurrentThread());

	DetourAttach(&(PVOID&)OldGetLocalTime, NewGetLocalTime);

	DetourTransactionCommit();
}

void EndHook() {
	DetourTransactionBegin();

	DetourUpdateThread(GetCurrentThread());

	DetourAttach(&(PVOID&)OldGetLocalTime, NewGetLocalTime);

	DetourTransactionCommit();
}

int main() {
	SYSTEMTIME time, time2;
	GetLocalTime(&time);
	cout << time.wHour << ":" << time.wMinute << endl;
	StartHook();

	GetLocalTime(&time2);
	cout << time2.wHour << ":" << time2.wMinute << endl;

	EndHook();
	return 0;
}

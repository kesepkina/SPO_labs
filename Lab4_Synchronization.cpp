#include <iostream>
#include <windows.h>
#include <ctime>
using namespace std;

CRITICAL_SECTION criticalSection;
const int ARRAY_LENGTH = 20;
int customArray[ARRAY_LENGTH] = { 0 };

void runThread1()
{
	srand(time(0));

	while (true) {

		EnterCriticalSection(&criticalSection);

		for (int i = 0; i < ARRAY_LENGTH; i++) {
			customArray[i] = rand() % 301 - 150;
		}

		LeaveCriticalSection(&criticalSection);
		Sleep(10);
	}
}

void runThread2()
{
	while (true) {

		EnterCriticalSection(&criticalSection);

		for (int i = 0; i < ARRAY_LENGTH; i++) {
			if (customArray[i] % 2 == 0) {
				customArray[i] = 0;
			}
		}

		LeaveCriticalSection(&criticalSection);
		Sleep(10);
	}
}

void runThread3()
{
	while (true) {

		Sleep(1000);
		EnterCriticalSection(&criticalSection);

		for (int element : customArray) {
			cout << element << " ";
		}
		cout << endl;

		LeaveCriticalSection(&criticalSection);
	}
}

int main()
{
	cout << "Press enter to stop." << endl;

	InitializeCriticalSection(&criticalSection);

	HANDLE firstThread = CreateThread(NULL, 0, 
		(LPTHREAD_START_ROUTINE)runThread1, NULL, 0, NULL);

	if (firstThread == NULL) {
		return GetLastError();
	}

	HANDLE secondThread = CreateThread(NULL, 0,
		(LPTHREAD_START_ROUTINE)runThread2, NULL, 0, NULL);
	 
	if (secondThread == NULL) {
		return GetLastError();
	}

	HANDLE thirdThread = CreateThread(NULL, 0,
		(LPTHREAD_START_ROUTINE)runThread3, NULL, 0, NULL);

	if (thirdThread == NULL) {
		return GetLastError();
	}

	cin.get();
	DeleteCriticalSection(&criticalSection);

	CloseHandle(firstThread);
	CloseHandle(secondThread);
	CloseHandle(thirdThread);

	return 0;
}
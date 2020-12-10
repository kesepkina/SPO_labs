#include <iostream>
#include <windows.h>

using namespace std;

HANDLE semaphore;
const int ARRAY_LENGTH = 20;
int customArray[ARRAY_LENGTH];

int runThread1()
{
	srand(time(0));
	for (int i = 0; i < ARRAY_LENGTH; i++) {
		customArray[i] = rand() % 501;
		cout << endl << "array[" << i << "] = " << customArray[i] << endl;
		ReleaseSemaphore(semaphore, 1, NULL);
		Sleep(500);
	}
	return 0;
}

int runThread2()
{
	for (int i = 0; i < ARRAY_LENGTH; i++) {
		WaitForSingleObject(semaphore, INFINITE);
		if (customArray[i] % 2 == 1) {
			customArray[i] = 1000;
		}
		cout << "array[" << i << "] = " << customArray[i] << endl;
	}
	return 0;
}

int main() {

	semaphore = CreateSemaphore(NULL, 0, 21, "MySemaphore");
	if (semaphore == NULL) {
		cout << "Create semaphore failed" << endl;
		return GetLastError();
	}

	HANDLE firstThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)runThread1, NULL, 0, NULL);
	if (firstThread == NULL) {
		return GetLastError();
	}

	HANDLE secondThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)runThread2, NULL, 0, NULL);
	if (secondThread == NULL) {
		return GetLastError();
	}

	WaitForSingleObject(firstThread, INFINITE);
	WaitForSingleObject(secondThread, INFINITE);

	CloseHandle(semaphore);
	CloseHandle(firstThread);
	CloseHandle(secondThread);

	cout << endl << " Result:" << endl;
	for (int i = 0; i < ARRAY_LENGTH; i++) {
		cout << " " << customArray[i];
	}
	cout << endl;

	return 0;
}
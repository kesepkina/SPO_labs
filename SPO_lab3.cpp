#include <iostream>
#include <windows.h>

using namespace std;

int counter1 = 0, counter2=0, counter3=0;
HANDLE hTh01, hTh02, hTh03;
DWORD IDTh01, IDTh02, IDTh03;
char input;

void firstThread() 
{
	while (true) {
		counter1++;
		Sleep(10);
	}
}

void secondThread()
{
	while (true) {
		counter2++;
		Sleep(100);
	}
}

void thirdThread() 
{
	while (true) {
		counter3++;
		Sleep(1000);
	}
}

int threadWithLargestCounterValue(){
	int thread = 0;
	if (counter1 > counter2 && counter1 > counter3) {
	thread = 1;
	}
	else if (counter2 > counter1 && counter2 > counter3) {
		thread = 2;
	}
	else {
		thread = 3;
	}
	return thread;
}

int main()
{
	hTh01 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)firstThread, NULL, 0, &IDTh01);
	if (hTh01 == NULL) {
		return GetLastError();
	}

	hTh02 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)secondThread, NULL, 0, &IDTh02);
	if (hTh02 == NULL) {
		return GetLastError();
	}

	hTh03 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)thirdThread, NULL, 0, &IDTh03);
	if (hTh03 == NULL) {
		return GetLastError();
	}
	
	do
	{
		cout << "Enter :" << endl;
		cout << " 'q' - Exit" << endl;
		cout << " 'v' - Display number of thread with the largest counter value" << endl;
		cin >> input;

		if (input == 'v') {
			SuspendThread(hTh01);
			SuspendThread(hTh02);
			SuspendThread(hTh03);

			cout << threadWithLargestCounterValue() << ". thread has the largest counter value." << endl;
			
			ResumeThread(hTh01);
			ResumeThread(hTh02);
			ResumeThread(hTh03);
		}

	} while (input != 'q');

	TerminateThread(hTh01, 0);
	TerminateThread(hTh02, 0);
	TerminateThread(hTh03, 0);

	CloseHandle(hTh01);
	CloseHandle(hTh02);
	CloseHandle(hTh03);
}
#include <iostream> 
#include <windows.h> 
using namespace std;

const int ARRAY_LENGTH = 10;
const int RUN_TIMES = 3;
int customArray[ARRAY_LENGTH] = { 0 };

int runThread1()
{
	srand(time(0));
	HANDLE mutexHandle1 = OpenMutex(SYNCHRONIZE, FALSE, "MyMutex");

	if (mutexHandle1 == NULL) {
		cout << "Mutex opening in thread1 failed" << endl;
		return GetLastError();
	}

	for (int i = 0; i < RUN_TIMES; i++) {

		WaitForSingleObject(mutexHandle1, INFINITE);

		for (int i = 0; i < ARRAY_LENGTH; i++) {
			customArray[i] = rand() % 11;
		}


		cout << endl;
		for (int element : customArray) {
			cout << " " << element;
			Sleep(100);
		}
		cout << endl;

		ReleaseMutex(mutexHandle1);

	}

	CloseHandle(mutexHandle1);
	return 0;
}

int runThread2()
{
	Sleep(10);
	HANDLE mutexHandle2 = OpenMutex(SYNCHRONIZE, FALSE, "MyMutex");

	if (mutexHandle2 == NULL) {
		cout << "Mutex opening in thread2 failed" << endl;
		return GetLastError();
	}

	for (int i = 0; i < RUN_TIMES; i++) {

		WaitForSingleObject(mutexHandle2, INFINITE);
		int sum = 0;

		for (int i = 0; i < ARRAY_LENGTH - 1; i++) {
			sum += customArray[i];
		}
		customArray[ARRAY_LENGTH - 1] = sum;

		for (int element : customArray) {
			cout << " " << element;
			Sleep(100);
		}
		cout << endl;

		ReleaseMutex(mutexHandle2);

	}

	CloseHandle(mutexHandle2);
	return 0;
}

int main() {

	cout << "-- Press enter to stop --" << endl;

	HANDLE mutexHandle = CreateMutex(NULL, FALSE, "MyMutex");
	if (mutexHandle == NULL) {
		cout << "Mutex creating failed" << endl;
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

	cin.get();

	CloseHandle(mutexHandle);
	CloseHandle(firstThread);
	CloseHandle(secondThread);
	return 0;
}
#include<iostream>
#include<thread>
#include<mutex>
#include "windows.h"
#include <stdio.h>
#include <atomic>

using namespace std;
//
////std::mutex mtx;
//CRITICAL_SECTION CriticalSection;
//

struct Semaphore {
	int size;
	atomic<int> count;
	mutex updateMutex;

	Semaphore(int n) : size(n) { count.store(0); }

	void aquire() {
		while (1) {
			while (count >= size) {}
			updateMutex.lock();
			if (count >= size) {
				updateMutex.unlock();
				continue;
			}
			++count;
			updateMutex.unlock();
			break;

		}
	}

	void release() {
		updateMutex.lock();
		if (count > 0) {
			--count;
		} // else log err
		updateMutex.unlock();
	}
};


void increament(int& num, Semaphore& s)
{
	//mtx.lock();
	//EnterCriticalSection(&CriticalSection);
	s.aquire();
	for (int i = 0; i < 1000; i++)
	{
		num += 1;
		cout << num << endl;
	}
	//LeaveCriticalSection(&CriticalSection);
	//mtx.unlock();
	s.release();
}

void decrement(int& num, Semaphore& s)
{
	//mtx.lock();
	//EnterCriticalSection(&CriticalSection);
	s.aquire();
	for (int i = 1000; i > 0; i--)
	{
		num -= 1;
		cout << num << endl;
	}
	//LeaveCriticalSection(&CriticalSection);
	//mtx.unlock();
	s.release();
}



int main()
{
	Semaphore s(1);
	/*if (!InitializeCriticalSectionAndSpinCount(&CriticalSection,
		0x00000400))
		return 0;
	*/
	int num = 0;
	thread incThr(increament, ref(num), ref(s));
	thread decThr(decrement, ref(num), ref(s));
	incThr.join();
	decThr.join();
	cout << num << endl;

	/*DeleteCriticalSection(&CriticalSection);	*/
	system("pause");
	return 0;
}
// Author: UdbhavSharma

#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
using namespace std;

std::mutex producerMutex1, producerMutex2, coutMutex;
int x = 0, y = 0;

void doSomething() { std::this_thread::sleep_for(std::chrono::milliseconds(1000)); }
void increment(int &u, std::mutex &m, const char *desc)
{

	for (int i = 0; i < 5; i++)
	{
		m.lock();
		u++;
		coutMutex.lock();
		cout << desc << u << endl;
		coutMutex.unlock();
		m.unlock();
		doSomething();
	}
}
	void getSumOfXandY()
	{
		int count = 5;
		while (1)
		{
			int status = std::try_lock(producerMutex1, producerMutex2);
			if (status == -1)
			{
				if (x != 0 and y != 0)
				{
					count--;
					cout << "sum: " << x + y << endl;
					x = 0, y = 0;
				}
				producerMutex1.unlock();
				producerMutex2.unlock();
				if (count == 0)
					break;
			}
		}
	}
	int32_t main()
	{
		std::thread t1(increment, std::ref(x), std::ref(producerMutex1), "x ");
		std::thread t2(increment, std::ref(y), std::ref(producerMutex2), "y ");
		std::thread t3(getSumOfXandY);
		t1.join();
		t2.join();
		t3.join();
	}

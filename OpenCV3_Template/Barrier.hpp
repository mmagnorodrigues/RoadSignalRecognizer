#pragma once
#ifndef BARRIER
#define BARRIER

#include <vector>
#include <mutex>

class Barrier {
public:
	int assign(); //returns thread position in barrier
	void wait();
	void checkout(int threadPos);

private:
	std::vector<bool> statusVector;
	int numThreads;
	std::mutex assginLock;
	std::mutex checkoutLock;
};


#endif // !BARRIER


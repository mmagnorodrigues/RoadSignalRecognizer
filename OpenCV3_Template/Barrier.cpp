
#include "Barrier.hpp"
#include <iostream>
#include <mutex>

int Barrier::assign() {
	this->assginLock.lock();

	this->statusVector.push_back(false);
	int returnValue = this->numThreads;
	this->numThreads++;

	this->assginLock.unlock();

	return returnValue;
}

void Barrier::wait() {
	bool aguardar = true;
	while (aguardar)
	{
		for (int i = 0; i < this->numThreads; i++) {
			if (this->statusVector.at(i) == true) {
				aguardar = false;
			}
		}
	}
}

void Barrier::checkout(int threadPos) {
	this->checkoutLock.lock();
	this->statusVector.at(threadPos) = true;
	this->checkoutLock.unlock();
}




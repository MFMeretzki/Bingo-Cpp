// Implementation of the ConcurrentOutput class.

#include "ConcurrentOutput.h"
#include <iostream>
#include <string>
#include <mutex>
using namespace std;


std::mutex ConcurrentOutput::mut;


void ConcurrentOutput::Write(std::string s) {
	mut.lock();
		cout << s << endl;
	mut.unlock();
}
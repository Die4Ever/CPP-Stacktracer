
#include "stdafx.h"
#include <iostream>
#include <vector>
#include <string>
#include <math.h>
#include <algorithm>
#include <thread>
#include <mutex>
#include <sstream>
using namespace std;

#ifdef WIN32
#define __PRETTY_FUNCTION__ __FUNCTION__"()"
#define __builtin_FUNCTION() "?"
#endif

std::mutex debug_out_mutex;

template <class A>
string ToString(A a)
{
	std::ostringstream s;
	s << a;
	return s.str();
}

class _DEBUGOBJBASE
{
public:
	const char *File;
	const char *Func;
	int Line;
};

class _DEBUGOBJ : public _DEBUGOBJBASE
{
public:
	_DEBUGOBJ(const char *file, const char *func, int line);
	~_DEBUGOBJ();
};

thread_local vector<_DEBUGOBJBASE> stacktrace;

void PrintStackTrace(string pretext)
{
	debug_out_mutex.lock();
	cout << "\n" << pretext;
	cout << "\nStacktrace!=============\n";
	for (auto &d : stacktrace) {
		string file = d.File;
		auto fslash = file.find_last_of('/');
		auto bslash = file.find_last_of('\\');
		if (fslash == SIZE_MAX) fslash = 0;
		if (bslash == SIZE_MAX) bslash = 0;
		file = file.substr( std::max(fslash, bslash) );
		cout << file << ":" << d.Func << ":" << d.Line << "\n";
	}
	cout << "=============\n\n";
	cout.flush();
	debug_out_mutex.unlock();
}

_DEBUGOBJ::_DEBUGOBJ(const char *file, const char *func, int line)
{
	File = file;
	Func = func;
	Line = line;
	stacktrace.push_back(*this);
	PrintStackTrace("thread "+ToString(this_thread::get_id())+" entering");
}

_DEBUGOBJ::~_DEBUGOBJ()
{
	PrintStackTrace("thread " + ToString(this_thread::get_id()) + " exiting");
	if(stacktrace.size()>0) stacktrace.pop_back();
}

#ifdef _DEBUG
#define BEGINFUNC _DEBUGOBJ _debugobj = _DEBUGOBJ(__FILE__, __PRETTY_FUNCTION__, __LINE__)
#else
#define BEGINFUNC
#endif

int factorial(int n)
{
	BEGINFUNC;
	if (n <= 1) return 1;
	return n * factorial(n - 1);
}

int main()
{
	BEGINFUNC;
	vector<thread> threads;
	for (int i = 0;i < 100000000;i++) {//make sure it doesn't memory leak
		threads.emplace_back(thread(factorial, rand() % 5));
		if (threads.size() > 100) {
			for (auto &t : threads) t.join();
			threads.clear();
		}
	}
    return 0;
}


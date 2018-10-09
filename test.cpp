
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

enum class LogLevel { TRACE, INFO, DEBUG, WARN, ERROR, CRIT };
const LogLevel loglevel = LogLevel::DEBUG;

void PrintStackTrace(string pretext);

class _DEBUGOBJBASE
{
public:
	const char *File;
	const char *Func;
	int Line;
	string mess;

	void AddMess(LogLevel level, string s, int line)
	{
		const char* levels[] = { "TRACE", "INFO", "DEBUG", "WARN", "ERROR", "CRIT" };
		string newmess = string(levels[(int)level]) + ": " + s + " (line: "+ ToString(line) +")";
		if(level >= loglevel) PrintStackTrace(newmess);
		mess += "--" + newmess + "\n";
	}
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
	cout << "\nthread " + ToString(this_thread::get_id()) << ": " << pretext;
	cout << "\nStacktrace!=============\n";
	for (size_t n = stacktrace.size() - 1; n < stacktrace.size(); n--) {
		auto &d = stacktrace[n];
		string file = d.File;
		auto fslash = file.find_last_of('/');
		auto bslash = file.find_last_of('\\');
		if (fslash == SIZE_MAX) fslash = 0;
		if (bslash == SIZE_MAX) bslash = 0;
		file = file.substr(std::max(fslash, bslash));
		cout << file << ":" << d.Func << ":" << d.Line << "\n";
		if (d.mess.length()) cout << d.mess;
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
	if(LogLevel::TRACE >= loglevel) PrintStackTrace("entering");
}

_DEBUGOBJ::~_DEBUGOBJ()
{
	if (LogLevel::TRACE >= loglevel) PrintStackTrace("exiting");
	if (stacktrace.size() > 0) stacktrace.pop_back();
}

#ifdef _DEBUG
#define BEGINFUNC _DEBUGOBJ _debugobj = _DEBUGOBJ(__FILE__, __PRETTY_FUNCTION__, __LINE__)
#define LOG(level, message) (stacktrace.back().AddMess((level), (message), __LINE__))
#else
#define BEGINFUNC
#endif

int factorial(int n)
{
	BEGINFUNC;
	LOG(LogLevel::INFO, "factorial of "+ToString(n));
	if (n <= 1) {
		LOG(LogLevel::DEBUG, "returning 1");
		return 1;
	}
	int r = n * factorial(n - 1);
	LOG(LogLevel::DEBUG, "returning " + ToString(r));
	return r;
}

int main()
{
	BEGINFUNC;
	vector<thread> threads;
	for (int i = 0;i < 3;i++) {//make sure it doesn't memory leak
		LOG(LogLevel::DEBUG, "starting new thread");
		threads.emplace_back(thread(factorial, rand() % 5));
		if (threads.size() > 100) {
			for (auto &t : threads) t.join();
			threads.clear();
		}
	}
	for (auto &t : threads) t.join();
	threads.clear();
	return 0;
}


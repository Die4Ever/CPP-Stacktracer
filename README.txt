# CPP-Stacktracer
threadsafe stacktracer for C++


sample output
thread 20528: DEBUG: starting new thread (line: 120)
Stacktrace!=============
\test.cpp:main():117
=============


thread 20528: DEBUG: starting new thread (line: 120)
Stacktrace!=============
\test.cpp:main():117
--DEBUG: starting new thread (line: 120)
=============


thread 15988: DEBUG: returning 1 (line: 107)
Stacktrace!=============
\test.cpp:factorial():104
--INFO: factorial of 1 (line: 105)
=============


thread 20528: DEBUG: starting new thread (line: 120)
Stacktrace!=============
\test.cpp:main():117
--DEBUG: starting new thread (line: 120)
--DEBUG: starting new thread (line: 120)
=============


thread 1372: DEBUG: returning 1 (line: 107)
Stacktrace!=============
\test.cpp:factorial():104
--INFO: factorial of 1 (line: 105)
\test.cpp:factorial():104
--INFO: factorial of 2 (line: 105)
=============


thread 23548: DEBUG: returning 1 (line: 107)
Stacktrace!=============
\test.cpp:factorial():104
--INFO: factorial of 1 (line: 105)
\test.cpp:factorial():104
--INFO: factorial of 2 (line: 105)
\test.cpp:factorial():104
--INFO: factorial of 3 (line: 105)
\test.cpp:factorial():104
--INFO: factorial of 4 (line: 105)
=============


thread 1372: DEBUG: returning 2 (line: 111)
Stacktrace!=============
\test.cpp:factorial():104
--INFO: factorial of 2 (line: 105)
=============


thread 23548: DEBUG: returning 2 (line: 111)
Stacktrace!=============
\test.cpp:factorial():104
--INFO: factorial of 2 (line: 105)
\test.cpp:factorial():104
--INFO: factorial of 3 (line: 105)
\test.cpp:factorial():104
--INFO: factorial of 4 (line: 105)
=============


thread 23548: DEBUG: returning 6 (line: 111)
Stacktrace!=============
\test.cpp:factorial():104
--INFO: factorial of 3 (line: 105)
\test.cpp:factorial():104
--INFO: factorial of 4 (line: 105)
=============


thread 23548: DEBUG: returning 24 (line: 111)
Stacktrace!=============
\test.cpp:factorial():104
--INFO: factorial of 4 (line: 105)
=============

Press any key to continue . . .

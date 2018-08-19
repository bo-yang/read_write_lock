# read_write_lock

A simple C++11/C++14 read-write lock(write-biased). Since C++17, [std::shared_mutex](https://en.cppreference.com/w/cpp/thread/shared_mutex) is introduced, which is the standard multiple-read and single-write lock.

The read-write lock is defined in `readwritelock.hh`, while `readwritelock_test.cc` implements the producer-consumer(and watcher) problem as the test case. To try out the example, build the program as

```
g++ -Wall -std=c++11 readwritelock_test.cc -o readwritelock_test
./readwritelock_test
```

Following is a sample output - producers produce & save the numbers into the back of buffer, while the consumers consume the numbers from the front of the buffer. Watchers print the sanpshot of the buffer when it get a chance.

```
bobyan$ ./readwritelock_test
20 produced by 0x700001b28000
40 produced by 0x700001bab000
0 produced by 0x700001aa5000
80 produced by 0x700001cb1000
60 produced by 0x700001c2e000
20 consumed by 0x700001d34000
40 consumed by 0x700001db7000
0 consumed by 0x700001e3a000
61 produced by 0x700001c2e000
80 consumed by 0x700001db7000
60 consumed by 0x700001d34000
61 consumed by 0x700001e3a000
41 produced by 0x700001bab000
81 produced by 0x700001cb1000
1 produced by 0x700001aa5000
21 produced by 0x700001b28000
2 produced by 0x700001aa5000
41 consumed by 0x700001db7000
81 consumed by 0x700001d34000
22 produced by 0x700001b28000
1 consumed by 0x700001db7000
21 consumed by 0x700001e3a000
2 consumed by 0x700001d34000
23 produced by 0x700001b28000
82 produced by 0x700001cb1000
62 produced by 0x700001c2e000
24 produced by 0x700001b28000
25 produced by 0x700001b28000
83 produced by 0x700001cb1000
42 produced by 0x700001bab000
22 consumed by 0x700001d34000
23 consumed by 0x700001e3a000
84 produced by 0x700001cb1000
26 produced by 0x700001b28000
27 produced by 0x700001b28000
43 produced by 0x700001bab000
82 consumed by 0x700001db7000
3 produced by 0x700001aa5000
85 produced by 0x700001cb1000
62 consumed by 0x700001db7000
28 produced by 0x700001b28000
63 produced by 0x700001c2e000
24 consumed by 0x700001d34000
86 produced by 0x700001cb1000
25 consumed by 0x700001e3a000
83 consumed by 0x700001d34000
44 produced by 0x700001bab000
87 produced by 0x700001cb1000
0x700001f40000: total_cargo = 30
42 84 26 27 43 3 85 28 63 86 44 87 
4 produced by 0x700001aa5000
64 produced by 0x700001c2e000
42 consumed by 0x700001d34000
84 consumed by 0x700001db7000
29 produced by 0x700001b28000
65 produced by 0x700001c2e000
45 produced by 0x700001bab000
88 produced by 0x700001cb1000
30 produced by 0x700001b28000
46 produced by 0x700001bab000
26 consumed by 0x700001e3a000
5 produced by 0x700001aa5000
27 consumed by 0x700001d34000
43 consumed by 0x700001db7000
31 produced by 0x700001b28000
......
```

### References
- https://stackoverflow.com/questions/12033188/how-would-you-implement-your-own-reader-writer-lock-in-c11
- https://en.cppreference.com/w/cpp/thread/shared_mutex
- https://en.wikipedia.org/wiki/Readers%E2%80%93writer_lock

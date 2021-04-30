# 服务器信息
- 两台 4核4G 的Centos 虚拟机进行测试

# 无参数
## 单线程
### 短链接
```
[root@192 ~]# ab -n 400000 -c 1000 http://192.168.1.128/
This is ApacheBench, Version 2.3 <$Revision: 1430300 $>
Copyright 1996 Adam Twiss, Zeus Technology Ltd, http://www.zeustech.net/
Licensed to The Apache Software Foundation, http://www.apache.org/

Benchmarking 192.168.1.128 (be patient)
Completed 40000 requests
Completed 80000 requests
Completed 120000 requests
Completed 160000 requests
Completed 200000 requests
Completed 240000 requests
Completed 280000 requests
Completed 320000 requests
Completed 360000 requests
Completed 400000 requests
Finished 400000 requests


Server Software:        WEBD
Server Hostname:        192.168.1.128
Server Port:            80

Document Path:          /
Document Length:        703 bytes

Concurrency Level:      1000
Time taken for tests:   99.853 seconds
Complete requests:      400000
Failed requests:        0
Write errors:           0
Total transferred:      326800000 bytes
HTML transferred:       281200000 bytes
Requests per second:    4005.89 [#/sec] (mean)
Time per request:       249.632 [ms] (mean)
Time per request:       0.250 [ms] (mean, across all concurrent requests)
Transfer rate:          3196.11 [Kbytes/sec] received

Connection Times (ms)
              min  mean[+/-sd] median   max
Connect:        0  186 677.5      6   31088
Processing:     3   59 127.1     31   13357
Waiting:        1   56 126.6     29   13336
Total:          4  245 697.4     46   31345

Percentage of the requests served within a certain time (ms)
  50%     46
  66%     59
  75%     73
  80%     88
  90%   1034
  95%   1079
  98%   3032
  99%   3071
 100%  31345 (longest request)
```
### 长连接
```
[root@192 ~]# ab -k -n 400000 -c 1000 http://192.168.1.128/
This is ApacheBench, Version 2.3 <$Revision: 1430300 $>
Copyright 1996 Adam Twiss, Zeus Technology Ltd, http://www.zeustech.net/
Licensed to The Apache Software Foundation, http://www.apache.org/

Benchmarking 192.168.1.128 (be patient)
Completed 40000 requests
Completed 80000 requests
Completed 120000 requests
Completed 160000 requests
Completed 200000 requests
Completed 240000 requests
Completed 280000 requests
Completed 320000 requests
Completed 360000 requests
Completed 400000 requests
Finished 400000 requests


Server Software:        WEBD
Server Hostname:        192.168.1.128
Server Port:            80

Document Path:          /
Document Length:        703 bytes

Concurrency Level:      1000
Time taken for tests:   98.793 seconds
Complete requests:      400000
Failed requests:        32
   (Connect: 0, Receive: 0, Length: 32, Exceptions: 0)
Write errors:           0
Non-2xx responses:      32
Keep-Alive requests:    0
Total transferred:      326784704 bytes
HTML transferred:       281183168 bytes
Requests per second:    4048.88 [#/sec] (mean)
Time per request:       246.982 [ms] (mean)
Time per request:       0.247 [ms] (mean, across all concurrent requests)
Transfer rate:          3230.26 [Kbytes/sec] received

Connection Times (ms)
              min  mean[+/-sd] median   max
Connect:        0  180 656.6      6   31091
Processing:     1   62 129.1     29    3593
Waiting:        1   59 128.5     28    3593
Total:          2  241 680.5     42   31323

Percentage of the requests served within a certain time (ms)
  50%     42
  66%     59
  75%     74
  80%     90
  90%   1031
  95%   1078
  98%   3030
  99%   3070
 100%  31323 (longest request)
```

## 多线程(4个线程)
### 短连接
```
[root@192 ~]# ab -n 400000 -c 1000 http://192.168.1.128/
This is ApacheBench, Version 2.3 <$Revision: 1430300 $>
Copyright 1996 Adam Twiss, Zeus Technology Ltd, http://www.zeustech.net/
Licensed to The Apache Software Foundation, http://www.apache.org/

Benchmarking 192.168.1.128 (be patient)
Completed 40000 requests
Completed 80000 requests
Completed 120000 requests
Completed 160000 requests
Completed 200000 requests
Completed 240000 requests
Completed 280000 requests
Completed 320000 requests
Completed 360000 requests
Completed 400000 requests
Finished 400000 requests


Server Software:        WEBD
Server Hostname:        192.168.1.128
Server Port:            80

Document Path:          /
Document Length:        703 bytes

Concurrency Level:      1000
Time taken for tests:   105.211 seconds
Complete requests:      400000
Failed requests:        0
Write errors:           0
Total transferred:      326800000 bytes
HTML transferred:       281200000 bytes
Requests per second:    3801.90 [#/sec] (mean)
Time per request:       263.027 [ms] (mean)
Time per request:       0.263 [ms] (mean, across all concurrent requests)
Transfer rate:          3033.35 [Kbytes/sec] received

Connection Times (ms)
              min  mean[+/-sd] median   max
Connect:        0  196 768.9      4   63139
Processing:     6   62 137.4     30   12966
Waiting:        2   60 136.9     29   12964
Total:         14  258 791.8     42   63391

Percentage of the requests served within a certain time (ms)
  50%     42
  66%     56
  75%     67
  80%     84
  90%   1034
  95%   1087
  98%   3035
  99%   3075
 100%  63391 (longest request)
```
### 长连接
```
[root@192 ~]# ab -k -n 400000 -c 1000 http://192.168.1.128/
This is ApacheBench, Version 2.3 <$Revision: 1430300 $>
Copyright 1996 Adam Twiss, Zeus Technology Ltd, http://www.zeustech.net/
Licensed to The Apache Software Foundation, http://www.apache.org/

Benchmarking 192.168.1.128 (be patient)
Completed 40000 requests
Completed 80000 requests
Completed 120000 requests
Completed 160000 requests
Completed 200000 requests
Completed 240000 requests
Completed 280000 requests
Completed 320000 requests
Completed 360000 requests
Completed 400000 requests
Finished 400000 requests


Server Software:        WEBD
Server Hostname:        192.168.1.128
Server Port:            80

Document Path:          /
Document Length:        703 bytes

Concurrency Level:      1000
Time taken for tests:   105.508 seconds
Complete requests:      400000
Failed requests:        419
   (Connect: 0, Receive: 0, Length: 419, Exceptions: 0)
Write errors:           0
Non-2xx responses:      419
Keep-Alive requests:    0
Total transferred:      326599718 bytes
HTML transferred:       280979606 bytes
Requests per second:    3791.19 [#/sec] (mean)
Time per request:       263.769 [ms] (mean)
Time per request:       0.264 [ms] (mean, across all concurrent requests)
Transfer rate:          3022.95 [Kbytes/sec] received

Connection Times (ms)
              min  mean[+/-sd] median   max
Connect:        0  198 794.8      4   31103
Processing:     5   63 128.7     31    7339
Waiting:        1   61 128.2     30    7337
Total:         15  260 814.9     44   31299

Percentage of the requests served within a certain time (ms)
  50%     44
  66%     57
  75%     68
  80%     85
  90%   1034
  95%   1082
  98%   3036
  99%   3080
 100%  31299 (longest request)
```

# 带参数
## 单线程
### GET请求
```
[root@192 ~]# ab  -n 10000 -c 1000 'http://192.168.1.128/get_show.html?user_name=%E5%B0%8F%E7%8E%8B&email=%E5%B9%BF%E4%B8%9C%E7%9C%81&phone=110119112'
This is ApacheBench, Version 2.3 <$Revision: 1430300 $>
Copyright 1996 Adam Twiss, Zeus Technology Ltd, http://www.zeustech.net/
Licensed to The Apache Software Foundation, http://www.apache.org/

Benchmarking 192.168.1.128 (be patient)
Completed 1000 requests
Completed 2000 requests
Completed 3000 requests
Completed 4000 requests
Completed 5000 requests
Completed 6000 requests
Completed 7000 requests
Completed 8000 requests
Completed 9000 requests
Completed 10000 requests
Finished 10000 requests


Server Software:        WEBD
Server Hostname:        192.168.1.128
Server Port:            80

Document Path:          /get_show.html?user_name=%E5%B0%8F%E7%8E%8B&email=%E5%B9%BF%E4%B8%9C%E7%9C%81&phone=110119112
Document Length:        2878 bytes

Concurrency Level:      1000
Time taken for tests:   53.174 seconds
Complete requests:      10000
Failed requests:        2178
   (Connect: 0, Receive: 0, Length: 2178, Exceptions: 0)
Write errors:           0
Non-2xx responses:      2178
Total transferred:      24141766 bytes
HTML transferred:       22897222 bytes
Requests per second:    188.06 [#/sec] (mean)
Time per request:       5317.433 [ms] (mean)
Time per request:       5.317 [ms] (mean, across all concurrent requests)
Transfer rate:          443.37 [Kbytes/sec] received

Connection Times (ms)
              min  mean[+/-sd] median   max
Connect:        0 1060 2166.3      4   15049
Processing:     0 1667 2349.3   1151   52864
Waiting:        0 1667 2349.4   1151   52864
Total:          1 2727 3324.5   1967   52871

Percentage of the requests served within a certain time (ms)
  50%   1967
```

### POST请求
```
[root@192 ~]#  ab -n 10000 -c 1000 -p postdata.txt -T application/x-www-form-urlencoded "http://192.168.1.128/post_show.html"
This is ApacheBench, Version 2.3 <$Revision: 1430300 $>
Copyright 1996 Adam Twiss, Zeus Technology Ltd, http://www.zeustech.net/
Licensed to The Apache Software Foundation, http://www.apache.org/

Benchmarking 192.168.1.128 (be patient)
Completed 1000 requests
Completed 2000 requests
Completed 3000 requests
Completed 4000 requests
Completed 5000 requests
Completed 6000 requests
Completed 7000 requests
Completed 8000 requests
Completed 9000 requests
Completed 10000 requests
Finished 10000 requests


Server Software:        WEBD
Server Hostname:        192.168.1.128
Server Port:            80

Document Path:          /post_show.html
Document Length:        2879 bytes

Concurrency Level:      1000
Time taken for tests:   50.793 seconds
Complete requests:      10000
Failed requests:        980
   (Connect: 0, Receive: 0, Length: 980, Exceptions: 0)
Write errors:           0
Non-2xx responses:      980
Total transferred:      27329080 bytes
Total body sent:        2440000
HTML transferred:       26142040 bytes
Requests per second:    196.88 [#/sec] (mean)
Time per request:       5079.272 [ms] (mean)
Time per request:       5.079 [ms] (mean, across all concurrent requests)
Transfer rate:          525.44 [Kbytes/sec] received
                        46.91 kb/s sent
                        572.35 kb/s total

Connection Times (ms)
              min  mean[+/-sd] median   max
Connect:        0  857 1737.3      1   15040
Processing:     1 1836 2015.5   1763   28978
Waiting:        1 1835 2016.7   1762   28978
Total:          1 2693 2761.1   1964   29995

Percentage of the requests served within a certain time (ms)
  50%   1964
  66%   3103
  75%   4003
  80%   4782
  90%   5753
  95%   7578
  98%   9238
  99%  12794
 100%  29995 (longest request)
```
### GET请求，不解析模板
```
[root@192 ~]#  ab -n 400000 -c 1000 'http://192.168.1.128/favicon.ico?user_name=%E5%B0%8F%E7%8E%8B&email=%E5%B9%BF%E4%B8%9C%E7%9C%81&phone=110119112'
This is ApacheBench, Version 2.3 <$Revision: 1430300 $>
Copyright 1996 Adam Twiss, Zeus Technology Ltd, http://www.zeustech.net/
Licensed to The Apache Software Foundation, http://www.apache.org/

Benchmarking 192.168.1.128 (be patient)
Completed 40000 requests
Completed 80000 requests
Completed 120000 requests
Completed 160000 requests
Completed 200000 requests
Completed 240000 requests
Completed 280000 requests
Completed 320000 requests
Completed 360000 requests
Completed 400000 requests
Finished 400000 requests


Server Software:        WEBD
Server Hostname:        192.168.1.128
Server Port:            80

Document Path:          /favicon.ico?user_name=%E5%B0%8F%E7%8E%8B&email=%E5%B9%BF%E4%B8%9C%E7%9C%81&phone=110119112
Document Length:        4286 bytes

Concurrency Level:      1000
Time taken for tests:   106.706 seconds
Complete requests:      400000
Failed requests:        400
   (Connect: 0, Receive: 0, Length: 400, Exceptions: 0)
Write errors:           0
Non-2xx responses:      400
Total transferred:      1759574400 bytes
HTML transferred:       1712756400 bytes
Requests per second:    3748.63 [#/sec] (mean)
Time per request:       266.764 [ms] (mean)
Time per request:       0.267 [ms] (mean, across all concurrent requests)
Transfer rate:          16103.48 [Kbytes/sec] received

Connection Times (ms)
              min  mean[+/-sd] median   max
Connect:        0  183 761.8     12   63119
Processing:     3   80 180.7     40   33130
Waiting:        1   73 161.6     35   11186
Total:          3  264 789.9     59   63153

Percentage of the requests served within a certain time (ms)
  50%     59
  66%     81
  75%    103
  80%    148
  90%   1044
  95%   1098
  98%   2323
  99%   3073
 100%  63153 (longest request)
```
## 多线程
### GET 请求
```
[root@192 ~]# ab  -k -n 3000 -c 1000 'http://192.168.1.128/get_show.html?user_name=%E5%B0%8F%E7%8E%8B&email=%E5%B9%BF%E4%B8%9C%E7%9C%81&phone=110119112'
This is ApacheBench, Version 2.3 <$Revision: 1430300 $>
Copyright 1996 Adam Twiss, Zeus Technology Ltd, http://www.zeustech.net/
Licensed to The Apache Software Foundation, http://www.apache.org/

Benchmarking 192.168.1.128 (be patient)
Completed 300 requests
Completed 600 requests
Completed 900 requests
Completed 1200 requests
Completed 1500 requests
Completed 1800 requests
Completed 2100 requests
Completed 2400 requests
Completed 2700 requests
Completed 3000 requests
Finished 3000 requests


Server Software:        WEBD
Server Hostname:        192.168.1.128
Server Port:            80

Document Path:          /get_show.html?user_name=%E5%B0%8F%E7%8E%8B&email=%E5%B9%BF%E4%B8%9C%E7%9C%81&phone=110119112
Document Length:        2878 bytes

Concurrency Level:      1000
Time taken for tests:   3.840 seconds
Complete requests:      3000
Failed requests:        1965
   (Connect: 0, Receive: 0, Length: 1965, Exceptions: 0)
Write errors:           0
Non-2xx responses:      1965
Keep-Alive requests:    0
Total transferred:      3762855 bytes
HTML transferred:       3326535 bytes
Requests per second:    781.25 [#/sec] (mean)
Time per request:       1279.996 [ms] (mean)
Time per request:       1.280 [ms] (mean, across all concurrent requests)
Transfer rate:          956.95 [Kbytes/sec] received

Connection Times (ms)
              min  mean[+/-sd] median   max
Connect:        0   91 259.9      4    1014
Processing:     6  606 913.2     29    3693
Waiting:        6  605 913.4     28    3693
Total:          8  697 1035.4     35    3801

Percentage of the requests served within a certain time (ms)
  50%     35
  66%    366
  75%   1251
  80%   1767
  90%   2397
  95%   3040
  98%   3393
  99%   3472
 100%   3801 (longest request)
```
### POST请求
```
[root@192 ~]#  ab -k -n 3000 -c 1000 -p postdata.txt -T application/x-www-form-urlencoded "http://192.168.1.128/post_show.html"
This is ApacheBench, Version 2.3 <$Revision: 1430300 $>
Copyright 1996 Adam Twiss, Zeus Technology Ltd, http://www.zeustech.net/
Licensed to The Apache Software Foundation, http://www.apache.org/

Benchmarking 192.168.1.128 (be patient)
Completed 300 requests
Completed 600 requests
Completed 900 requests
Completed 1200 requests
Completed 1500 requests
Completed 1800 requests
Completed 2100 requests
Completed 2400 requests
Completed 2700 requests
Completed 3000 requests
Finished 3000 requests


Server Software:        WEBD
Server Hostname:        192.168.1.128
Server Port:            80

Document Path:          /post_show.html
Document Length:        2879 bytes

Concurrency Level:      1000
Time taken for tests:   9.122 seconds
Complete requests:      3000
Failed requests:        46
   (Connect: 0, Receive: 0, Length: 46, Exceptions: 0)
Write errors:           0
Non-2xx responses:      46
Keep-Alive requests:    0
Total transferred:      8856916 bytes
Total body sent:        804000
HTML transferred:       8512708 bytes
Requests per second:    328.88 [#/sec] (mean)
Time per request:       3040.604 [ms] (mean)
Time per request:       3.041 [ms] (mean, across all concurrent requests)
Transfer rate:          948.20 [Kbytes/sec] received
                        86.07 kb/s sent
                        1034.28 kb/s total

Connection Times (ms)
              min  mean[+/-sd] median   max
Connect:        0  661 1454.2     34    7027
Processing:     0 1515 647.0   1567    4022
Waiting:        0 1515 647.0   1566    4022
Total:          1 2175 1594.6   1873    8914

Percentage of the requests served within a certain time (ms)
  50%   1873
  66%   2245
  75%   2636
  80%   2873
  90%   3560
  95%   5096
  98%   8490
  99%   8800
 100%   8914 (longest request)
```
### GET请求，不解析模板
```
[root@192 ~]#  ab -n 300000 -c 1000 'http://192.168.1.128/favicon.ico?user_name=%E5%B0%8F%E7%8E%8B&email=%E5%B9%BF%E4%B8%9C%E7%9C%81&phone=110119112'
This is ApacheBench, Version 2.3 <$Revision: 1430300 $>
Copyright 1996 Adam Twiss, Zeus Technology Ltd, http://www.zeustech.net/
Licensed to The Apache Software Foundation, http://www.apache.org/

Benchmarking 192.168.1.128 (be patient)
Completed 30000 requests
Completed 60000 requests
Completed 90000 requests
Completed 120000 requests
Completed 150000 requests
Completed 180000 requests
Completed 210000 requests
Completed 240000 requests
Completed 270000 requests
Completed 300000 requests
Finished 300000 requests


Server Software:        WEBD
Server Hostname:        192.168.1.128
Server Port:            80

Document Path:          /favicon.ico?user_name=%E5%B0%8F%E7%8E%8B&email=%E5%B9%BF%E4%B8%9C%E7%9C%81&phone=110119112
Document Length:        4286 bytes

Concurrency Level:      1000
Time taken for tests:   81.347 seconds
Complete requests:      300000
Failed requests:        0
Write errors:           0
Total transferred:      1320900000 bytes
HTML transferred:       1285800000 bytes
Requests per second:    3687.91 [#/sec] (mean)
Time per request:       271.157 [ms] (mean)
Time per request:       0.271 [ms] (mean, across all concurrent requests)
Transfer rate:          15857.28 [Kbytes/sec] received

Connection Times (ms)
              min  mean[+/-sd] median   max
Connect:        0  195 742.3      6   31076
Processing:     6   70 145.0     35    6722
Waiting:        1   66 143.7     31    6716
Total:         17  265 767.1     49   31122

Percentage of the requests served within a certain time (ms)
  50%     49
  66%     63
  75%     78
  80%    103
  90%   1039
  95%   1090
  98%   3041
  99%   3081
 100%  31122 (longest request)
```
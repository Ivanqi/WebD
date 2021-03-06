# 服务器信息
- 两台 4核4G 的Centos 虚拟机进行测试

# 无参数
## 单线程
### 短链接
```
[root@192 ~]# ab -n 400000 -c 1000 http://192.168.1.128:8000/
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
Server Port:            8000

Document Path:          /
Document Length:        703 bytes

Concurrency Level:      1000
Time taken for tests:   92.179 seconds
Complete requests:      400000
Failed requests:        0
Write errors:           0
Total transferred:      312000000 bytes
HTML transferred:       281200000 bytes
Requests per second:    4339.37 [#/sec] (mean)
Time per request:       230.448 [ms] (mean)
Time per request:       0.230 [ms] (mean, across all concurrent requests)
Transfer rate:          3305.38 [Kbytes/sec] received

Connection Times (ms)
              min  mean[+/-sd] median   max
Connect:        0  162 593.5      9   31084
Processing:     5   66 126.7     32    6749
Waiting:        1   63 126.2     30    6749
Total:         13  227 611.9     44   31110

Percentage of the requests served within a certain time (ms)
  50%     44
  66%     58
  75%     75
  80%    108
  90%   1031
  95%   1064
  98%   1475
  99%   3052
 100%  31110 (longest request)
```
### 长连接
```
[root@192 ~]# ab -k -n 400000 -c 1000 http://192.168.1.128:8000/
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
Server Port:            8000

Document Path:          /
Document Length:        703 bytes

Concurrency Level:      1000
Time taken for tests:   92.007 seconds
Complete requests:      400000
Failed requests:        0
Write errors:           0
Keep-Alive requests:    0
Total transferred:      312000000 bytes
HTML transferred:       281200000 bytes
Requests per second:    4347.49 [#/sec] (mean)
Time per request:       230.018 [ms] (mean)
Time per request:       0.230 [ms] (mean, across all concurrent requests)
Transfer rate:          3311.56 [Kbytes/sec] received

Connection Times (ms)
              min  mean[+/-sd] median   max
Connect:        0  161 579.2      8   31064
Processing:     6   64 130.4     31    3530
Waiting:        1   62 130.0     30    3530
Total:         15  226 600.1     42   31105

Percentage of the requests served within a certain time (ms)
  50%     42
  66%     53
  75%     68
  80%     91
  90%   1032
  95%   1061
  98%   1513
  99%   3052
 100%  31105 (longest request)
```

## 多线程(4个线程)
### 短连接
```
[root@192 ~]# ab -n 400000 -c 1000 http://192.168.1.128:8000/
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
Server Port:            8000

Document Path:          /
Document Length:        703 bytes

Concurrency Level:      1000
Time taken for tests:   89.877 seconds
Complete requests:      400000
Failed requests:        0
Write errors:           0
Total transferred:      312000000 bytes
HTML transferred:       281200000 bytes
Requests per second:    4450.54 [#/sec] (mean)
Time per request:       224.692 [ms] (mean)
Time per request:       0.225 [ms] (mean, across all concurrent requests)
Transfer rate:          3390.06 [Kbytes/sec] received

Connection Times (ms)
              min  mean[+/-sd] median   max
Connect:        1  112  63.8    109    3124
Processing:    16  112  27.3    110     692
Waiting:        1  111  27.0    110     690
Total:         34  224  70.7    219    3272

Percentage of the requests served within a certain time (ms)
  50%    219
  66%    224
  75%    228
  80%    232
  90%    245
  95%    259
  98%    282
  99%    334
 100%   3272 (longest request)
 
```
### 长连接
```
[root@192 webapp]# ab -k -n 400000 -c 1000 http://192.168.1.128:8000/
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
Server Port:            8000

Document Path:          /
Document Length:        703 bytes

Concurrency Level:      1000
Time taken for tests:   86.704 seconds
Complete requests:      400000
Failed requests:        0
Write errors:           0
Keep-Alive requests:    0
Total transferred:      312000000 bytes
HTML transferred:       281200000 bytes
Requests per second:    4613.40 [#/sec] (mean)
Time per request:       216.760 [ms] (mean)
Time per request:       0.217 [ms] (mean, across all concurrent requests)
Transfer rate:          3514.11 [Kbytes/sec] received

Connection Times (ms)
              min  mean[+/-sd] median   max
Connect:        1  108  53.2    107    1123
Processing:    10  109  21.5    108     497
Waiting:        1  108  21.4    108     490
Total:         37  216  58.2    216    1533

Percentage of the requests served within a certain time (ms)
  50%    216
  66%    220
  75%    222
  80%    224
  90%    229
  95%    234
  98%    245
  99%    266
 100%   1533 (longest request)
```

# 带参数
## 单线程
### GET请求
```
[root@192 ~]#  ab -n 10000 -c 1000 'http://192.168.1.128:8000/get_show.html?user_name=%E5%B0%8F%E7%8E%8B&email=%E5%B9%BF%E4%B8%9C%E7%9C%81&phone=110119112'
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
Server Port:            8000

Document Path:          /get_show.html?user_name=%E5%B0%8F%E7%8E%8B&email=%E5%B9%BF%E4%B8%9C%E7%9C%81&phone=110119112
Document Length:        2878 bytes

Concurrency Level:      1000
Time taken for tests:   38.156 seconds
Complete requests:      10000
Failed requests:        0
Write errors:           0
Total transferred:      29550000 bytes
HTML transferred:       28780000 bytes
Requests per second:    262.08 [#/sec] (mean)
Time per request:       3815.579 [ms] (mean)
Time per request:       3.816 [ms] (mean, across all concurrent requests)
Transfer rate:          756.31 [Kbytes/sec] received

Connection Times (ms)
              min  mean[+/-sd] median   max
Connect:        0 1154 2457.0     64   15053
Processing:    61  936 1877.4    530   26045
Waiting:        3  935 1877.3    529   26045
Total:        121 2090 3226.7   1226   32825

Percentage of the requests served within a certain time (ms)
  50%   1226
  66%   1545
  75%   2233
  80%   3340
  90%   3980
  95%   7554
  98%  15374
  99%  16168
 100%  32825 (longest request)
```
### POST请求
```
[root@192 ~]#  ab -n 3000 -c 1000 -p postdata.txt -T application/x-www-form-urlencoded "http://192.168.1.128:8000/post_show.html"
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
Server Port:            8000

Document Path:          /post_show.html
Document Length:        2879 bytes

Concurrency Level:      1000
Time taken for tests:   13.326 seconds
Complete requests:      3000
Failed requests:        0
Write errors:           0
Total transferred:      8868000 bytes
Total body sent:        747000
HTML transferred:       8637000 bytes
Requests per second:    225.12 [#/sec] (mean)
Time per request:       4442.012 [ms] (mean)
Time per request:       4.442 [ms] (mean, across all concurrent requests)
Transfer rate:          649.87 [Kbytes/sec] received
                        54.74 kb/s sent
                        704.61 kb/s total

Connection Times (ms)
              min  mean[+/-sd] median   max
Connect:        0  763 1456.5     88    7019
Processing:    58 1234 1191.5    679   12838
Waiting:        4 1234 1191.6    679   12837
Total:        132 1997 1748.5   1543   12838

Percentage of the requests served within a certain time (ms)
  50%   1543
  66%   2397
  75%   3262
  80%   3398
  90%   3752
  95%   4768
  98%   7751
  99%   8623
 100%  12838 (longest request)
```
### GET请求，不解析模板
```
[root@192 src]#  ab -n 400000 -c 1000 'http://192.168.1.128:8000/favicon.ico?user_name=%E5%B0%8F%E7%8E%8B&email=%E5%B9%BF%E4%B8%9C%E7%9C%81&phone=110119112'
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
Server Port:            8000

Document Path:          /favicon.ico?user_name=%E5%B0%8F%E7%8E%8B&email=%E5%B9%BF%E4%B8%9C%E7%9C%81&phone=110119112
Document Length:        4286 bytes

Concurrency Level:      1000
Time taken for tests:   103.260 seconds
Complete requests:      400000
Failed requests:        0
Write errors:           0
Total transferred:      1746400000 bytes
HTML transferred:       1714400000 bytes
Requests per second:    3873.71 [#/sec] (mean)
Time per request:       258.150 [ms] (mean)
Time per request:       0.258 [ms] (mean, across all concurrent requests)
Transfer rate:          16516.24 [Kbytes/sec] received

Connection Times (ms)
              min  mean[+/-sd] median   max
Connect:        0  177 641.8      8   31084
Processing:     6   78 162.1     38   14007
Waiting:        1   74 161.4     34   14004
Total:         16  255 666.6     49   31530

Percentage of the requests served within a certain time (ms)
  50%     49
  66%     60
  75%     77
  80%    246
  90%   1041
  95%   1072
  98%   1881
  99%   3060
 100%  31530 (longest request)
```

## 多线程
### GET 请求
```
[root@192 src]#  ab  -k -n 10000 -c 1000 'http://192.168.1.128:8000/get_show.html?user_name=%E5%B0%8F%E7%8E%8B&email=%E5%B9%BF%E4%B8%9C%E7%9C%81&phone=110119112'DD
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
Server Port:            8000

Document Path:          /get_show.html?user_name=%E5%B0%8F%E7%8E%8B&email=%E5%B9%BF%E4%B8%9C%E7%9C%81&phone=110119112DD
Document Length:        2880 bytes

Concurrency Level:      1000
Time taken for tests:   35.860 seconds
Complete requests:      10000
Failed requests:        0
Write errors:           0
Keep-Alive requests:    10000
Total transferred:      29840000 bytes
HTML transferred:       28800000 bytes
Requests per second:    278.87 [#/sec] (mean)
Time per request:       3585.958 [ms] (mean)
Time per request:       3.586 [ms] (mean, across all concurrent requests)
Transfer rate:          812.63 [Kbytes/sec] received

Connection Times (ms)
              min  mean[+/-sd] median   max
Connect:        0   39 264.9      0    3012
Processing:    19 3375 713.5   3505    7607
Waiting:        5 3375 713.5   3505    7607
Total:         19 3414 820.8   3508    9293

Percentage of the requests served within a certain time (ms)
  50%   3508
  66%   3580
  75%   3674
  80%   3716
  90%   3814
  95%   3850
  98%   4143
  99%   5980
 100%   9293 (longest request)
```

### POST请求
```
[root@192 ~]#  ab -k -n 100000 -c 1000 -p postdata.txt -T application/x-www-form-urlencoded "http://192.168.1.128:8000/post_show.html"
This is ApacheBench, Version 2.3 <$Revision: 1430300 $>
Copyright 1996 Adam Twiss, Zeus Technology Ltd, http://www.zeustech.net/
Licensed to The Apache Software Foundation, http://www.apache.org/

Benchmarking 192.168.1.128 (be patient)
Completed 10000 requests
Completed 20000 requests
Completed 30000 requests
Completed 40000 requests
Completed 50000 requests
Completed 60000 requests
Completed 70000 requests
Completed 80000 requests
Completed 90000 requests
Completed 100000 requests
Finished 100000 requests


Server Software:        WEBD
Server Hostname:        192.168.1.128
Server Port:            8000

Document Path:          /post_show.html
Document Length:        2879 bytes

Concurrency Level:      1000
Time taken for tests:   355.881 seconds
Complete requests:      100000
Failed requests:        0
Write errors:           0
Keep-Alive requests:    100000
Total transferred:      298300000 bytes
Total body sent:        27300000
HTML transferred:       287900000 bytes
Requests per second:    280.99 [#/sec] (mean)
Time per request:       3558.810 [ms] (mean)
Time per request:       3.559 [ms] (mean, across all concurrent requests)
Transfer rate:          818.56 [Kbytes/sec] received
                        74.91 kb/s sent
                        893.47 kb/s total

Connection Times (ms)
              min  mean[+/-sd] median   max
Connect:        0    3  53.7      0    1021
Processing:    36 3538 360.6   3528    7815
Waiting:        4 3538 360.6   3528    7815
Total:         38 3541 364.7   3529    7815

Percentage of the requests served within a certain time (ms)
  50%   3529
  66%   3587
  75%   3622
  80%   3647
  90%   3757
  95%   3829
  98%   3901
  99%   3949
 100%   7815 (longest request)
```

### GET请求，不解析模板
```
[root@192 src]#  ab -n 400000 -c 1000 'http://192.168.1.128:8000/favicon.ico?user_name=%E5%B0%8F%E7%8E%8B&email=%E5%B9%BF%E4%B8%9C%E7%9C%81&phone=110119112'
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
Server Port:            8000

Document Path:          /favicon.ico?user_name=%E5%B0%8F%E7%8E%8B&email=%E5%B9%BF%E4%B8%9C%E7%9C%81&phone=110119112
Document Length:        4286 bytes

Concurrency Level:      1000
Time taken for tests:   85.915 seconds
Complete requests:      400000
Failed requests:        0
Write errors:           0
Total transferred:      1746400000 bytes
HTML transferred:       1714400000 bytes
Requests per second:    4655.77 [#/sec] (mean)
Time per request:       214.787 [ms] (mean)
Time per request:       0.215 [ms] (mean, across all concurrent requests)
Transfer rate:          19850.67 [Kbytes/sec] received

Connection Times (ms)
              min  mean[+/-sd] median   max
Connect:        1  117 412.6     47   15077
Processing:    21   97  89.5     69    2708
Waiting:        1   72  73.5     50    2042
Total:         47  214 424.6    123   15146

Percentage of the requests served within a certain time (ms)
  50%    123
  66%    147
  75%    170
  80%    188
  90%    394
  95%    792
  98%   1154
  99%   1238
 100%  15146 (longest request)
```

# 原理
ab命令会创建多个并发访问线程，模拟多个访问者同时对某一URL地址进行访问

它的测试目标是基于URL的，因此，它既可以用来测试apache的负载压力，也可以测试nginx、lighthttp、tomcat、IIS等其它Web服务器的压力

# ab的使用
```
ab -n 10000 -c 1000 http://192.168.0.128/
```
# ab命令选项详解
| 选项 | 作用 |
| --- | --- |
| -n | 在测试会话中所执行的请求个数。默认时，仅执行一个请求 |
| -c	 | 一次产生的请求个数。默认是一次一个 |
| -t | 测试所进行的最大秒数。其内部隐含值是-n 50000，它可以使对服务器的测试限制在一个固定的总时间以内。默认时，没有时间限制 |
| -p | 包含了需要POST的数据的文件 |
| -P | 对一个中转代理提供BASIC认证信任。用户名和密码由一个:隔开，并以base64编码形式发送。无论服务器是否需要(即, 是否发送了401认证需求代码)，此字符串都会被发送 |
| -T | POST数据所使用的Content-type头信息 |
| -v | 设置显示信息的详细程度-4或更大值会显示头信息，3或更大值可以显示响应代码(404,200等),2或更大值可以显示警告和其他信息 |
| -V | 显示版本号并退出 |
| -w | 以HTML表的格式输出结果。默认时，它是白色背景的两列宽度的一张表 |
| -i | 执行HEAD请求，而不是GET |
| -x | 设置<\table>属性的字符串 |
| -X | 对请求使用代理服务器 |
| -y | 设置<\tr>属性的字符串 |
| -z | 设置<\td>属性的字符串 |
| -C | 对请求附加一个Cookie:行。其典型形式是name=value的一个参数对，此参数可以重复 |
| -H | 对请求附加额外的头信息。此参数的典型形式是一个有效的头信息行，其中包含了以冒号分隔的字段和值的对(如,“Accept-Encoding:zip/zop;8bit”) |
| -A | 对服务器提供BASIC认证信任。用户名和密码由一个:隔开，并以base64编码形式发送。无论服务器是否需要(即,是否发送了401认证需求代码)，此字符串都会被发送 |
| -h | 显示使用方法 |
| -d | 不显示"percentage served within XX [ms] table"的消息(为以前的版本提供支持) |
| -e | 产生一个以逗号分隔的(CSV)文件，其中包含了处理每个相应百分比的请求所需要(从1%到100%)的相应百分比的(以微妙为单位)时间。由于这种格式已经“二进制化”，所以比’gnuplot’格式更有用 |
| -g | 把所有测试结果写入一个’gnuplot’或者TSV(以Tab分隔的)文件。此文件可以方便地导入到Gnuplot,IDL,Mathematica,Igor甚至Excel中。其中的第一行为标题 |
| -e | 产生一个以逗号分隔的(CSV)文件，其中包含了处理每个相应百分比的请求所需要(从1%到100%)的相应百分比的(以微妙为单位)时间。由于这种格式已经“二进制化”，所以比’gnuplot’格式更有用 |
| -i | 执行HEAD请求，而不是GET |
| -k | 启用HTTP KeepAlive功能，即在一个HTTP会话中执行多个请求。默认时，不启用KeepAlive功能 |
| -q | 如果处理的请求数大于150，ab每处理大约10%或者100个请求时，会在stderr输出一个进度计数。此-q标记可以抑制这些信息 |
| -r | 在遇到socket接收错误后，不退出测试 |

# ab的结果说明
```
Server Software: nginx/1.6.2					#测试目标服务器使用的WEB服务和版本
Server Hostname: 192.168.1.108					#测试目标服务器的地址
Server Port: 80									#测试目标服务器的端口
Document Path: /								#测试目标服务器的路径
Document Length: 41005 bytes 					#请求的页面大小
Concurrency Level: 20 							#并发量
Time taken for tests: 1180.733 seconds 			#测试总共耗时
Complete requests: 50000 						#完成的请求
Failed requests: 0 								#失败的请求
Write errors: 0 								#错误
Total transferred: 2067550000 bytes 			#总共传输数据量
HTML transferred: 2050250000 bytes				#所有请求的响应数据中正文数据的总和，也就是减去了Total transferred中HTTP响应数据中的头信息的长度
Requests per second: 42.35 [#/sec] (mean) 		#每秒钟的请求量。（仅仅是测试页面的响应速度）
Time per request: 472.293 [ms] (mean) 			#等于 Time taken for tests/(complete requests/concurrency level) 即平均请求等待时间（用户等待的时间）
Time per request: 23.615 [ms] (mean, across all concurrent requests) 		#等于 Time taken for tests/Complete requests 即服务器平均请求响应时间 在并发量为1时 用户等待时间相同
Transfer rate: 1710.03 [Kbytes/sec] received 	#平均每秒多少K，即带宽速率
Connection Times (ms)
min mean[+/-sd] median max
Connect: 0 1 18.5 0 1001
Processing: 38 471 534.1 155 9269
Waiting: 37 456 524.6 147 9259
Total: 40 472 534.5 155 9269
Percentage of the requests served within a certain time (ms)
50% 155
66% 571
75% 783
80% 871
90% 1211
95% 1603
98% 1839
99% 2003
100% 9269 (longest request)
```
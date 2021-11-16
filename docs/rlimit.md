# rlimit介绍
在Linux系统中，Resouce limit指在一个进程的执行过程中，它所能得到的资源的限制，比如进程的core file的最大值，虚拟内存的最大值等

Resouce limit的大小可以直接影响进程的执行状况。其有两个最重要的概念：soft limit 和 hard limit
```
struct rlimit {
    rlim_t rlim_cur;
    rlim_t rlim_max;
};
```
soft limit是指内核所能支持的资源上限
- 比如对于RLIMIT_NOFILE(一个进程能打开的最大文件 数，内核默认是1024)，soft limit最大也只能达到1024
- 对于RLIMIT_CORE(core文件的大小，内核不做限制)，soft limit最大能是unlimited

hard limit在资源中只是作为soft limit的上限
- 当你设置hard limit后，你以后设置的soft limit只能小于hard limit
- 要说明的是，hard limit只针对非特权进程，也就是进程的有效用户ID(effective user ID)不是0的进程。
- 具有特权级别的进程(具有属性CAP_SYS_RESOURCE)，soft limit则只有内核上限

结构体中 rlim_cur是要取得或设置的资源软限制的值，rlim_max是硬限制

这两个值的设置有一个小的约束
-  任何进程可以将软限制改为小于或等于硬限制
-  任何进程都可以将硬限制降低，但普通用户降低了就无法提高，该值必须等于或大于软限制
-  有超级用户可以提高硬限制

# 命令执行
```
# soft limit
~>ulimit -c -n -s 

core file size          (blocks, -c) 0
open files                      (-n) 100001
stack size              (kbytes, -s) 8192

 
# -H表示显示的是hard limit
~>ulimit -c -n -s -H
core file size          (blocks, -c) unlimited
open files                      (-n) 100001
stack size              (kbytes, -s) unlimited
```
从结果上可以看出soft limit和hard limit的区别。unlimited表示no limit, 即内核的最大值

对于resouce limit的读取修改，有两种方法
- 使用shell内建命令ulimit 
- 使用getrlimit和setrlimit API

ulimit是改变shell的resouce limit，并达到改变shell启动的进程的resouce limit效果(子进程继承)。

# 在C中使用ulimit
```
if (getrlimit(RLIMIT_CORE, &rlim)==0) {
    rlim_new.rlim_cur = rlim_new.rlim_max = RLIM_INFINITY;
    if (setrlimit(RLIMIT_CORE, &rlim_new)!=0) {
        rlim_new.rlim_cur = rlim_new.rlim_max =
        rlim.rlim_max;
        (void) setrlimit(RLIMIT_CORE, &rlim_new);
    }
}
```

# resource 标识
## RLIMIT_AS
进程的最大虚拟内存（地址）空格）以字节为单位

此限制影响对brk、mmap的调用和mremap，当超过这个限度

## RLIMIT_CORE
设定最大的core文件，当值为0时将禁止core文件非0时将设定产生的最大core文件大小为设定的值

## RLIMIT_CPU
CPU时间的最大量值（秒），当超过此软限制时向该进程发送SIGXCPU信号

## RLIMIT_DATA
数据段的最大字节长度

## RLIMIT_FSIZE
可以创建的文件的最大字节长度，当超过此软限制时向进程发送SIGXFSZ

## RLIMIT_MEMLOCK
可以使用的虚拟内存的最大字节数使用mlock（）和mlockall（）锁定到RAM中

## RLIMIT_NOFILE
每个进程能够打开的最多文件数。更改此限制将影响到sysconf函数在参数_SC_CHILD_MAX中的返回值

## RLIMIT_OFILE
RLIMIT_OFILE is the BSD name for RLIMIT_NOFILE

这里BSD系统中RLIMIT_NOFILE的别名

## RLIMIT_NPROC
每个实际用户ID所拥有的最大子进程数，更改此限制将影响到sysconf函数在参数_SC_CHILD_MAX中返回的值

## RLIMIT_RSS
最大驻内存集字节长度(RSS)如果物理存储器供不应求则内核将从进程处取回超过RSS的部份

## RLIMIT_STACK
栈的最大长度

## RLIMIT——VMEM
VMEM 可映照地址空间的最大字节长茺，这影响到mmap函数

# 参考资料
- [struct rlimit介绍](https://blog.csdn.net/turkeyzhou/article/details/16847619)
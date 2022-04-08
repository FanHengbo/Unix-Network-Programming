# Introduction
Unix domain Protocol是用来进行同一个host上的进程之间的通讯的，这很像Unix提供的pipe函数，这种也是IPC的一种方式，事实上按照分类也可以分为stream socket(像TCP)或者是datagram socket(像UDP)一样.网络通讯通过IP地址和port号来确定host,那么进程之间就利用`pathname`来确定.
# Socket Address Structure
类似于`sockaddr_in`,这种Unix domain也存在对应的储存地址的结构体:
![](image/2022-04-07-17-45-26.png)
其中`sun_familily`占用2bytes,指定的类型应该为`AF_LOCAL`.
在使用Unix domain socket的时候,与network socket对比存在以下的几点不同以及限制:
- 文件的permission(rwx,用三个八进制数来表示user, group, 和other的权限)由umask值和0777确定(0777 - umask).
- `pathname`应是一个绝对路径不能是相对路径.
- 在`connect`函数中指定的`pathname`必须和已经bind到Unix domain socket是同样的类型(stream或者是datagram).
- 对其使用`connect`函数的话和使用`open`函数write only的情况相同.
- 如果调用`connect`的时候listening方的queue满了,这个时候会直接返回赢得`ECONNREFUSED`,这就和TCP不一样了,TCP在遇到这种情况的时候,listening这一方会忽略SYNbit,这时候发送方会多尝试几次发送SYN
- 和UDP不一样的地方在于如果使用一个unbound的一个UDP socket发送数据的话kernel不会自动给它bind一个`pathname`(UDP会在发送的时候给它分配一个端口),这也就意味着不bind就不是full-duplex,receiver无法回复消息.也就是说无论是TCP还是UDP,如果不bind的话,在`connect`函数中kernel是不会帮你bind的.
# passing descriptors
现在考虑以下问题,如何将开放的descriptor从一个进程传到另外一个进程,比较容易想到的思路是通过fork来实现,因为child process会拥有所有parent process的descriptor,另外一种方式是通过exec类函数,这类函数可以将替换进程的image的时候是保持所有descriptor开放的.那么如何在一个进程中让另外一个进程的某个descriptor开放呢,这个时候就需要使用到Unix domain protocol了.
简而言之是通过进程之间通讯的方式来完成在一个进程中要求另外一个进程的某个descriptor开放,具体的步骤如下:
- 首先创造一个Unix domain socket,如果进程之间是无关的(例如不是父子关系),那么具体步骤就和我们之前叙述的相似,server和client bind某个pathname之后connect来进行通讯
- 之后就可以创建新的descriptor(使用open, pipe, socket, accept这种函数),也可以看出我们要求的是在进程之间传递descriptor,而不是仅仅file descriptor.
- sending process通过一个`msghdr`结构体来传递希望接受者打开的descriptor, 在使用`sendmsg`函数之后我们可以称这个descriptor进入了in flight状态,这时候哪怕刚刚调用完`sendmsg`函数进程就把这个descriptor关了(当然是在接受者调用`recvmsg`之前),这个descriptor是始终开放的.
  
##**1. 功能描述**
中继网络聊天室。1个TCP server，若干个client。          
每个client可以设置自己的昵称。          
每个client向server发送消息后，server会转发给所有其他client。          
每个client可以随时自由进出聊天室。          
server可以强行关闭聊天室，并在关闭前向所有在线client发警告。          
(没有实现超时、心跳功能)        
          
          
##**2. 设计思路**  
        
server 需要记录在线 client 的信息（如昵称等），通过维护`struct cinfo`链表实现。

```c
struct cinfo {
	int sockfd;
	int id;                 //聊天中显示在该用户所发消息前的用户unique ID，类似QQ号的功能
	char nickname[MAXNAME];
	int nameset;            //该client是否有设置昵称
	struct cinfo *next;
};
```
        
        
对该链表，有相应的一系列操作：

```c
void new_client( struct cinfo **phead, int fd);                                 //添加最新加入的client，其sockfd==fd
void destroy(struct cinfo **phead);                                             //free整个链表
int delete_client( struct cinfo **phead, int fd, FILE *logfp);                  //删除sockfd==fd的client记录
int search_client( const struct cinfo *head, struct cinfo **ppresult, int fd);  //查找sockfd==fd的client信息，装入ppresult
int change_name(int fd, char *s, struct cinfo **phead, FILE *logfp);            //将sockfd==fd的client的昵称该为s，若s过长则截断
```
                
                
设计“协议”（消息格式）如下：

```c
struct c_mesg {
	int type;
	char mesg[MAXMSG];
};
```


|type|消息类型|字符串mesg[]内容|
|:-:|:-:|:-:|
|1|设置昵称|用户昵称|
|2|群发消息|消息内容|
|3|用户退出|无意义|
                
                
                
####**server主函数思路：**    			
          
借鉴了 GNU C Library mannual 中的示例代码：<http://www.gnu.org/software/libc/manual/html_node/Server-Example.html>          

采用 I/O多路复用方法。将`STDIN`、监听套接字、每个会话套接字加入`select`的关注列表中，循环`select`，每次`select`返回时判断：  
-   若`STDIN`活跃，表明 server 的 master 希望关闭 server。则通知每个在线 client 退出，善后之后退出。
-   若监听套接字活跃，表明有新的 client 希望进入聊天室，`accept`产生新的会话套接字，将套接字加入`select`的关注列表、client 信息加入 server 维护的`cinfo`链表。
-   若会话套接字活跃，调用`mesg_from_client`函数处理来自该套接字的消息。对于出错或要求退出的 client，从 select 关注列表和`cinfo`链表中删除记录，关闭套接字。
        
        
```c
int mesg_from_client( int filedes, FILE *logfp, struct cinfo **phead);
```

从`filedes`套接字文件描述符中读消息，如前所述，处理`struct c_mesg`消息，根据 type 判断消息类型进行处理，包括更改昵称、发送相应的处理结果系统通知给该 client、记录日志、提取消息内容字符串`mesg[]`群发给用户等。
          
          
          
####**一些辅助函数：**

```c
int pr_time(char *buf, size_t maxsize);
//按 “YYYY-MM-DD 星期几 HH:MM:SS” 格式将本地时间打印到`buf`中
int make_socket_server( int portnumber );
//创建TCP socket，绑定到端口`portnumber`，返回sockfd
```
          
          
                    
####**client主函数思路：**    				

指数退让不断尝试对server进行`connect`，等待`MAXSLEEP`秒仍失败时放弃并退出。          		
I/O多路复用，用`select`循环监听`STDIN`和套接字，`select`每次返回时判断：  		

-   若套接字活跃，将读取的内容打印到标准输出。
    -   其中，若字串内容== server的关闭通知，说明server即将关闭，则关闭套接字并退出。
-   若STDIN活跃，则根据读取的内容判断用户希望进行的操作：
    -   若读到文件结束符（ctrl+d）或`“exit”`，表明用户希望退出聊天室，向server发送type为3的消息以通知server，然后退出。
    -   若读到`“set nickname”`，则将用户的下一行输入作为用户昵称，构造type为1的消息发送给server。
    -   其他输入，则作为用户希望群发的消息的内容，构造type为2的消息发送给server。
          
          
          

##**3.编译与测试**  
    
编译方法：
```shell
client/make all
server/make all
```  
  
  
测试方法说明:  
```shell
client/make test
server/make test
```  
  
  
测试样例日志参见:  `server/chatroom.log`
 

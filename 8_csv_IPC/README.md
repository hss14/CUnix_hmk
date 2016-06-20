##**1. 问题描述**
  
将作业6改为父子进程间使用进程间通信机制进行数据交换：  

+ `csv2bin`：一个父进程，多个子进程。每次父进程读一行，发送给某个子进程
+ `bincat`： 用`mmap`文件映射方式读二进制文件，打印完整的表格  
  
  
  
##**2. 设计思路**  
  
本作业在第6次(和第7次)作业的基础上完成，因此与作业6（7）相同的部分将不再赘述。  
  
  
  
```c
int line_csv2bin( const char *line, struct StuRecord *stu, uint8_t *stuname, FILE *fpbad );
```
将作业6中处理每行表格并填入`stu`和`stuname`的代码提取出来成为`line_csv2bin`函数，方便后续调用。
- 对于传入的一行csv表格`line`，若合法，则通过填入`stu`和`stuname`将处理结果传给调用者，同时返回`1`；
- 否则返回`0`.处理中所有遇到的错误或非法数据格式则记入文件`fpbad`.  
  
  
  
```c
#define NCHILD 4         //参数：几个转码子进程？
int csv2bin( const char *filecsv, const char *filebin );
```
本次作业的主体部分。
-   父进程首先`fork` `NCHILD`次，产生`NCHILD`个（转码）子进程
-   仿照课件中的例子，建立了`NCHILD`对半双工通信管道`par[NCHILD][2]`和`chd[NCHILD][2]`
-   父进程处理原始csv文件，将读入的每一行，以轮询的方式，通过管道依次发送给每个子进程；发送完一轮后，依次从相应的管道读取处理结果并写入结果bin文件。完成这样一轮以后重复此过程直到处理结束。
-   子进程每次从管道读入一行，处理完成后将结果通过管道返回父进程。返回的第一个数据为转码是否成功的`flag`，父进程读入`flag`后，根据其值决定是否继续读取处理结果`stu`和`stuname`。  
  
  
  
```c
int bincat(const char* filebin);
```
-   用`mmap`将整个`filebin`文件映射到内存中
-   用`memcpy`依次获取各个相应的数据  
  
   
  
本次作业比第7次作业更为简单，`pipe`和`mmap`、`memcpy`使用起来非常简单，基本完全没有调试就一次性通过了 ^_^  
  
  
  
##**3.编译与测试**  
    
`make all`：编译方法  
  
  
`make test`：测试样例及测试方法说明  
  

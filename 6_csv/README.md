##**1. 问题描述**

纯文本csv文件中有一个表，大致格式如下：

```
Student id, Name, Department, Age
2014310540, Hao Shanshan, E.E., 23
2015310044,   Dou Sen,     D.A.,        22
...
```

设计一种二进制存储格式以存放这样的表。字节序为BigEndian

实现2个功能：

1.  将csv文本文件进行格式转换，存储在二进制文件中
2.  从二进制文件中读取表，打印出来


##**2. 设计思路**

设计表存储2进制文件格式如下：

文件头用`uint32_t`类型存储本表中一共有多少条记录。

对csv表格中的每条记录，在2进制文件中用前后2条数据进行存储：结构体`StuRecord` —— 学号、年龄、院系、姓名长度（含`'\0'`）和字符串数组`stuname`。

```c
struct StuRecord {
	uint8_t dpt[9];     // 8 characters at most: abbreviation of department
	uint8_t age;
	uint8_t stuid[5];
	uint8_t lenName;
};
```

每个10进制数字用4bit足够存储，因此每个`uint8_t`类型可存储两个数字，如`"39"`可存储为`"00111001"`。该转换及其逆过程分别用如下函数处理：

```c
uint8_t convert( char num1, char num2 );
//将2个数字 num1、num2 转换为 8 bit 数据

int reverse(uint8_t num, int index);
//提取 8 bit 数据 num 中的第 0 个（高4位）或第 1 个（低4位）数字
```

故10位学号用`stuid[5]`字段、2位年龄用`age`字段存储。从二进制文件中读取每条记录时，先读取的`StuRecord`结构体中的`lenName`字段存储了`stuname`的实际长度，再读取`stuname`，从而更便利。

`lenName`字段的范围限制了`stuname`最多有255个字符（含`'\0'`），因而定义`stuname`长度的上限和csv文件每行长度的上限：

```c
#define MaxLine 512
#define MaxName 255
```
主功能实现函数为：

```c
int csv2bin( const char *filecsv, const char *filebin ); 
// 实现功能1。函数中对csv文件中数据的合法性进行了检查，考虑了各种边界情况，鲁棒性较好。测试样例 test.csv 对此进行了测试

int pr_bin(const char *filebin); 
// 实现功能2
```

##**3.编译与测试**
编译方法： `make all`

测试样例： `make test`

shell中的合法执行语句样例：

```shell
./csv6 t test.csv test.bin 2>test.err
# transform contents of test.csv to test.bin
# 	for more information:
# 	"test.csv": original csv file
# 	"test.bin": tranformed bin file
# 	"test.csv.bad": broken records of csv file "test.csv"
# 	"test.err": record the dealing with the broken records or any other errors occured during process

./csv6 p test.bin
# print out contents of test.bin
```



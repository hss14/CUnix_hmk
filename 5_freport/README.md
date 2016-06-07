##**1.问题描述**

编写文件检查程序，命令格式：
`freport [--`

功能 A:如果没有选择项,则
对在pathname文件树下的所有文件, 根据自己的设计列出其全路径和主要
文件属性:
1) 超长时间没有被访问过,而且文件非常大;
2) 在指定时间段被访问过;
3) 正被其他用户阅读;
...
功能B:如果有选择项,则按选择项的要求列出,
1) 超过一定长度的文件;
2)最近若干天访问过的文件;
3)....
功能C:对于长时间没有数据更新的用户,发通知
说明:1, 至少实现4个属性,至少有2个选项(使用getopt实现)。
2, 不能使用ftw()。
3, 必须能够处理文件树带环(符号连接造成)的情况


##**2.解决思路**

##**3.编译测试**
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
##**1.功能描述**

编写文件检查程序，命令格式：
`freport [--accessed DAYS] [--size-beyond BYTES] [--users-nologin DAYS] [PATHNAME]`

-   **功能 A:**如果没有任何选择项,则对在`PATHNAME`文件树下的所有文件, 列出其全路径和是否有如下文件属性:
1.  是否超长时间（超过`NO_ACC_SECS`秒）没有被访问过;
2.  文件是否非常大（超过`BYTES_BEYOND`字节）;
3.  在指定时间段（距今`ACC_BETW[0]`秒~`ACC_BETW[1]`秒）内是否被访问过;
4.  是否正在被其他用户阅读;

-   **功能B:**如果有相应的选择项,则按选择项的要求列出`PATHNAME`文件树下满足该条件的所有文件
1.  `--size-beyond BYTES`: 长度超过`BYTES`字节的文件;
2.  `--accessed DAYS`: 最近`DAYS`天访问过的文件;

-   **功能C:**如果选项为`--users-nologin DAYS`，则列出`DAYS`天内没有login过的用户

要求说明:
1.  至少实现4个属性,至少有2个选项(使用`getopt`实现)
2.  不能使用`ftw()`
3.  必须能够处理文件树带环(符号连接造成)的情况


##**2.解决思路与代码结构**

**`report_users.c`**
```c
int report_users( int days );
//实现功能C。打印出所有`days`内没有登陆过的用户。
```
用`system`函数执行`lastlog`命令实现。
为了排除从来不登陆的系统账户的影响，限定只考察uid属于以下范围的帐号：
-   [`RANGE_USERS[0]`, `RANGE_USERS[1]`]
-       在我的系统里为[1000, 65533], 低于1000的都是系统帐号， 65534是nobody帐号。
-       不是很清楚这个数值的可移植性。
-   `UID_ROOT`（superuser帐号），一般为0


**`print_attribute.c`**
```c
int A_pr_attb( char *fullname, struct stat *buf, struct timespec *now, int opt );
//实现功能A。对文件名为 fullname 的文件（已经得到stat结构体、当前时间）打印文件名、是否有前述4个属性
int B_s_pr_huge( char *fullname, struct stat *buf, struct timespec *now, int opt );
// 如果文件长度超过 opt 字节，则打印出文件名 fullname
int B_a_pr_acced( char *fullname, struct stat *buf, struct timespec *now, int opt );
// 如果文件在 opt 天内访问过，则打印出文件名 fullname
```
这些属性大部分都可以容易地从stat结构体中得到。对功能A中的“是否正在被其他用户阅读（打开）”属性，则通过`system`
执行`lsof -u ^1000 fullname`(排除uid=1000的当前用户）结果是否为空得到。


**`hash.c`**
```c
int hashcheck( long int inode );
```
为了处理文件/目录的软/硬链接，防止重复访问与陷入回环，对`inode`除法散列建立hash表（按 `inode % SizeHashList` 的余数）记录所有访问过的文件（含目录），可根据文件树的大小选择合适的`SizeHashList`值。`traverse`访问每个文件（目录）前用此函数检查该表，若`inode`已存在，返回`1`，说明已经遍历过，直接放弃；否则返回`0`，在表中新增该`inode`，对其访问处理。


**`traverse.c`**
```c
int traverse( char *pathname, int myfun(), struct timespec *now, int opt );
```
-   通过递归调用自己，实现对pathname的全遍历。每读到一个目录，有两种遍历文件树的思路，这里采用的是第二种。
-       （1）chdir到该目录下，直接对文件名遍历
-       （2）不改变cwd，更新路径名，使用（相对初始cwd的或者绝对路径的）全路径文件名遍历
-   用`lstat`判断pathname的类型：
-       目录： 读取目录中的每个文件，递归调用traverse
-       符号链接：用readlink函数获取符号链接到的文件名，对其traverse。注意符号链接的内容可能是相对该符号链接所在目录的路径，此时需要进一步处理。
-       其他文件：调用myfun函数执行A/B功能


**`freport.c`**
```c
main.c
```
使用`getopt`库函数实现选项解析，对功能C调用`report_users`函数，对功能A/B，调用以`print_attribute.c`中相应的函数的指针为参数的`traverse`函数进行遍历、打印等操作。


##**3.编译测试**
编译方法： `make all`

测试样例： `make test`

shell中的合法执行语句样例：

```shell
	# example of legal augments:
	#
	# =======  function A as demanded =======
	# ./freport PATHNAME
	# ===> ls all files in PATHNAME and whether has the following specific features:
	# 	* "long time no access" if not accessed in the last 30 days
	# 	* "huge file" if size beyond 1M bytes
	# 	* "accessed during specific period" if accessed during the week before last
	# 	* "being read" if is being read by other users exactly when it is traversed by this program
	#
	# =======  function B as demanded =======
	# ./freport --accessed 3 PATHNAME  
	# ===> ls files in PATHNAME that have been accessed with 3 days
	#
	# ./freport --size-beyond 1000000 PATHNAME  
	# ===> ls files in PATHNAME whose size is bigger than 1MB
	#
	# ======= function C as demanded =======
	# ./freport --users-nologin 3 
	# ===> list users who has never login in the last 3 days
```
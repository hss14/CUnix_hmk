#问题描述

**定义一个分数类型如下：**

```c
typedef struct fraction {
    int numerator;
    int denominator;
} fraction;
```
**实现其四则运算操作和一个转换到浮点型的函数**

#实现框架

为了能兼容`INFINITY`和`NAN`，实际使用的数据结构如下：

```c
typedef struct fraction {
	float numerator;
	float denominator;
}fraction;
// always int indeed, except for NAN and INFINITY
```

所用函数（及调用关系）如下：

```c
// 处理shell参数读取和合法性检查，执行相应的转换为浮点数或加减乘除运算，打印算式和结果
int main()
{    
    // 构造最简（分子分母互素）分数 n/d，其中负分数的负号统一在分子n，0统一表示为0/1
    // 用INFINITY、NAN，对所有可能的输入参数给出构造结果
    fraction construct ( float n, float d);
    {
        // 默认a，b均非零，返回它们的最大公约数
        unsigned int myGCD(unsigned int a, unsigned int b);
    }
    
    // 四则运算函数，每个函数均：
    // 1. 使用construct函数，以使返回结果最简
    // 2. 用INFINITY、NAN容忍所有可能的输入
    fraction add (fraction x, fraction y);      // 返回 x+y
    fraction sub (fraction x, fraction y);      // 返回 x-y
    fraction multiply (fraction x, fraction y); // 返回 x*y
    fraction division (fraction x, fraction y); // 返回 x/y
    
    // 返回转换为浮点数的结果
    double convert(fraction x);
    
    // 打印该分数，形如 numerator/demoninator
    void print_fraction (fraction x);
    
}
```

# 编译与测试

编译方法： `make all`

测试样例： `make test`

shell中的合法执行语句：
```shell
./myfraction 66 / 2016 =
# convert to float: must enter 5 parameters seperated by space: the 2nd-numerator, the 4th-denomerator

./myfraction 1 / 2 + 1 / 3 =
# to execute the four basic arithmetic operations: must enter 9 parameters seperated by space:
# the 5th: select among '+', '-', 'x', '/' to execute corresponding operation
# the 2nd and the 4th: numerator and denomerator of the first fraction operand
# the 6th and the 8th: numerator and denomerator of the first fraction operand
```

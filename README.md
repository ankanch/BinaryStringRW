# BinRW

该算法用于读取文件二进制数据。或者从已有二进制数据恢复文件。

by Kanch -> kanchisme at gmail . com 

# 算法示例

假设有一个文件，内容如下：
```
this is a test by Kanch.
```

在经过本程序处理后，得到其二进制代码如下：
```
001011100001011010010110110011100000010010010110110011100000010010000110000001000010111010100110110011100010111000000100010001101001111000000100110100101000011001110110110001100001011001110100
```
同理，可以由以上二进制代码恢复源文件。

# 使用方法

#### 调用命令：

> brw [input-file] [output-file] [mode: -a(all),-r(range)] [l-range] [r-range]

#### 参数解释：

`input-file`: 需要处理的源文件路径

`output-file`:输出的目标文件路径，如果该项为“-”则在命令行输出结果

`mode`:处理模式，`-a` 全部读取，`-r` 按照区间读取，`-rdt` 从二进制恢复文件

`l-range`:如果模式为 -r，则这里输入读取起始位置（单位bit）

`r-range`:如果模式为 -r，则这里输入读取结束位置（单位bit，带范围检查）

**注意：如果在-r模式先仅给出一个值（l-range或者r-range），程序将会从文件起始位置读取到给定值位置。

# 其它

您可以在GPL协议下自由修改本代码。

![GPL](https://www.gnu.org/graphics/gplv3-127x51.png)



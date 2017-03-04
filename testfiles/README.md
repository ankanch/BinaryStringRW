# BinRW 算法
该算法的作用是用于分析文件的二进制数据

# 文件夹说明

这里的文件主要是测试文件，用来测试算法的正确性。

`r`开头的文件均为读取结果文件(数据为二进制字符串)

诸如`rxr`的文件均为恢复结果文件

诸如`tix`的文件均为测试源文件

`TEST CMD.txt`中包含了测试用的命令行

# 测试语句

```
【读取二进制txt测试】
brw.exe testfiles\ti1.txt testfiles\r1.txt -a

【恢复二进制txt恢复测试】
brw.exe testfiles\r1.txt testfiles\r2r.txt -rdt

【读取二进制doc测试】
brw.exe testfiles\ti3.docx testfiles\r3.txt -a

【恢复二进制doc恢复测试】
brw.exe testfiles\r3.txt testfiles\r3r.docx -rdt

```

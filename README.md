# BinaryStringRW
####read out binary as string and write binary (string) in files to reduct.
####本算法用来测试将二进制以字符串的形式读出与将字符串（二进制形式）写入原文件，保持数据不变
------------------------------
####this algorithm is used to read file's binary out as string then write those binary(string) into a new file to reduct the raw file
by Kanch @2015-10-4     
http://akakanch.com    
kanch@akakanch.com   
core algorithm included in BinaryStringRWAlgorithm files :)
.
------------------------------------------
######NOTICE
######You may need set up files below to make this main function works
const char* testfilepath = "C:\\binalgtest\\rawfile.txt";      //the raw file path(file you used to read out binary)   
const char* binarydatapath = "C:\\binalgtest\\binstring.txt";    //binary string path(file used to save binary-string)   
const char* targetfilepath = "C:\\binalgtest\\reductionfile.txt";    //reduction file via a known binary string   
.
-----------------------------------------------
you may need doc below to understand some points of this algorithm
https://docs.google.com/drawings/d/1odXlKXx6MggQvDeObna4wYsEd_kVF23HoaM_Si7z32M/edit?usp=sharing
.
---------------------------------------
any way,hope this algorithm can help you.
I mean,when you read a file's binary out,you know,you can analyize it,then,you know what can you do with it.
but there are still some problems,like portable,or the effectiveness.
so I hope you guys can help me to fix it,make it much better. :)

#include <iostream>
#include <string>
#include <string.h>
#include <fstream> 
#include <stdio.h>
#include <stdlib.h>

using namespace std;

char MODE_ALL[] = "-a";
char MODE_RANGE[] = "-r";
char MODE_REDUCT[] = "-rdt";

string STRING_USAGE = "\n\t\tBinRW v1.0 by Kanch\nThis algorithm is used to lookup binary data of any file.\n"
                        "---\nUsages:\n\n\tbrw [input-file] [output-file] [mode: -a(all),-r(range)] [l-range] [r-range]\n"
                        "\n\t1. -a means read all the binary data of a file,"
                        "\n\t    in this case, l-range and r-range will be igonred."
                        "\n\t2. -r means read certain range binary data of a file,"
                        "\n\t    range can be assigned by l-range(beg,include) and r-range(end,exclude),"
                        "\n\t    if only one range wether l or r has been specified,application"
                        "\n\t    will read from beginning of the file to the specified range -> [0,range). "
                        "\n\t3. -rdt means use input-file binary data to reduct the original file to the "
                        "\n\t    output-file destination."
                        "\n\t**input `-` in output-file field if you'd like to print the result(only works for -a & -r).'"
                        "\n---\n";

string STRING_INVAILD = "\n invaild parameters.\n run brw.exe to see usages. \n brw exit.\n";
string STRING_ERROR_OPEN = "\n an error occured when open file.\n please check wether the file exist. \n brw exit.\n";
string STRING_ERROR_RANGE = "\n range error!\n upper bound must bigger than the lower bound! \n brw exit.\n";
string STRING_ERROR_RANGE_FILE = "\n range error!\n r-range exceeds the length of file. \n brw exit.\n";


unsigned char CMPARY[] = {0x1,0x2,0x4,0x8,0x10,0x20,0x40,0x80};
const int CMPARYLENGTH = 8;

//必要函数声明
inline const string char2bin(const unsigned char ch);
const string readBinary(const string filepath ,const int lr=0,const int rr=0);
const int writeBinary(const string filepath,const string & binData);
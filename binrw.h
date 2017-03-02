#include <string>
#include <string.h>
#include <fstream>
#include <malloc.h>  //for _msize() function
#include <stdio.h>

using namespace std;

char MODE_ALL[] = "-a";
char MODE_RANGE[] = "-r";

string STRING_USAGE = "\n\t\tBinRW by Kanch\nThis algorithm is used to lookup binary data of any file.\n"
                        "---\nUsages:\n\n\tbrw [input-file] [output-file] [mode: -a(all),-r(range)] [l-range] [r-range]\n"
                        "\n\t1. -a means read all the binary data of a file,"
                        "\n\t    in this case, l-range and r-range will be igonred."
                        "\n\t2. -r means read certain range binary data of a file,"
                        "\n\t    range can be assigned by l-range(beg) and r-range(end),"
                        "\n\t    if only one range wether l or r has been specified,application"
                        "\n\t    will read from beginning of the file to the specified range. "
                        "\n---\n";

string STRING_INVAILD = "\n invaild parameters.\n run brw.exe to see usages. \n brw exit.\n";

unsigned char CMPARY[] = {0x1,0x2,0x4,0x8,0x10,0x20,0x40,0x80};
const int CMPARYLENGTH = 8;

//必要函数声明
const string readBinary(const string filepath );
const string writeBinary(const string filepath,const string & binData);
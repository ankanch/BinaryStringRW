#include <iostream>
#include <fstream>
#include <string>
#include <malloc.h>  //for _msize() function
#include "BinaryStringRWAlgorithm.h"
#include <ctime>

/*
@zh-CN
本算法用来测试将二进制以字符串的形式读出与将字符串（二进制形式）写入原文件，保存数据不变
by Kanch @ 2015 10-4
http://akakanch.com
@en-us
this algorithm is used to read file's binary out as string then write those binary(string) into a new file to reduct the raw file
by Kanch @2015-10-4
mailto://kanch@akakanch.com
---------------------------------------
this file is used to test wether this algorithm works properly.
the core algorithm is in BinaryStringRWAlgorithm.h and .cpp file
for more infomation please read readme.md file 
any way,hope this algorithm can help you.
*/


using namespace std;


//test infomation print function
template<class T>
inline void TEST(const T dta)
{
	cout << dta << endl;
}

//essential specific binary bit varibles note:short should no smaller than char,so, minium is 16 bit,2 bytes;
//array below with 32 elements,like 32 bits,4 bytes.
/*/
short SV[32] = { 0x1,0x2,0x4,0x8,
0x10,0x20,0x40,0x80,
0x100,0x200,0x400,0x800,
0x1000,0x2000,0x4000,0x8000,
0x10000,0x20000,0x40000,0x80000,
0x100000,0x200000,0x400000,0x800000,
0x1000000,0x2000000,0x4000000,0x8000000,
0x10000000,0x20000000,0x40000000,0x80000000,
};
/*/
short SV[32] = { 0x1,0x2,0x4,0x8,
0x10,0x20,0x40,0x80,
0x100,0x200,0x400,0x800,
0x1000,0x2000,0x4000,0x8000
};
//for this algorithm can run successfully on another systems,we have to know the length of type short in specfic systems.
int slen = sizeof(short);   //slen is the len of type short
int sbitlen = slen * 8;     //sbitlen is the bits's sum of short io specfic systems.
short * bitdta;   //used to save specfic length of short's  bit data


//some essential test varibles
const char* testfilepath = "C:\\binalgtest\\rawfile.txt";      //the raw file path
const char* binarydatapath = "C:\\binalgtest\\binstring.txt";    //binary string path
const char* targetfilepath = "C:\\binalgtest\\reductionfile.txt";    //reduction file via a known binary string(above 1)

/*///pics
const char* testfilepath = "C:\\binalgtest\\pictest\\rawfile.jpg";      //the raw file path
const char* binarydatapath = "C:\\binalgtest\\pictest\\binstring.txt";    //binary string path
const char* targetfilepath = "C:\\binalgtest\\pictest\\reductionfile.jpg";    //reduction file via a known binary string(above 1)
/*/
int main(int argc, char**argv)
{
	BinaryStringRW sw;

	cout << "slen= "; TEST(slen);
	cout << "sbitlen= "; TEST(sbitlen);
	cout << "(sizeof(SV) / sizeof(SV[0]))= "; TEST((sizeof(SV) / sizeof(SV[0])));

	TEST("call determinBitData");
	sw.determinBitData(sbitlen);
	cout << "(_msize(bitdta) / sizeof(bitdta[0]))= "; TEST((sizeof(bitdta) / sizeof(bitdta[0])));
	cout << "data of bitdta[]:" << endl;

	TEST("[S1-----now start test readBinary function(no reverse)-------");	
	//cout << "readBinary()= " << sw.readBinary(testfilepath) << endl;
	//cout << "readBinaryS()= " << sw.readBinaryS(testfilepath) << endl;
	
	
	TEST("[S1-----readBinary function test completed,please check result------\n\n\n\n");

	TEST("[S2-----now start test writeBinary function------");
	//ofstream a("C:\\binalgtest\\binaryR.txt");
	//ofstream b("C:\\binalgtest\\binaryRS.txt");
	//a << sw.readBinary(testfilepath);
	//b << sw.readBinaryS(testfilepath);
	//a.close(); b.close();
	//string a = sw.readBinaryS(testfilepath);
	//cout << a << endl;
	//sw.writeBinary(targetfilepath, sw.readBinaryS(testfilepath));
	sw.writeBinaryS(targetfilepath, sw.readBinaryS(testfilepath));
	//sw.writeBinary(targetfilepath, sw.readBinary(testfilepath));
	//cout << endl << endl << endl << endl <<"now start test readBinary's time and readBinaryS' time" <<endl;
	//int time_s = clock();
	//sw.readBinary(testfilepath);
	//int time_a = clock() - time_s;
	//sw.readBinaryS(testfilepath);
	//int time_b = clock() - (time_a + time_s);
	//cout << "readBinary's time= " << time_a << endl << "readBinaryS' time=" << time_b << endl<<endl;
	/*/
	TEST("[S2-----writeBinary function test completed,please check result------");
	TEST("[S3-----we will use readBinary function to read out the bits of targetfile------");
	("[S3-call readBinary");
	TEST("[S3--result:");
	//cout << sw.readBinary(targetfilepath) << endl;
	TEST("[S3-----plesae check the output,to see if writeBinary function works properly------");
	TEST("[\n\n\n\nS4-----BinaryStringRW algorithm all test completed------\n\n\n");
	/*/

	delete[] bitdta;
	return 0;
}


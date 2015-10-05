#include <iostream>
#include <fstream>
#include <string>
/*
本算法用来测试将二进制以字符串的形式读出与将字符串（二进制形式）写入原文件，保存数据不变
by Kanch @ 2015 10-4
http://akakanch.com
*/


using namespace std;


//test infomation print function
template<class T>
inline void TEST(const T dta)
{
	cout << dta << endl;
}


//essential function definations
const string readBinary(const string filepath);                              //该函数实现将二进制以字符串的形式读出

const string writeBinary(const string filepath,const string & binData);      //该函数实现将字符串形式二进制写入文件

void determinBitData(const int sbitlen);                                    //该函数用来决定位数组大小

//essential specific binary bit varibles note:short should no smaller than char,so, minium is 16 bit,2 bytes;
//array below with 32 elements,like 32 bits,4 bytes.
short SV[] = { 0x0,0x1,0x2,0x3,
			   0x4,0x5,0x6,0x7,
			   0x8,0x9,0xA,0xB,
			   0xC,0xD,0xE,0xF,
			   0x0F,0x1F,0x2F,
			   0x3F,0x4F,0x5F,
			   0x6F,0x7F,0x8F,
			   0x9F,0xAF,0xBF,
			   0xCF,0xDF,0xEF,0xFF };

//for this algorithm can run successfully on another systems,we have to know the length of type short in specfic systems.
int slen = sizeof(short);   //slen is the len of type short
int sbitlen = slen * 8;     //sbitlen is the bits's sum of short io specfic systems.
short * bitdta;   //used to save specfic length of short's  bit data


//some essential test varibles
const char* testfilepath = "C:\\binalgtest\\rawfile.txt";      //the raw file path
const char* binarydatapath = "C:\\binalgtest\\binstring.txt";    //binary string path
const char* targetfilepath = "C:\\binalgtest\\reductionfile.txt";    //reduction file via a known binary string(above 1)

int main(int argc, char**argv)
{

	TEST(slen);
	TEST(sbitlen);
	TEST((sizeof(SV) / sizeof(SV[0])));
	TEST("[S1-----now start test readBinary function-------");
	readBinary(testfilepath);
	TEST("[S1-----readBinary function test completed,please check result------");

	return 0;
}


//specfic functions definations
//
void determinBitData(const int sbitlen)                          
{
	if (sbitlen <= (sizeof(SV)/sizeof(SV[0])))
	{
		bitdta = new short[sbitlen];
		for (int i = 0; i < sbitlen; i++)
		{
			bitdta[i] = SV[i];
		}
	}
	else
	{
		cout << "out of range" << endl;
	}
}

const string readBinary(const string filepath)
{
	ifstream read(filepath,ios_base::binary|ios_base::in|ios_base::ate);  //open specfic file,then move pointer to the end of file
	string binData = "";
	int fileylen = read.tellg();   //tell  the len of the file (bytes)
	read.seekg(0, ios_base::beg);  //reduct the pointer to the begin of the file in case of steps behind read nothing out :(
	int fileblen = fileylen * 8;
	short buf = 0;
	cout << "file length(from tellg())= " << fileylen << " bytes"<<endl;
	cout << "file length(from tellg())= " << fileblen << " bits" << endl;
	read.read((char*)&buf, sizeof(short));
	cout << "after read from file,short buf = " << buf << endl;


	read.close();
	return binData;
}

const string writeBinary(const string filepath,const string & binData)
{
	string a = &binData[0];

	return a;
}
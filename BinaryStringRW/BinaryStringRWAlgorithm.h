#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <malloc.h>  //for _msize() function
using namespace std;


/*
this file is BinaryStringRW algorithm core file
*/
/*
@zh-CN
���㷨�������Խ����������ַ�������ʽ�����뽫�ַ�������������ʽ��д��ԭ�ļ����������ݲ���
by Kanch @ 2015 10-4
http://akakanch.com
@en-us
this algorithm is used to read file's binary out as string then write those binary(string) into a new file to reduct the raw file
by Kanch @2015-10-4
mailto://kanch@akakanch.com
*/

class BinaryStringRW 
{
	public:
		//essential function definations
		const string readBinary(const string filepath);                              //�ú���ʵ�ֽ����������ַ�������ʽ����
		const string readBinaryS(const string filepath);							//����ͬ�ϣ��������Ч�ʸ���
		const string writeBinary(const string filepath, const string & binData);      //�ú���ʵ�ֽ��ַ�����ʽ������д���ļ�

		void determinBitData(const int sbitlen);                                    //�ú�����������λ�����С

	private:

	//essential specific binary bit varibles note:short should no smaller than char,so, minium is 16 bit,2 bytes;
	//array below with 32 elements,like 32 bits,4 bytes.
/*/this will cause error C2397: �ӡ�int��ת������short����Ҫ����ת��,code blew
	short SV[32] = { 0x1,0x2,0x4,0x8,
					0x10,0x20,0x40,0x80,
					0x100,0x200,0x400,0x800,
					0x1000,0x2000,0x4000,0x8000,
					0x10000,0x20000,0x40000,0x80000,
					0x100000,0x200000,0x400000,0x800000,
					0x1000000,0x2000000,0x4000000,0x8000000,
					0x10000000,0x20000000,0x40000000,0x80000000,
	};
	short SV[32] = { 0x1,0x2,0x4,0x8,
				0x10,0x20,0x40,0x80,
				0x100,0x200,0x400,0x800,
				0x1000,0x2000,0x4000,0x8000
		};
	/*/
	//why this? because I can't compile successfully in VS2015 without this :(
	//I don't even know why the same array in BinaryStringRW.cpp won't cause error C2397,but the array in this class caused :(
	//if you know why,get in touch with me -> kanchisme@gmail.com
	short SV[32] = { short(0x1),short(0x2),short(0x4),short(0x8),
			short(0x10),short(0x20),short(0x40),short(0x80),
			short(0x100),short(0x200),short(0x400),short(0x800),
			short(0x1000),short(0x2000),short(0x4000),short(0x8000)
		};
	//for this algorithm can run successfully on another systems,we have to know the length of type short in specfic systems.
	int slen = sizeof(short);   //slen is the len of type short
	int sbitlen = slen * 8;     //sbitlen is the bits's sum of short io specfic systems.
	short * bitdta;   //used to save specfic length of short's  bit data
};

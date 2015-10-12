#include "BinaryStringRWAlgorithm.h"
//specfic functions definations
//
void BinaryStringRW::determinBitData(const int s)
{
	if (s <= (sizeof(SV) / sizeof(SV[0])))
	{
		bitdta = new short[s];
		for (int i = 0; i < s; i++)
		{
			bitdta[i] = SV[i];
		}
	}
	else
	{
		cout << "out of range" << endl;
	}
}

//more efficientive version
const string BinaryStringRW::readBinary(const string filepath)
{
	ifstream read(filepath, ios_base::binary | ios_base::in | ios_base::ate);  //open specfic file,then move pointer to the end of file
	string binData = "", binBuf = "";
	int fileylen = read.tellg();   //tell  the len of the file (bytes)
	read.seekg(0, ios_base::beg);  //reduct the pointer to the begin of the file in case of steps behind read nothing out :(
	int fileblen = fileylen * 8;
	short buf = 0, bbuf = 0;
	int looprounds = 0;
	int lastshortbitscover = 0;  //#V1
								 ///*/
								 //determine how much loops need to read out all the bits;
	if ((fileylen%sizeof(short)))  //#1
	{
		looprounds = (fileylen / sizeof(short)) + 1;
		lastshortbitscover = (fileylen%sizeof(short)) * 8; //rember,we need bits instead of byte :(
	}
	else                            //#2
	{
		looprounds = (fileylen / sizeof(short));
	}
	//start reading,
	//notice:if #1 was excuted,we need to use a varible to flag how much bits in the last short are really in raw file
	//so,this varible is #V1
	//pocess below doesn't reverse the bits read;if we reverse it,changes should also be make in writeBinary function
	for (int i = 0; i < looprounds; i++)
	{
		read.read((char*)&buf, sizeof(short));
		if (i < (looprounds - 1))
		{
			for (int m = 0; m < sbitlen; m++)
			{
				if ((buf & bitdta[m]) == bitdta[m])
				{
					binBuf += "1";
				}
				else
				{
					binBuf += "0";
				}
			}
		}
		else
		{
			for (int m = 0; m < lastshortbitscover; m++)
			{
				if ((buf & bitdta[m]) == bitdta[m])
				{
					binBuf += "1";
				}
				else
				{
					binBuf += "0";
				}
			}
		}
		//I'm thinking wether should I reverse the bits read to a acceptable format,at least,for we huamn
		//what's a acceptable format?look here:https://docs.google.com/drawings/d/1odXlKXx6MggQvDeObna4wYsEd_kVF23HoaM_Si7z32M/edit?usp=sharing
		//if necessary to reverse,add code here(above)
		binData += binBuf;
		binBuf = "";
	}
	///*/

	//TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST  START
	//test code ,to test this if this function works properly
	cout << "file length(from tellg())= " << fileylen << " bytes" << endl;
	cout << "file length(from tellg())= " << fileblen << " bits" << endl;
	read.seekg(0, ios_base::beg);
	read.read((char*)&buf, sizeof(short));
	read.read((char*)&bbuf, sizeof(short));
	cout << "after read from file,short buf = " << buf << endl << "bbuf = " << bbuf << endl;
	cout << "looprounds= " << looprounds << endl;
	cout << "lastshortbitscover= " << lastshortbitscover << endl;
	//TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST  END



	read.close();
	return binData;
}


/*/
important notice:because I haven't reverse the bits read out in readBinary function,
so when you make changes in writeBinary,take care of bits' sequence :)
in order to write bits completely,I may use more than one types to write.
/*/
const string BinaryStringRW::writeBinary(const string filepath, const string & binData)
{
	ofstream write(filepath, ios_base::out | ios_base::binary);
	string a = binData, sbuf = "";
	short buf = 0;
	int looprounds = 0, lastshortbitscover = 0;
	//determines how many loops do we need,and the last type to use to write
	if ((binData.length() % sbitlen) == 0)
	{
		looprounds = binData.length() / sbitlen;
	}
	else
	{
		looprounds = binData.length() / sbitlen + 1;
		lastshortbitscover = binData.length() % sbitlen;     //this is very essencial to ensure the complatence of data,this derterines last type to write
	}
	if (lastshortbitscover != 0 && lastshortbitscover % 8 == 0)         //seems there are some bits left,so we need to decide which type to use to write the last varible
	{
		for (int i = 0; i < looprounds; i++)
		{
			if (i < (looprounds - 1)) //#9
			{
				sbuf = a.substr(sbitlen*i, sbitlen);  //read sizeof(short) 's char 0 or 1
				for (int m = 0; m < sbitlen; m++)
				{
					if (sbuf[m] == '1')   //now set the bit of short
					{
						buf = (buf | bitdta[m]);
					}

				}
				write.write((char*)&buf, sizeof(short));
				buf = 0;        //set all bits to 0
				sbuf = "";

			}
			else     //this is used to write the last varible,I don't know use which type,so we need to determine by length
			{
				sbuf = a.substr(sbitlen*i, a.length() - sbitlen*i); //the last bits of data
																	//if there really have some bits left,it should be 8 bits,so,we use char to write the last bits
				char lastbits = 0;
				for (int m = 0; m < sbuf.length(); m++)
				{
					if (sbuf[m] == '1')
					{
						lastbits = (lastbits | bitdta[m]);
					}
				}
				write.write((char*)&lastbits, sizeof(char));

			}
		}
	}
	else if (lastshortbitscover == 0)     //no bits left for short?so,we just need to write
	{
		for (int i = 0; i < looprounds; i++)   //code copied from #9
		{
			sbuf = a.substr(sbitlen*i, sbitlen);  //read sizeof(short) 's char 0 or 1
			for (int m = 0; m < sbitlen; m++)
			{
				if (sbuf[m] == '1')   //now set the bit of short
				{
					buf = (buf | bitdta[m]);
				}

			}
			write.write((char*)&buf, sizeof(short));
			buf = 0;        //set all bits to 0
			sbuf = "";
		}
	}

	write.close();
	return a;
}


const string BinaryStringRW::readBinaryS(const string filepath)
{
	ifstream read(filepath,ios_base::in);  //open specfic file,then move pointer to the end of file to get file length
	string rawdata = "", CLbuf = ""; ///used to save the data of file as string
	while (!read.eof())
	{
		read >> CLbuf;   //for efficient,I have to read file data all out to a varible as string,cuz readBinary() is waste of time on disk IO
		rawdata += CLbuf;
	}
	string binData = "", binBuf = "";
	int loopperchar = sizeof(rawdata[0])*8;    //determine how much loops need to read out all the bits;
	for (int i = 0; i < rawdata.length(); i++)   //now starting read bits
	{
		//start reading,
		//pocess below doesn't reverse the bits read;if we reverse it,changes should also be make in writeBinary function
				for (int m = 0; m < loopperchar; m++)
				{
					if ((rawdata[i] & bitdta[m]) == bitdta[m])
					{
						binBuf += "1";
					}
					else
					{
						binBuf += "0";
					}
				}
		binData += binBuf;
		binBuf = "";
	}
	//I'm thinking wether should I reverse the bits read to a acceptable format,at least,for we huamn
	//what's a acceptable format?look here:https://docs.google.com/drawings/d/1odXlKXx6MggQvDeObna4wYsEd_kVF23HoaM_Si7z32M/edit?usp=sharing
	//if necessary to reverse,add code here(above)
	
	///*/

	//TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST  START
	//test code ,to test this if this function works properly
	cout << "after read from file,string rawdata  = " << rawdata << endl;
	//TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST  END



	read.close();
	return binData;
}
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


////more efficientive version,lower memory&time cost
const string BinaryStringRW::readBinaryS(const string filepath)
{
	string binData = "", binBuf = "";
	ifstream in(filepath, ios_base::in | ios_base::ate | ios_base::binary);
	int bytelen = in.tellg();     //get the byte size of the file
	in.close();
	FILE* stream;
	const int MAX_BUFFER = 12000;   //a 12000bytes(11.7 kb) buffer
	char buf[MAX_BUFFER] = "";
	int len=0,roundsread=0,bytesleft=-1,bufi=0;            //roundsread used to store how many times need to read,bytesleft record how many bytes left in last rounds' reading
	//varible avove bufi is used to save if is the last buf[]
	//determine how many buffers need
	if (bytelen % MAX_BUFFER == 0) // bytelen = k * MAX_BUFFER ,so we just need k times reading
	{
		roundsread = bytelen / MAX_BUFFER;
	}
	else if (bytelen % MAX_BUFFER !=0)  //so we need k+1 times reading
	{
		//if it is the last buffer to read && size doesn't equ to MAX_BUFFER
		roundsread = bytelen / MAX_BUFFER + 1;
		bytesleft = bytelen % MAX_BUFFER;
	}
	for (int i = 0; i < roundsread; i++)  //this loop keep reading from file
	{
		//start reading
		if ((stream = fopen(filepath.c_str(), "rb")) == NULL)
		{
			fprintf(stderr, "Cannot open input file.\n");
			return 0;
		}
		len = fread(buf, 1, MAX_BUFFER, stream);
		//fseek(stream, MAX_BUFFER, i*MAX_BUFFER);
		//cout << "len= " << len << "-bytelen= " << bytelen << "-bytesleft=" << bytesleft << endl;
		//start pocessing
		if (i != roundsread-1 )    ///we have to know if it is the last Buffer to read
		{
			for (int t = 0; t < MAX_BUFFER; t++)    //pocessing k buffer
			{
				for (int r = 0; r < sizeof(char); r++)
				{
					if (buf[t] & CV[r] == CV[r])
					{
						binBuf += "1";
						continue;
					}
					binBuf += "0";
				}
				binData += binBuf;
				binBuf = "";
			}
		}
		else if (i == roundsread - 1)
		{
			for (int t = 0; t < bytesleft; t++)   //pocessing the last buffer
			{
				for (int r = 0; r < sizeof(char); r++)
				{
					if (buf[t] & CV[r] == CV[r])
					{
						binBuf += "1";
						continue;
					}
					binBuf += "0";
				}
				binData += binBuf;
				binBuf = "";
			}
		}
	}

	fclose(stream);
	return binData;
}

//second way to write binary data for readBinaryS function
const string BinaryStringRW::writeBinaryS(const string filepath, const string & binData)
{
	string a = binData, sbuf = "";
	FILE* stream;
	const int MAX_BUFFER = 12000;   //a 12000bytes(11.7 kb) buffer
	char buf[MAX_BUFFER] = "", x = 0;
	int len = 0, roundswrite = 0, bytesleft = -1, bufi = 0;            //roundsread used to store how many times need to read,bytesleft record how many bytes left in last rounds' reading
																	  //varible avove bufi is used to save if is the last buf[]
																	  //determine how many buffers need
	//open file buffer
	if ((stream = fopen(filepath.c_str(), "rb")) == NULL)
	{
		fprintf(stderr, "Cannot open input file.\n");
		return 0;
	}

	if (binData.length() % MAX_BUFFER == 0) // bytelen = k * MAX_BUFFER ,so we just need k times reading
	{
		roundswrite = binData.length() / MAX_BUFFER;
	}
	else if (binData.length() % MAX_BUFFER != 0)  //so we need k+1 times reading
	{
		//if it is the last buffer to read && size doesn't equ to MAX_BUFFER
		roundswrite = binData.length() / MAX_BUFFER + 1;
		bytesleft = binData.length() % MAX_BUFFER;
	}
	for (int i = 0; i < roundswrite; i++)  //this loop keep reading from file
	{
		//start pocessing
		if (i != roundswrite - 1)    ///we have to know if it is the last Buffer to read
		{
			for (int t = 0; t < MAX_BUFFER; t++)    //pocessing k buffer
			{
				for (int r = 0; r < sizeof(char); r++)
				{
					if (binData[i*MAX_BUFFER + t] == '1')
					{
						x = x | CV[r];
						continue;
					}
				}
				buf[t] = x;
				x = 0;
			}
			//write data to file;
			fwrite(buf, 1, MAX_BUFFER, stream);
		}
		else if (i == roundswrite - 1)
		{
			for (int t = 0; t < bytesleft; t++)   //pocessing the last buffer
			{
				for (int r = 0; r < sizeof(char); r++)
				{
					if (binData[i*MAX_BUFFER + t] == '1')
					{
						x = x | CV[r];
						continue;
					}
				}
				buf[t] = x;
				x = 0;
			}
			//write data to file;
			fwrite(buf, 1, bytesleft, stream);
		}
	}

	fclose(stream);

	return a;
}
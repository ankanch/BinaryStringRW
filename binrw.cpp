#include <iostream>
#include "binrw.h"



int main(int argc,char**argv)
{
    /*参数说明 :
    *           1            2                  3                 4          5
    * brw [input-file] [output-file] [mode: -a(all),-r(range)] [l-range] [r-range]
    * 最后两个参数仅在 第三个参数为 -r 时有效。
    */
    if(argc == 1){
        cout<<STRING_USAGE<<endl;
    }else if( argc == 4){
        //选中为读取整个文件的二进制代码
        if( strcmp(argv[3],MODE_ALL) != 0 ){
            cout<<STRING_INVAILD<<endl;
            return -1;
        }
        //开始处理
        string result = readBinary(argv[1]);
        cout<<"---\nresult:\n"<<result<<endl;

    }else if( argc == 6 or argc == 5){
        //选中为读取指定区间二进制代码，如果为5个参数，则默认为从 0 到 r-range
        //如果为6个参数，则为 l-range 到 r-range
        if( strcmp(argv[3],MODE_RANGE) != 0 ){
            cout<<STRING_INVAILD<<endl;
            return -1;
        }
        //开始处理
        if(argc == 5){
            //5个参数，从 0 到 r-range
            string result = readBinary(argv[1],0,atoi(argv[4]));
            cout<<"---\nresult:\n"<<result<<endl;
        }else if(argc == 6){
            //6个参数，从 l-range 到 r-range
            string result = readBinary(argv[1],atoi(argv[4]),atoi(argv[5]));
            cout<<"---\nresult:\n"<<result<<endl;
        }

    }else{
        //错误
        cout<<STRING_INVAILD<<endl;
    }
    return 0;
}

const string readBinary(const string filepath ,const int lr,const int rr)
{
    ifstream read(filepath, ios_base::binary | ios_base::in | ios_base::ate); 
    if( !read ){
        cout<<STRING_ERROR_OPEN<<endl;
        return "ERROR IN I/O";
    }
	string binData = "";
	int filelen = read.tellg();
    int filebinlen = filelen * 8;     
	read.seekg(0, ios_base::beg);
    unsigned char bicc = 0;  // stands for basic container 
    //开始处理
    cout<<"\nprocessing...  ";
    //当设置为读取全部数据的时候
    if(lr == rr && lr == 0){
        while( !read.eof() ){
            read>>bicc;
            binData += char2bin(bicc);
        }
    }else{
        //当设置为读取指定区间的时候
        //判断上下界是否合法
        if(rr <= lr){
            cout<<STRING_ERROR_RANGE<<endl;
            return "RANGE ERROR 1";
        }
        //判断上界是否超出文件长度
        if(rr > filebinlen){
            cout<<STRING_ERROR_RANGE_FILE<<endl;
            return "RANGE ERROR 2";
        }

        int nread = rr - lr;  //reading range
        int bitsskips = lr%8;
        nread += bitsskips;   //由于会错位，这里采取的策略是多读取要错位的那麽多位，最后再从最终字符串整体右移
        int loops = nread / 8;
        int loopsleft = nread % 8;
        read.seekg(lr/8, ios_base::beg);  // move to reading begin pos
        for(int i=0;i<loops;i++){
            read>>bicc;
            binData += char2bin(bicc);
        }
        //处理剩余字节
        string binBuf = "";
        read>>bicc;
        for(int i=loopsleft;i>0;i--){
            //cout<<"\rprocessing...  ";
            if( (bicc & CMPARY[i-1])== CMPARY[i-1] ){
                binBuf = "1" + binBuf;
            }else{
                binBuf = "0" + binBuf;
            }
        }
        binData += binBuf;
        binData = binData.substr(bitsskips,binData.length()-bitsskips);
    }
    cout<<"\tdone\n";
    
    read.close();
    return binData;
}
const string writeBinary(const string filepath,const string & binData)
{
    return "";
}

inline const string char2bin(const unsigned char ch)
{
    string binBuf = "";
    for(int i=0;i<CMPARYLENGTH;i++){
            //cout<<"\rprocessing...  ";
            if( (ch & CMPARY[i])== CMPARY[i] ){
                binBuf += "1";
            }else{
                binBuf += "0";
            }
    }
    return binBuf;
}
#include <iostream>
#include "binrw.h"



int main(int argc,char**argv)
{
    /*参数说明 :
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

    }else if( argc == 6 or argc == 5){
        //选中为读取指定区间二进制代码，如果为5个参数，则默认为从 0 到 l-range
        //如果为6个参数，则为 l-range 到 r-range
        if( strcmp(argv[3],MODE_RANGE) != 0 ){
            cout<<STRING_INVAILD<<endl;
            return -1;
        }
        //开始处理

    }else{
        //错误
        cout<<STRING_INVAILD<<endl;
    }
    return 0;
}

const string readBinary(const string filepath )
{
    ifstream read(filepath, ios_base::binary | ios_base::in | ios_base::ate);  
	string binData = "", binBuf = "";
	int filelen = read.tellg();     
	read.seekg(0, ios_base::beg); 
	int filebinlen = filelen * 8;   //得到文件二进制长度
    //开始处理
    
    read.close();
    return binData;
}
const string writeBinary(const string filepath,const string & binData)
{
    return "";
}
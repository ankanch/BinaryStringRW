#include "binrw.h"

static unsigned long GL_FILEBINLEN = 0;  //储存总共需要处理的二进制长度，用于用户进度更新线程
static unsigned long GL_BIN_PROCESSED = 0; //储存当前处理了多少二进制数据，用于用户进度更新线程
static bool finished = false;   //标记任务是否完成

int main(int argc,char**argv)
{
    /*参数说明 :
    *           1            2                  3                 4          5
    * brw [input-file] [output-file] [mode: -a(all),-r(range)] [l-range] [r-range]
    * 最后两个参数仅在 第三个参数为 -r 时有效。
    */
    if(argc == 1){
        cout<<STRING_USAGE<<endl;
    }else if( argc == 4 || strcmp(argv[3],MODE_ALL) == 0 || strcmp(argv[3],MODE_REDUCT) == 0  || strcmp(argv[3],MODE_HEX) == 0){
        //选中为读取整个文件的二进制代码
        if( strcmp(argv[3],MODE_ALL) == 0 || strcmp(argv[3],MODE_HEX) == 0){
            //开始处理
            string result = readBinary(argv[1]);
            if(strcmp(argv[3],MODE_HEX) == 0){
                result = convert2Hex(result);
            }
            if(strcmp(argv[2],"-") == 0){
                cout<<"---\nresult:\n"<<result<<endl;
            }else{
                ofstream write(argv[2],ios_base::out);
                write<<result;
                write.close();
            }
        }else if( strcmp(argv[3],MODE_REDUCT) == 0 ){
            ifstream read(argv[1],ios_base::in);
            string bindata;
            read>>bindata;
            read.close();
            writeBinary(argv[2],bindata);
        }else{
            cout<<STRING_INVAILD<<endl;
            return -1;
        }

    }else if( argc == 6 or argc == 5 or strcmp(argv[3],MODE_RANGE) == 0){
        //选中为读取指定区间二进制代码，如果为5个参数，则默认为从 0 到 r-range
        //如果为6个参数，则为 l-range 到 r-range
        if( strcmp(argv[3],MODE_RANGE) != 0 ){
            cout<<STRING_INVAILD<<endl;
            return -1;
        }
        string result = "";
        //开始处理
        if(argc == 5){
            //5个参数，从 0 到 r-range
            result = readBinary(argv[1],0,atoi(argv[4]));
        }else if(argc == 6){
            //6个参数，从 l-range 到 r-range
            result = readBinary(argv[1],atoi(argv[4]),atoi(argv[5]));
        }
        if(strcmp(argv[2],"-") == 0){
            cout<<"---\nresult:\n"<<result<<endl;
        }else{
            ofstream write(argv[2],ios_base::out);
            write<<result;
            write.close();
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
    unsigned long filebinlen = filelen * 8;
    GL_FILEBINLEN = filebinlen;
    unsigned long bitsread = 0;     
	read.seekg(0, ios_base::beg);
    unsigned char bicc = 0;  // stands for basic container 
    pthread_t progressthread;
    //开始处理
    cout<<"\nprocessing...  "<<endl;
    size_t binDataOffset = 0;
    //当设置为读取全部数据的时候
    if(lr == rr && lr == 0){
        binData.resize(filebinlen);
        pthread_create(&progressthread, NULL, showProgressT, NULL);
        const size_t BUFFER_SIZE = 8192;
        char buffer[BUFFER_SIZE];
        while (read.read(buffer, BUFFER_SIZE) || read.gcount() > 0) {
            size_t count = read.gcount();
            for (size_t i = 0; i < count; ++i) {
                bicc = static_cast<unsigned char>(buffer[i]);
                GL_BIN_PROCESSED += 8;
                char2bin_inline(bicc, binData, binDataOffset);
            }
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

        pthread_create(&progressthread, NULL, showProgressT, NULL);
        int nread = rr - lr;  //reading range
        GL_FILEBINLEN = nread;
        int bitsskips = lr%8;
        nread += bitsskips;   //由于会错位，这里采取的策略是多读取要错位的那麽多位，最后再从最终字符串整体右移
        int loops = nread / 8;
        int loopsleft = nread % 8;

        string rawDataStr = "";
        rawDataStr.resize(nread);
        size_t rawOffset = 0;

        pthread_create(&progressthread, NULL, showProgressT, NULL);

        read.seekg(lr/8, ios_base::beg);  // move to reading begin pos
        const size_t BUFFER_SIZE = 8192;
        char buffer[BUFFER_SIZE];
        int remaining_loops = loops;

        while (remaining_loops > 0) {
            size_t to_read = (remaining_loops * 8 > BUFFER_SIZE * 8) ? BUFFER_SIZE : remaining_loops;
            read.read(buffer, to_read);
            size_t count = read.gcount();
            if (count == 0) break;

            for (size_t i = 0; i < count; ++i) {
                bicc = static_cast<unsigned char>(buffer[i]);
                GL_BIN_PROCESSED+=8;
                char2bin_inline(bicc, rawDataStr, rawOffset);
            }
            remaining_loops -= count;
        }

        //处理剩余字节
        if (loopsleft > 0) {
            string binBuf = "";
            read>>bicc;
            for(int i=loopsleft;i>0;i--){
                GL_BIN_PROCESSED++;
                if( (bicc & CMPARY[i-1])== CMPARY[i-1] ){
                    binBuf = "1" + binBuf;
                }else{
                    binBuf = "0" + binBuf;
                }
            }
            for (size_t i = 0; i < binBuf.length(); ++i) {
                rawDataStr[rawOffset++] = binBuf[i];
            }
        }

        binData = rawDataStr.substr(bitsskips, rawDataStr.length()-bitsskips);
    }
    read.close();
    finished = true;
    pthread_join(progressthread, NULL);
    return binData;
}

//该函数仅能够处理以8的倍数的数据。
const int writeBinary(const string filepath,const string & binData)
{
    ofstream write(filepath, ios_base::binary | ios_base::out);
    if( !write ){
         cout<<STRING_ERROR_OPEN<<endl;
         return -1;
    }
    cout<<"processing..."<<endl;
    pthread_t progressthread;
    pthread_create(&progressthread, NULL, showProgressT, NULL);
    GL_FILEBINLEN = binData.length();

    const size_t BUFFER_SIZE = 8192;
    char buffer[BUFFER_SIZE];
    size_t buf_index = 0;

    unsigned char bicc = 0x0;
    int svc = 0;
    for(size_t i=0;i<binData.length();i++){
        if(binData[i] == '1'){
            bicc |= CMPARY[svc];
        }
        svc++;
        if(svc == 8){
            svc = 0;
            buffer[buf_index++] = static_cast<char>(bicc);
            GL_BIN_PROCESSED+=8;
            bicc = 0x0;

            if (buf_index >= BUFFER_SIZE) {
                write.write(buffer, BUFFER_SIZE);
                buf_index = 0;
            }
        }
    }

    if (buf_index > 0) {
        write.write(buffer, buf_index);
    }

    write.close();
    finished = true;
    pthread_join(progressthread, NULL);
    return 0;
}

inline void char2bin_inline(const unsigned char ch, string& binData, size_t& offset)
{
    for(int i=0;i<CMPARYLENGTH;i++){
        if( (ch & CMPARY[i])== CMPARY[i] ){
            binData[offset++] = '1';
        }else{
            binData[offset++] = '0';
        }
    }
}

void* showProgressT(void* args)
{
    while(!finished)
    {
        double progress = 0.0;
        if (GL_FILEBINLEN > 0) {
            progress = GL_BIN_PROCESSED/double(GL_FILEBINLEN);
        }
        int barWidth = 70;
        string progressbar = "======================================================================";
        string spacebar___ = "                                                                      ";
        cout << "[";
        int pbpos = barWidth * progress;
        if (pbpos > barWidth) pbpos = barWidth;
        if (pbpos < 0) pbpos = 0;
        int sbpos = barWidth - pbpos;
        cout<<progressbar.substr(0,pbpos)<<spacebar___.substr(0,sbpos);
        cout.precision(3);
        cout << "] " << progress * 100.0<< fixed  << " %     \r";
    }
    cout<<"["<<"======================================================================"<< "] " << " 100%         \r";
    cout<<endl<<"done.\n";
    return NULL;
}

const string convert2Hex(const string & binData){
    int loops = binData.length()/4;
    int bitsleft = binData.length()%4;
    string result = "";
    result.resize(loops + (bitsleft > 0 ? 1 : 0));
    size_t res_index = 0;

    for(int i=0;i<loops;i++){
        string hexbit = binData.substr(i*4,4);
        for(int j=0;j<16;j++){
            if(HEXCODE[j] == hexbit){
                result[res_index++] = HEXARY[j];
                break;
            }
        }
    }

    if (bitsleft > 0) {
        string hexbit = binData.substr(loops*4,bitsleft);
        for(int i=0;hexbit.length()<4;i++){
            hexbit = "0" + hexbit;
        }
        for(int j=0;j<16;j++){
                if(HEXCODE[j] == hexbit){
                    result[res_index++] = HEXARY[j];
                    break;
                }
        }
    }
    return result;
}
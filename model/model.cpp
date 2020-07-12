#include"model.h"
using namespace  std;
Model::Model()
{
    //给数据成员分配空间
    _url=std::make_shared<std::string>();
    _dir=std::make_shared<std::string>();
    _htmltxt=std::make_shared<std::string>();

};

std::shared_ptr<std::string> Model::get_Url()
{
    return this->_url;
};
std::shared_ptr<std::string> Model::get_Dir()
{
    return this->_dir;
};
std::shared_ptr<std::string> Model::get_Htmltxt()
{
    return this->_htmltxt;
};
//下面实3个空函数，需要填充实际运行代码
//此处只是为了便于编译通过
bool Model::downLoad()
{

    QMessageBox::information(nullptr,"download","begin");//测试用
    //无法保证url和dir合法，先进行简单检查
    if(*_url==""||*_dir=="")
    {
        return false;
    };

    CURL *curl_handle;
    CURLcode res;
    
    curl_global_init(CURL_GLOBAL_ALL);
    
    curl_handle = curl_easy_init();

    curl_easy_setopt(curl_handle, CURLOPT_URL, (*_url).c_str());
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&mem);
    
    res = curl_easy_perform(curl_handle);
    
    if(res != CURLE_OK)
        cout << "curl_easy_perform() failed: " << curl_easy_strerror(res) << endl;
    else
        cout << (unsigned long)chunk.size <<" bytes retrieved\n";
    
    curl_easy_cleanup(curl_handle);
    
    curl_global_cleanup();
    //最好另开线程进行下载，否则下载时间太长时，整个程序会因等待下载而失去响应
    //不过第一轮迭代不要求这一点，之后再添加
    //此处不进行实际保存，存到变量_htmltxt中即可



    //最后弹出消息窗作为测试
    QMessageBox::information(nullptr,"download","succeed");
    return true;
};


static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
  size_t realsize = size * nmemb;
  MemoryStruct* mem = (MemoryStruct*)userp;
  delete[] mem->memory;
  mem->memory = new char[mem->size + realsize + 1];
  memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;
 
  return realsize;
}

bool Model::setUrl(const std::string& str)
{
    *_url=str;
    return true;
};
bool Model::setDir(const std::string& str)
{
    *_dir=str;
    return true;
};

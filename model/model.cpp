#include"model.h"
#include<QMessageBox>
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

    //do somthing about download
    //最好另开线程进行下载，否则下载时间太长时，整个程序会因等待下载而失去响应
    //不过第一轮迭代不要求这一点，之后再添加
    //此处不进行实际保存，存到变量_htmltxt中即可



    //最后弹出消息窗作为测试
    QMessageBox::information(nullptr,"download","succeed");
    return true;
};
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

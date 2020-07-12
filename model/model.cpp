#include"model.h"

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
    //do somthing
    return true;
};//
bool Model::setUrl(const std::string& str)
{
    //do somthing
    return true;
};
bool Model::setDir(const std::string& str)
{
    //do somthing
    return true;
};

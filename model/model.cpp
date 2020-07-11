#include"model.h"


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

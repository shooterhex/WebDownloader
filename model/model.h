#pragma once
#include"../common/cslbase.h"
#include<string>
class Model:public PropertyTrigger
{
public:
    std::shared_ptr<std::string> get_Url();
    std::shared_ptr<std::string> get_Dir();
    std::shared_ptr<std::string> get_Htmltxt();

private:
    std::shared_ptr<std::string> _htmltxt;
    std::shared_ptr<std::string> _dir;
    std::shared_ptr<std::string> _url;
};

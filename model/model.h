#pragma once
#include"../common/cslbase.h"
#include <QMessageBox>
#include <string>
#include <curl/curl.h>

class Model:public PropertyTrigger
{

public:
    Model();

    std::shared_ptr<std::string> get_Url();
    std::shared_ptr<std::string> get_Dir();
    std::shared_ptr<std::string> get_Htmltxt();

    //待实现,以下3个接口都将暴露给viewModel,由viewModel层触发
    bool downLoad();//
                    //读取_dir,_url,将下载的内容存入_htmltxt
                    //fire
                    //函数正常执行,返回true
    bool setUrl(const std::string& str);//设置_url
    bool setDir(const std::string& str);//设置_Dir


private:
    std::shared_ptr<std::string> _htmltxt;
    std::shared_ptr<std::string> _dir;
    std::shared_ptr<std::string> _url;
    MemoryStruct mem;
};


class MemoryStruct {
public:
  char* memory;
  size_t size;
  MemoryStruct() { memory = nullptr; size = 0; }
};
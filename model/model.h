#pragma once

#include"../common/cslbase.h"
#include"../common/def.h"
#include <QMessageBox>
#include <string>
#include <fstream>
#include <cstdlib>
#include <cstdio>

//Windows下编译工程时，需要在QMake中添加WINDOWS宏的定义
#ifdef WINDOWS
#include "../common/curl/curl.h"
#define CURL_STATICLIB
#pragma comment(lib, "Normaliz.lib")
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Wldap32.lib")
#pragma comment(lib, "Crypt32.lib")
#pragma comment(lib, "advapi32.lib")
#else
//Linux环境下，直接从软件包管理器安装curl静态库
#include <curl/curl.h>
#endif

typedef enum {
    title_sytax,
    aherf,
    title_eq,
    span,
    p_syntax,
    br_syntax,
    h1,
    h2,
    h3,
    h4,
    h5,
    h6
} txt_tag;

class tag_node {
public:
    int txt_start, txt_end;
    txt_tag tag_type;
};

struct greater
{
    bool operator()(const tag_node& a, const tag_node& b)
    {
        return a.txt_end > b.txt_end;
    }
};

class MemoryStruct {
public:
  std::string memory;
  size_t size;
  MemoryStruct() { memory = ""; size = 0; }
};

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
    bool setType(const int& type);//待实现
    static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp);
    std::string txt_proc(MemoryStruct& mem);
    bool image_proc(MemoryStruct& mem);
    bool image_download(std::string& img_url, std::string path);

private:
    std::shared_ptr<std::string> _htmltxt;
    std::shared_ptr<std::string> _dir;
    std::shared_ptr<std::string> _url;
    int type;
};



#pragma once
#ifndef DEF_H
#define DEF_H

#endif // DEF_H

//定义文件类型
#define TYPE_HTML 100
#define TYPE_TEXT 101
#define TYPE_PHOTO 102


//定义属性改变的消息
#define TASK_LIST_CHANGED 1000

#include<string>
struct WebTask
{
    int id;
    std::string url;
    std::string dir;
    int type;
};

#pragma once

//定义文件类型
#define TYPE_HTML 100
#define TYPE_TEXT 101
#define TYPE_PHOTO 102

//定义属性改变的消息
#define TASK_LIST_CHANGED 1000

#include<string>

inline std::string fileTypeIdToString(int type)
{
    switch (type) {
    case TYPE_HTML:
        return "HTML代码";
    case TYPE_TEXT:
        return "文本";
    case TYPE_PHOTO:
        return "图片";
    default:
        return "ERROR: invalid type ID";
    }
}

struct WebTask
{
    int id;
    std::string url;
    std::string dir;
    int type;
};

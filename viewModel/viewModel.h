#pragma once

#include"../common/cslbase.h"
#include"../common/def.h"
#include"../model/model.h"
#include<memory>
#include<QQueue>
class ViewModel:public PropertyTrigger
{
public:
    ViewModel(/* args */);
    ~ViewModel();
//以下属性对window而言暂时用不上
//properties
    std::shared_ptr<std::string> get_Htmltxt() noexcept;
    std::shared_ptr<std::string> get_Url() noexcept;
    std::shared_ptr<std::string> get_Dir() noexcept;

    //该属性将提供给window
    //考虑将任务列表属性放在viewModel层维护
    //目前先在构造函数中对tasklist赋值测试
    std::shared_ptr<QQueue<WebTask>> get_TaskList() noexcept;

//commands
    CommandFunc get_SetUrlCommand();
    CommandFunc get_SetDirCommand();
    CommandFunc get_SetTypeCommand();
    CommandFunc get_DownloadCommand();
//methods
    void SetModel(const std::shared_ptr<Model>& spModel);

    PropertyNotification get_notification();
private:
    //任务列表
    std::shared_ptr<QQueue<WebTask>> _taskList;
    std::shared_ptr<Model>  m_spModel;
    std::thread downloading_task;
    std::promise<char> download_result;
    bool is_task_finished;
};

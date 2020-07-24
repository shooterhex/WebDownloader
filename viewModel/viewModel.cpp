#include"viewModel.h"

ViewModel::ViewModel()
{
    _taskList=std::make_shared<QQueue<WebTask>>();
    is_task_finished = false;
};
ViewModel::~ViewModel(){};
void ViewModel::SetModel(const std::shared_ptr<Model>& spModel)
{
    m_spModel = spModel;
    m_spModel->AddNotification(get_notification());
};
//properties
std::shared_ptr<std::string> ViewModel::get_Htmltxt() noexcept
{
    return m_spModel->get_Htmltxt();
};
std::shared_ptr<std::string> ViewModel::get_Dir() noexcept
{
    return m_spModel->get_Dir();
};
std::shared_ptr<std::string> ViewModel::get_Url() noexcept
{
    return m_spModel->get_Url();
};
std::shared_ptr<QQueue<WebTask>> ViewModel::get_TaskList() noexcept
{
    return _taskList;
};




PropertyNotification ViewModel::get_notification()
{
    return [this](uint32_t uID)
            {
                // 异步触发可尝试信号槽机制实现
                // this->Fire(uID); //result输出到mainwindow 再由mainwindow把uid改回TASK_LIST_CHANGED
                //任务完成时，删除第一项任务
                _taskList->pop_front();

                //检查是否有待下载任务
                if(!_taskList->empty())
                {
                    auto t=_taskList->front();
                    this->m_spModel->setDir(t.dir);
                    this->m_spModel->setUrl(t.url);
                    this->m_spModel->setType(t.type);
                    this->m_spModel->downLoad();
                }
                else
                    is_task_finished = true;
            };
};

CommandFunc ViewModel::get_DownloadCommand()
{
    static int cnt;
    return [this](std::any&& param)->bool
    {
        WebTask t=std::any_cast<WebTask>(param);
        t.id=cnt++;
        _taskList->push_back(t);

        Fire(TASK_LIST_CHANGED);

        qDebug() << "is_task_finished = " << is_task_finished;

        if(is_task_finished)
        {
            downloading_task.join(); //回收线程
            is_task_finished = false; //重新初始化
        }
        else if(!downloading_task.joinable()) //joinable时表示下载线程正在运行 因此什么都不用做
        {
            this->m_spModel->setDir(t.dir);
            this->m_spModel->setUrl(t.url);
            this->m_spModel->setType(t.type);
            downloading_task = std::thread(&Model::downLoad, m_spModel);
            Fire(TASK_BEGIN); //弹窗提示开始下载
        }
        return true; //NULL statement
    };
};
CommandFunc ViewModel::get_SetDirCommand()
{
    return [this](std::any&& param)->bool
    {
        return this->m_spModel->setDir(std::any_cast<std::string>(param));
    };
};
CommandFunc ViewModel::get_SetUrlCommand()
{
    return [this](std::any&& param)->bool
    {
        return this->m_spModel->setUrl(std::any_cast<std::string>(param));
    };
};
CommandFunc ViewModel::get_SetTypeCommand()
{
    return [this](std::any&& param)->bool
    {
        return this->m_spModel->setType(std::any_cast<int>(param));
    };
};

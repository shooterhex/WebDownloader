#include"App.h"
void App::run()
{
    //消息传递的绑定
    _viewModel.AddNotification(_mainWindow.get_Notification());

    //此处model不是成员变量，（按照老师的wm1案例
    std::shared_ptr<Model> spModel(std::make_shared<Model>());
    //viewModel和Model的绑定，同时完成了spModel绑定VM的消息
    _viewModel.SetModel(spModel);

    _mainWindow.set_DownloadCommand(_viewModel.get_DownloadCommand());
    _mainWindow.set_GetTaskListCommand(_viewModel.get_GetTaskListFunc());
    _mainWindow.show();
}

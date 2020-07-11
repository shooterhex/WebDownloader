#include"App.h"
void App::run()
{
    _viewModel.AddNotification(_mainWindow.get_Notification());
    _model.AddNotification(_viewModel.get_notification());
    _mainWindow.show();
}

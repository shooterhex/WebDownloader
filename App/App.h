#include "../window/mainWindow.h"
#include "model/model.h"
#include "viewModel/viewModel.h"
class App
{
private:
    MainWindow _mainWindow;
    Model _model;
    ViewModel _viewModel;
public:
    void run();
};

#include"mainWindow.h"

PropertyNotification MainWindow::get_Notification()
{
    return [this](uint32_t uID)
                {
                    if (uID == HTMLTEXT) {
                        this->update();
                    }
                };
}
MainWindow::MainWindow(QWidget *parent):QMainWindow(parent)
{

}

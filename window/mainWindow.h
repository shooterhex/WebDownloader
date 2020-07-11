#include<QMainWindow>
#include"../common/cslbase.h"
class MainWindow:public QMainWindow
{

    public:
        explicit MainWindow(QWidget *parent = nullptr);
        PropertyNotification get_Notification();
        //void set_ptrM1Command(std::shared_ptr<ICommandBase> ptrCommand);
        //void set_ptrM2Command(std::shared_ptr<ICommandBase> ptrCommand);
        //void set_ptrAboutCommand(std::shared_ptr<ICommandBase> ptrCommand);
        //~FirstPage();
        void set_DownloadCommand(CommandFunc&& cf);
    private:
        //Ui::FirstPage *ui;
        //std::shared_ptr<ICommandBase> _ptrM1Command;
        //std::shared_ptr<ICommandBase> _ptrM2Command;
        //std::shared_ptr<ICommandBase> _ptrAboutCommand;

        CommandFunc m_cmdFunc_Download;
};

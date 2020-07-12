#include<QMainWindow>
#include"../common/cslbase.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow:public QMainWindow
{

    Q_OBJECT
    public:
        explicit MainWindow(QWidget *parent = nullptr);
        ~MainWindow();

        PropertyNotification get_Notification();
        //void set_downloadCommand(std::shared_ptr<CommandFunc> ptrCommand);
        //void set_ptrM2Command(std::shared_ptr<ICommandBase> ptrCommand);
        //void set_ptrAboutCommand(std::shared_ptr<ICommandBase> ptrCommand);

        //待实现，要求点击点击下载按钮，执行m_cmdFunc_Download()
        void set_DownloadCommand(CommandFunc&& cf);
private slots:
        void OnBtnDownload();
    private:
        Ui::MainWindow *ui;
        std::shared_ptr<CommandFunc> _downloadCommand;
        //std::shared_ptr<ICommandBase> _ptrM2Command;
        //std::shared_ptr<ICommandBase> _ptrAboutCommand;

        CommandFunc m_cmdFunc_Download;
};

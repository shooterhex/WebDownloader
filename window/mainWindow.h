#include<QMainWindow>
#include<viewModel/viewModel.h>
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

        void set_DownloadCommand(CommandFunc&& cf);
        void setViewModel(ViewModel* viewModel);
private slots:
        void OnBtnDownload();
        void OnBtnChooseDir();

        void OnNewTaskButtonPressed();
        void OnTaskListButtonPressed();

    private:
        Ui::MainWindow *ui;
        std::shared_ptr<CommandFunc> _downloadCommand;
        //std::shared_ptr<ICommandBase> _ptrM2Command;
        //std::shared_ptr<ICommandBase> _ptrAboutCommand;

        CommandFunc m_cmdFunc_Download;
        ViewModel* m_viewModel;
};

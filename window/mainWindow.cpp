#include"mainWindow.h"
#include"ui_mainWindow.h"

#include<QMessageBox>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //setWindowTitle("服装绘图");
    connect(ui->pushButton_2,SIGNAL(clicked()),this,SLOT(OnBtnDownload()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

PropertyNotification MainWindow::get_Notification()
{
    return [this](uint32_t uID)
                {
                    if (uID == HTMLTEXT) {
                        this->update();
                    }
                };
}
void MainWindow::set_DownloadCommand(CommandFunc&& cf)
{
    m_cmdFunc_Download = std::move(cf);
};
void MainWindow::OnBtnDownload()
{
    //QMessageBox::information(nullptr,"nn","ss");
    m_cmdFunc_Download(std::any());//此处语法要求必须传参
}

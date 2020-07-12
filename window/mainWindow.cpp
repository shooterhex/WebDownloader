#include"mainWindow.h"
#include"ui_mainWindow.h"
#include<QFileDialog>
#include<QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->startPushButton, SIGNAL(clicked()), this, SLOT(OnBtnDownload()));
    connect(ui->chooseDirPushButton, SIGNAL(clicked()), this, SLOT(OnBtnChooseDir()));
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

void MainWindow::setViewModel(ViewModel* viewModel)
{
    m_viewModel = viewModel;
}

void MainWindow::OnBtnDownload()
{
    set_DownloadCommand(m_viewModel->get_DownloadCommand());
    std::string dir = ui->dirTextEdit->toPlainText().toStdString();
    m_viewModel->get_SetDirCommand()(dir);
    std::string url = ui->urlTextEdit->toPlainText().toStdString();
    m_viewModel->get_SetUrlCommand()(url);

    m_cmdFunc_Download(std::any()); //Dummy argument
}

void MainWindow::OnBtnChooseDir()
{
    QString dirName = QFileDialog::getSaveFileName(this, "Select destination directory");
    ui->dirTextEdit->setText(dirName);
}

#include"mainWindow.h"
#include"ui_mainWindow.h"
#include<QFileDialog>
#include<QMessageBox>
#include"../common/def.h"

void testGitPush() {
    //...
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    OnNewTaskButtonPressed();

    connect(ui->startPushButton, SIGNAL(clicked()), this, SLOT(OnBtnDownload()));
    connect(ui->chooseDirPushButton, SIGNAL(clicked()), this, SLOT(OnBtnChooseDir()));

    connect(ui->newTaskPushButton, SIGNAL(clicked()), this, SLOT(OnNewTaskButtonPressed()));
    connect(ui->taskListPushButton, SIGNAL(clicked()), this, SLOT(OnTaskListButtonPressed()));
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

    int typeID = -1;
    switch (ui->fileTypeComboBox->currentIndex()) {
    case 0: //HTML
        typeID = TYPE_HTML;
        break;
    case 1: //Text
        typeID = TYPE_TEXT;
        break;
    case 2: //Image
        typeID = TYPE_PHOTO;
        break;
    }
    m_viewModel->get_SetTypeCommand()(typeID);

    auto res = m_cmdFunc_Download(std::any()); //Dummy argument
    if (res) {
        QMessageBox::information(this, "Success", "Succeeded to download.");
    }
    else {
        QMessageBox::warning(this, "Error", "Failed to download!");
    }
}

void MainWindow::OnBtnChooseDir()
{
    QString dirName = QFileDialog::getSaveFileName(this, "Select destination directory");
    ui->dirTextEdit->setText(dirName);
}

void MainWindow::OnNewTaskButtonPressed()
{
    ui->newTaskPushButton->setFlat(true);
    ui->taskListPushButton->setFlat(false);
    ui->newTaskPageWidget->show();
    ui->taskListPageWidget->hide();
}

void MainWindow::OnTaskListButtonPressed()
{
    ui->taskListPushButton->setFlat(true);
    ui->newTaskPushButton->setFlat(false);
    ui->taskListPageWidget->show();
    ui->newTaskPageWidget->hide();
}

#include "mainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Load "new task" page by default
    onNewTaskActionTriggered();

    //Connect buttons with corresponding slots
    connect(ui->startPushButton, SIGNAL(clicked()), this, SLOT(onDownloadButtonPressed()));
    connect(ui->chooseFilePushButton, SIGNAL(clicked()), this, SLOT(onChooseFileButtonPressed()));
    connect(ui->chooseDirPushButton, SIGNAL(clicked()), this, SLOT(onChooseDirButtonPressed()));
    connect(ui->refreshTaskListPushButton, SIGNAL(clicked()), this, SLOT(onRefreshTaskListButtonPressed()));

    //Connect menubar actions with corresponding slots
    connect(ui->newTaskAction, SIGNAL(triggered()), this, SLOT(onNewTaskActionTriggered()));
    connect(ui->quitAction, SIGNAL(triggered()), this, SLOT(onQuitActionTriggered()));
    connect(ui->taskListAction, SIGNAL(triggered()), this, SLOT(onTaskListActionTriggered()));
    connect(ui->aboutAction, SIGNAL(triggered()), this, SLOT(onAboutActionTriggered()));

    //Attack QLabel object to status bar
    m_statusBarLabel = new QLabel();
    ui->statusBar->addWidget(m_statusBarLabel);

    //Initialize status bar with nTasks = 0
    m_nTasks = 0;
    updateStatusBar();
}

MainWindow::~MainWindow()
{
    delete ui;
}

PropertyNotification MainWindow::get_Notification()
{
    return [this](uint32_t uID)
                {
                    switch (uID)
                    {
                    case TASK_SINGLE_SUCEEDED:
                        QMessageBox::information(this, "Success", "Succeeded to download.");
                        break;
                    
                    case TASK_SINGLE_FAILED:
                        QMessageBox::warning(this, "Error", "Failed to download!");
                        break;
                    
                    case TASK_BEGIN:
                        QMessageBox::information(this, "Tasks", "Download start.");
                        break;
                    
                    default: break;
                    }
                    this->update();
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

void MainWindow::onDownloadButtonPressed()
{
    set_DownloadCommand(m_viewModel->get_DownloadCommand());

    std::string dir = ui->dirTextEdit->toPlainText().toStdString();
//    m_viewModel->get_SetDirCommand()(dir);
    std::string url = ui->urlTextEdit->toPlainText().toStdString();
//    m_viewModel->get_SetUrlCommand()(url);

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
    //m_viewModel->get_SetTypeCommand()(typeID);

    bool res = m_cmdFunc_Download(std::any(WebTask{0,url,dir,typeID}));
    if (res) {
        m_nTasks += 1;
        updateStatusBar();
        qDebug() << "succeed OnBtnDownload\n";
    }
    else {
        qDebug() << "failed OnBtnDownload\n";
    }
}

void MainWindow::onChooseFileButtonPressed()
{
    QString fileName = QFileDialog::getSaveFileName(this, QStringLiteral("选择写入文件的路径"));
    ui->dirTextEdit->setText(fileName);
}

void MainWindow::onChooseDirButtonPressed()
{
    QString dirName = QFileDialog::getExistingDirectory(this, QStringLiteral("选择写入目录的路径"));
    ui->dirTextEdit->setText(dirName);
}

void MainWindow::onNewTaskActionTriggered()
{
    ui->newTaskPageWidget->show();
    ui->taskListPageWidget->hide();
}

void MainWindow::onQuitActionTriggered()
{
    exit(0);
}

void MainWindow::onTaskListActionTriggered()
{
    ui->taskListPageWidget->show();
    ui->newTaskPageWidget->hide();

    updateTaskList();
}

void MainWindow::onAboutActionTriggered()
{
    static QString aboutInfo = tr("WebDownload by C++ project group");
    QMessageBox::information(this, tr("关于"), aboutInfo);
}

void MainWindow::onRefreshTaskListButtonPressed()
{
    updateTaskList();
}

void MainWindow::updateTaskList()
{
    auto queue = m_viewModel->get_TaskList();
    if (! queue) {
        QMessageBox::critical(this, QStringLiteral("错误"), QStringLiteral("加载任务队列时失败！"));
        return;
    }
    m_nTasks = queue->size();

    ui->taskListLabel->setText(QStringLiteral("任务列表：%1项进行中").arg(queue->size()));

    auto* table = ui->taskListTableWidget;
    table->clearContents();
    table->setRowCount(0);

    for (const WebTask& task: *queue) {
        int k = table->rowCount();
        table->insertRow(k);

        table->setItem(k, 0, new QTableWidgetItem(QString::number(task.id)));
        table->setItem(k, 1, new QTableWidgetItem(QString::fromStdString(task.url)));
        table->setItem(k, 2, new QTableWidgetItem(QString::fromStdString(task.dir)));
        table->setItem(k, 3, new QTableWidgetItem(QString::fromStdString(fileTypeIdToString(task.type))));
    }

    updateStatusBar();
}

void MainWindow::updateStatusBar()
{
    m_statusBarLabel->setText(QStringLiteral("总任务数：%1").arg(m_nTasks));
}

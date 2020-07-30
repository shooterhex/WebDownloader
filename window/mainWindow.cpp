#include"mainWindow.h"
#include"ui_mainWindow.h"
#include<QFileDialog>
#include<QMessageBox>
#include"../common/def.h"
#include"batchparser.h"
#include<QDebug>
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
                        //QMessageBox::information(this, "Success", "Succeeded to download.");
                        break;
                    
                    case TASK_SINGLE_FAILED:
                        //QMessageBox::warning(this, "Error", "Failed to download!");
                        break;
                    
                    case TASK_BEGIN:
                        //QMessageBox::information(this, "Tasks", "Download start.");
                        break;
                    
                    default: break;
                    }
                    //this->update();
                };
}

void MainWindow::set_DownloadCommand(CommandFunc&& cf)
{
    m_cmdFunc_Download = std::move(cf);
};

void MainWindow::set_GetTaskListCommand(GetTaskListFunc &&cf)
{
    m_cmdFunc_GetTaskList = std::move(cf);
}

void MainWindow::processDownload(const QString& url, const QString& dir)
{
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

    bool res = m_cmdFunc_Download(std::any(WebTask{0,url.toStdString(),dir.toStdString(),typeID}));
    if (res) {
        m_nTasks += 1;
        qDebug() << "succeed OnBtnDownload\n";
    }
    else {
        qDebug() << "failed OnBtnDownload\n";
    }
}

void MainWindow::onDownloadButtonPressed()
{
    auto parseRes = parseBatch(ui->urlTextEdit->toPlainText(), ui->dirTextEdit->toPlainText(), ui->argsTextEdit->toPlainText());
    if (parseRes.index() == 0) {
        QMessageBox::critical(this, QStringLiteral("错误"), std::get<0>(parseRes));
        return;
    }
    const auto& tasks = std::get<1>(parseRes);
    for (const auto& t: tasks) {
        processDownload(t.url, t.dir);
    }
}

void MainWindow::onChooseFileButtonPressed()
{
    QString fileName = QFileDialog::getSaveFileName(this,QStringLiteral("选择写入文件的路径"));
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
    static QString aboutInfo = QStringLiteral("WebDownload by C++ project group");
    QMessageBox::information(this, tr("About"), aboutInfo);
}

void MainWindow::onRefreshTaskListButtonPressed()
{
    updateTaskList();
}

void MainWindow::updateTaskList()
{
    auto queue = m_cmdFunc_GetTaskList();
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

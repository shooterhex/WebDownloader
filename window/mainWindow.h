﻿#pragma once

#include<QMainWindow>
#include<QLabel>
#include<any>
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
        void onDownloadButtonPressed();
        void onChooseFileButtonPressed();
        void onChooseDirButtonPressed();
        void onRefreshTaskListButtonPressed();

        void onNewTaskActionTriggered();
        void onQuitActionTriggered();
        void onTaskListActionTriggered();
        void onAboutActionTriggered();

    private:
        Ui::MainWindow *ui;
        std::shared_ptr<CommandFunc> m_downloadCommand;
        //std::shared_ptr<ICommandBase> _ptrM2Command;
        //std::shared_ptr<ICommandBase> _ptrAboutCommand;

        //Attached to status bar
        QLabel* m_statusBarLabel;

        CommandFunc m_cmdFunc_Download;
        ViewModel* m_viewModel;

        //Number of tasks in the task list
        int m_nTasks;

        void updateTaskList();
        void updateStatusBar();
};

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "constants.h"
#include "environmentvariablesdialog.h"
#include "configuration.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    redIcon(":/images/red-icon"),
    greenIcon(":/images/green-icon"),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->actionNew, SIGNAL(triggered(bool)), this, SLOT(newTab()));
    connect(ui->actionGlobal_environment_variables, SIGNAL(triggered(bool)), this, SLOT(editGlobalEnv()));

    Configuration &configuration = Configuration::getInstance();

    QStringList groupNames;

    configuration.getConsoleNames(groupNames);

    foreach (const QString &groupName, groupNames)
    {
        newTab(groupName);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::newTab()
{
    ConsoleWidget *consoleWidget = new ConsoleWidget(this);

    prepareTab(consoleWidget, "new-tab");
}

void MainWindow::newTab(const QString &tabName)
{
    ConsoleWidget *consoleWidget = new ConsoleWidget(tabName, this);

    prepareTab(consoleWidget, tabName);
}

void MainWindow::prepareTab(ConsoleWidget *consoleWidget, const QString &tabName)
{
    int tabIndex = ui->tabWidget->addTab(consoleWidget, redIcon, tabName);

    consoleWidget->setProperty(TAB_INDEX, tabIndex);

    connect(consoleWidget, SIGNAL(titleUpdated(const QString&, int)), this, SLOT(updateTabText(const QString &, int)));
    connect(consoleWidget, SIGNAL(started(int)), this, SLOT(processStarted(int)));
    connect(consoleWidget, SIGNAL(stopped(int)), this, SLOT(processStopped(int)));
}

void MainWindow::updateTabText(const QString &newText, int tabIndex)
{
    ui->tabWidget->setTabText(tabIndex, newText);
}

void MainWindow::editGlobalEnv()
{
    EnvironmentVariablesDialog dialog(this);

    dialog.setModal(true);
    dialog.exec();
}

void MainWindow::processStarted(int tabIndex)
{
    ui->tabWidget->setTabIcon(tabIndex, greenIcon);
}

void MainWindow::processStopped(int tabIndex)
{
ui->tabWidget->setTabIcon(tabIndex, redIcon);
}

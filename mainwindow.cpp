#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "constants.h"
#include <QSettings>

#define TAB_INDEX "TAB_INDEX"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->actionNew, SIGNAL(triggered(bool)), this, SLOT(newTab()));

    QSettings settings(COMPANY_NAME, APP_NAME);

    QStringList groupNames = settings.childGroups();

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
    int tabIndex = ui->tabWidget->addTab(consoleWidget, tabName);

    consoleWidget->setProperty(TAB_INDEX, tabIndex);

    connect(consoleWidget, SIGNAL(titleUpdated(const QString&)), this, SLOT(updateTabText(const QString &)));
}

void MainWindow::updateTabText(const QString &newText)
{
    ConsoleWidget *consoleWidget= dynamic_cast<ConsoleWidget *>(sender());

    int tabIndex = consoleWidget->property(TAB_INDEX).toInt();

    ui->tabWidget->setTabText(tabIndex, newText);
}

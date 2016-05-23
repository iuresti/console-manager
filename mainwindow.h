#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QIcon>

#include "consolewidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void newTab();
    void updateTabText(const QString &newText, int tabIndex);
    void editGlobalEnv();
    void processStarted(int tabIndex);
    void processStopped(int tabIndex);

private:
    void newTab(const QString &tabName);
    void prepareTab(ConsoleWidget *consoleWidget, const QString &tabName);
private:
    Ui::MainWindow *ui;

    QIcon redIcon;
    QIcon greenIcon;
};

#endif // MAINWINDOW_H

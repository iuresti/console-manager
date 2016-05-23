#ifndef CONSOLEWIDGET_H
#define CONSOLEWIDGET_H

#include <QDialog>
#include <QProcess>

namespace Ui {
class ConsoleWidget;
}

class ConsoleWidget : public QDialog
{
    Q_OBJECT

public:
    explicit ConsoleWidget(QWidget *parent = 0);
    explicit ConsoleWidget(const QString &tabName, QWidget *parent = 0);

    ~ConsoleWidget();

signals:
    void titleUpdated(const QString &newTitle, int tabIndex);
    void started(int tabIndex);
    void stopped(int tabIndex);

private slots:
    void titleTextEditChanged(const QString &newTitle);
    void toggleCommand();
    void printProcessOutput();
    void processStarted();
    void processFinished(int,QProcess::ExitStatus);
    void openEnvVarDialog();

private:
    void executeCommand();
    void prepareEnv(QStringList &env, const QString &rootTitle);

private:
    Ui::ConsoleWidget *ui;

    QProcess process;
};

#endif // CONSOLEWIDGET_H

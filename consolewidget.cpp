#include "consolewidget.h"
#include "ui_consolewidget.h"
#include "constants.h"
#include "environmentvariablesdialog.h"

#include <QSettings>

ConsoleWidget::ConsoleWidget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConsoleWidget)
{
    ui->setupUi(this);

    connect(ui->title, SIGNAL(textChanged(QString)), this, SLOT(titleTextEditChanged(const QString &)));

    connect(&process, SIGNAL(readyRead()), this, SLOT(printProcessOutput()));
    connect(&process, SIGNAL(started()), this, SLOT(processStarted()));
    connect(&process, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(processFinished(int,QProcess::ExitStatus)));
}

ConsoleWidget::ConsoleWidget(const QString &tabName, QWidget *parent) :
    ConsoleWidget::ConsoleWidget(parent)
{
    QSettings settings(COMPANY_NAME, APP_NAME);

    settings.beginGroup(tabName);
    ui->title->setText(tabName);
    ui->command->setText(settings.value(PARAM_COMMAND).toString());
    ui->workingDirectory->setText(settings.value(PARAM_WORKING_DIR).toString());
    settings.endGroup();
}

ConsoleWidget::~ConsoleWidget()
{
    delete ui;
}

void ConsoleWidget::titleTextEditChanged(const QString &newTitle)
{
    emit titleUpdated(newTitle);
}

void ConsoleWidget::toggleCommand()
{
    if(process.state() == QProcess::NotRunning)
    {
        executeCommand();
    }
    else
    {
        process.close();
        process.waitForFinished(10000);
    }
}

void ConsoleWidget::printProcessOutput(){
    while(process.bytesAvailable() > 0){
        QByteArray bLine = process.readLine();
        QString line(bLine);
        ui->output->appendHtml(line);
    }
}

void ConsoleWidget::executeCommand()
{
    QSettings settings(COMPANY_NAME, APP_NAME);
    QStringList env;

    ui->output->clear();
    ui->commandBtn->setText("Starting...");

    settings.beginGroup(ui->title->text());
    settings.setValue(PARAM_COMMAND, ui->command->text());
    settings.setValue(PARAM_WORKING_DIR, ui->workingDirectory->text());
    settings.endGroup();

    prepareEnv(env);

    process.setWorkingDirectory(ui->workingDirectory->text());
    process.setEnvironment(env);
    process.start(ui->command->text());

    process.waitForStarted(10000);
}

void ConsoleWidget::processStarted()
{
    int id = process.processId();
    ui->processIdLabel->setText(QString::number(id));

    ui->commandBtn->setText("Stop");
}

void ConsoleWidget::processFinished(int,QProcess::ExitStatus)
{
    ui->processIdLabel->setText("");
    ui->commandBtn->setText("Execute");
}

void ConsoleWidget::openEnvVarDialog()
{
    EnvironmentVariablesDialog dialog(ui->title->text(), this);

    dialog.setModal(true);
    dialog.exec();
}

void ConsoleWidget::prepareEnv(QStringList &env)
{
    QSettings settings(COMPANY_NAME, APP_NAME);

    settings.beginGroup(ui->title->text() + "/env");
    QStringList keys = settings.childKeys();

    foreach(QString key, keys)
    {
        QString line;

        line.append(key).append("=").append(settings.value(key).toString());
        env << line;
    }

    settings.endGroup();
}

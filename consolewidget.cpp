#include "consolewidget.h"
#include "ui_consolewidget.h"
#include "constants.h"
#include "configuration.h"
#include "environmentvariablesdialog.h"

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
    Configuration &configuration = Configuration::getInstance();
    QString command;
    QString workingDirectory;

    configuration.load(tabName, command, workingDirectory);

    ui->title->setText(tabName);
    ui->command->setText(command);
    ui->workingDirectory->setText(workingDirectory);


}

ConsoleWidget::~ConsoleWidget()
{
    delete ui;
}

void ConsoleWidget::titleTextEditChanged(const QString &newTitle)
{
    emit titleUpdated(newTitle, property(TAB_INDEX).toInt());
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
        QByteArray bLine = process.readLine().trimmed();
        QString line(bLine);
        if(!line.isEmpty()){
            ui->output->appendPlainText(line);
        }
    }
}

void ConsoleWidget::executeCommand()
{
    Configuration &configuration = Configuration::getInstance();
    QStringList env;

    ui->output->clear();
    ui->commandBtn->setText("Starting...");

    configuration.save(ui->title->text(), ui->command->text(), ui->workingDirectory->text());

    configuration.getEnvironment(env, ui->title->text());

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

    emit started(property(TAB_INDEX).toInt());
}

void ConsoleWidget::processFinished(int,QProcess::ExitStatus)
{
    ui->processIdLabel->setText("");
    ui->commandBtn->setText("Execute");

    emit stopped(property(TAB_INDEX).toInt());
}

void ConsoleWidget::openEnvVarDialog()
{
    EnvironmentVariablesDialog dialog(ui->title->text(), this);

    dialog.setModal(true);
    dialog.exec();
}

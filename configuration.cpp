#include "configuration.h"
#include "constants.h"

#include <QSettings>
#include <QDir>
#include <QUuid>
#include <QCryptographicHash>
#include <QTextStream>

Configuration Configuration::configuration;

Configuration::Configuration()
{
    QString currentPath = QDir::currentPath();
    QString tmpDirPath = currentPath += "\\execution\\tmp";
    QDir tmpDir(tmpDirPath);
    QFile idFile("console-manager.id");
    QUuid uuid = QUuid::createUuid();
    int uuidLen = uuid.toString().length() - BRACES_LEN;

    if(idFile.open(QIODevice::ReadOnly)){
        QTextStream in(&idFile);
        currentInstanceSettings = in.readLine();
        idFile.close();
    }

    if(currentInstanceSettings.length() != uuidLen)
    {
        QUuid uuid = QUuid::createUuid();
        currentInstanceSettings = uuid.toString();
        currentInstanceSettings = currentInstanceSettings.mid(1, currentInstanceSettings.length()-2);
        if(idFile.open(QIODevice::Truncate | QIODevice::WriteOnly)){
            QTextStream out(&idFile);
            out << currentInstanceSettings;
            idFile.close();
        }
    }

    currentInstanceSettings += "/";

    QSettings settings(COMPANY_NAME, APP_NAME);

    settings.beginGroup(currentInstanceSettings + GLOBAL + "/env");
    settings.setValue("TEMP", tmpDirPath);
    settings.setValue("TMP", tmpDirPath);
    settings.endGroup();

    tmpDir.mkpath(tmpDirPath);
}

Configuration &Configuration::getInstance()
{
    return configuration;
}

void Configuration::load(const QString &tabName, QString &command, QString &workingDirectory)
{
    QSettings settings(COMPANY_NAME, APP_NAME);

    settings.beginGroup(currentInstanceSettings + tabName);
    command = settings.value(PARAM_COMMAND).toString();
    workingDirectory = settings.value(PARAM_WORKING_DIR).toString();
    settings.endGroup();
}

void Configuration::save(const QString &tabName, const QString &command, const QString &workingDirectory)
{
    QSettings settings(COMPANY_NAME, APP_NAME);

    settings.beginGroup(currentInstanceSettings + tabName);
    settings.setValue(PARAM_COMMAND, command);
    settings.setValue(PARAM_WORKING_DIR, workingDirectory);
    settings.endGroup();
}

void Configuration::getEnvironment(QStringList &env, const QString &tabName)
{
    QMap<QString,QString> vars;

    prepareEnv(vars, GLOBAL);
    prepareEnv(vars, tabName);

    QMap<QString, QString>::iterator i;
    for (i = vars.begin(); i != vars.end(); ++i)
    {
        QString line;

        line.append(i.key()).append("=").append(i.value());
        env << line;
    }
}

void Configuration::prepareEnv(QMap<QString,QString> &vars, const QString &rootTitle)
{
    QSettings settings(COMPANY_NAME, APP_NAME);

    settings.beginGroup(currentInstanceSettings + rootTitle + "/env");
    QStringList keys = settings.childKeys();

    foreach(QString key, keys)
    {
        vars[key] = settings.value(key).toString();
    }

    settings.endGroup();
}

void Configuration::updateEnv(const QString &tabName, const QMap<QString, QString> &vars)
{
    QSettings settings(COMPANY_NAME, APP_NAME);
    QMap<QString, QString>::const_iterator i;

    settings.beginGroup(currentInstanceSettings + tabName + "/env");
    settings.remove("");

    for (i = vars.begin(); i != vars.end(); ++i)
    {
        settings.setValue(i.key(), i.value());
    }

    settings.endGroup();
}

void Configuration::getEnv(const QString &tabName, QMap<QString, QString> &env)
{
    QSettings settings(COMPANY_NAME, APP_NAME);

    settings.beginGroup(currentInstanceSettings + tabName + "/env");
    QStringList keys = settings.childKeys();

    foreach(QString key, keys){
        env[key] =settings.value(key).toString();
    }
}

void Configuration::getConsoleNames(QStringList &consoleNames)
{
    QSettings settings(COMPANY_NAME, APP_NAME);

    settings.beginGroup(currentInstanceSettings);
    QStringList all = settings.childGroups();

    foreach (const QString &consoleName, all)
    {
        if(consoleName == GLOBAL)
            continue;

        consoleNames << consoleName;
    }
    settings.endGroup();


}

#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <QStringList>
#include <QMap>

class Configuration
{
private:
    Configuration();

public:

    static Configuration &getInstance();

    void load(const QString &tabName, QString &command, QString &workingDirectory);
    void save(const QString &tabName, const QString &command, const QString &workingDirectory);

    void getEnvironment(QStringList &env, const QString &tabName);

    void getEnv(const QString &tabName, QMap<QString, QString> &env);
    void updateEnv(const QString &tabName, const QMap<QString, QString> &newVars);

    void getConsoleNames(QStringList &consoleNames);

private:
    void prepareEnv(QMap<QString,QString> &vars, const QString &rootTitle);
    QString currentInstanceSettings;

    static Configuration configuration;
};

#endif // CONFIGURATION_H

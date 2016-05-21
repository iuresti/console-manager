#ifndef ENVIRONMENTVARIABLESDIALOG_H
#define ENVIRONMENTVARIABLESDIALOG_H

#include <QDialog>

#include <QStandardItemModel>

namespace Ui {
class EnvironmentVariablesDialog;
}

class EnvironmentVariablesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EnvironmentVariablesDialog(const QString &titleConsole, QWidget *parent = 0);
    ~EnvironmentVariablesDialog();

public slots:
    virtual void accept();
    virtual void reject();

    void addVar();
    void deleteVar();

private:
    void loadSettings();

    Ui::EnvironmentVariablesDialog *ui;

    QStandardItemModel *itemModel;
    QString titleConsole;
};

#endif // ENVIRONMENTVARIABLESDIALOG_H

#include "environmentvariablesdialog.h"
#include "ui_environmentvariablesdialog.h"

#include <QMap>
#include "constants.h"

#include "configuration.h"

EnvironmentVariablesDialog::EnvironmentVariablesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EnvironmentVariablesDialog)
{
    ui->setupUi(this);

    QStringList labels;

    labels << "Name" << "Value";

    itemModel = new QStandardItemModel(3, 2);
    itemModel->setHorizontalHeaderLabels(labels);
    ui->envVars->horizontalHeader()->setStretchLastSection(true);

    this->setWindowTitle(tr("Environment variables for %1").arg(titleConsole));
    this->titleConsole = GLOBAL;

    loadSettings();

    ui->envVars->setModel(itemModel);
}

EnvironmentVariablesDialog::EnvironmentVariablesDialog(const QString &titleConsole, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EnvironmentVariablesDialog)
{
    ui->setupUi(this);

    QStringList labels;

    labels << "Name" << "Value";

    itemModel = new QStandardItemModel(3, 2);
    itemModel->setHorizontalHeaderLabels(labels);
    ui->envVars->horizontalHeader()->setStretchLastSection(true);

    this->setWindowTitle(tr("Environment variables for %1").arg(titleConsole));
    this->titleConsole = titleConsole;

    loadSettings();

    ui->envVars->setModel(itemModel);
}

EnvironmentVariablesDialog::~EnvironmentVariablesDialog()
{
    delete ui;
}

void EnvironmentVariablesDialog::accept()
{
    Configuration &configuration = Configuration::getInstance();
    QMap<QString, QString> newVars;
    int totalRows = itemModel->rowCount();

    for(int r = 0; r < totalRows; r++){
        QStandardItem *itemName = itemModel->item(r, 0);
        QStandardItem *itemValue = itemModel->item(r, 1);

        newVars[itemName->text()] = itemValue->text();
    }

    configuration.updateEnv(titleConsole, newVars);

    QDialog::accept();
}

void EnvironmentVariablesDialog::reject()
{
    QDialog::reject();
}

void EnvironmentVariablesDialog::loadSettings()
{
    Configuration &configuration = Configuration::getInstance();
    QMap<QString, QString> env;
    QMap<QString, QString>::iterator i;
    int row = 0;

    configuration.getEnv(titleConsole, env);

    itemModel->setRowCount(env.size());
    for (i = env.begin(); i != env.end(); ++i)
    {
        itemModel->setItem(row, 0, new QStandardItem(i.key()));
        itemModel->setItem(row, 1, new QStandardItem(i.value()));
        row++;
    }
}

void EnvironmentVariablesDialog::addVar()
{
    QList<QStandardItem *> items;

    QStandardItem *itemName = new QStandardItem("type-name");
    QStandardItem *itemValue = new QStandardItem("type-value");

    items << itemName << itemValue;

    itemModel->appendRow(items);
}

void EnvironmentVariablesDialog::deleteVar()
{
    QItemSelectionModel *itemsSelected = ui->envVars->selectionModel();

    QModelIndexList modelIndexList = itemsSelected->selectedRows();

    if(modelIndexList.size()){
        int row = modelIndexList.at(0).row();

        itemModel->removeRow(row);
    }

}

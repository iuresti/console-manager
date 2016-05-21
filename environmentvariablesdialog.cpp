#include "environmentvariablesdialog.h"
#include "ui_environmentvariablesdialog.h"

#include "constants.h"

#include <QSettings>


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
    QSettings settings(COMPANY_NAME, APP_NAME);
    int totalRows = itemModel->rowCount();

    settings.beginGroup(titleConsole + "/env");
    settings.remove("");

    for(int r = 0; r < totalRows; r++){
        QStandardItem *itemName = itemModel->item(r, 0);
        QStandardItem *itemValue = itemModel->item(r, 1);

        settings.setValue(itemName->text(), itemValue->text());
    }

    settings.endGroup();

    QDialog::accept();
}

void EnvironmentVariablesDialog::reject()
{
    QDialog::reject();
}

void EnvironmentVariablesDialog::loadSettings()
{
    QSettings settings(COMPANY_NAME, APP_NAME);

    settings.beginGroup(titleConsole + "/env");
    QStringList keys = settings.childKeys();
    int row = 0;

    itemModel->setRowCount(keys.size());
    foreach(QString key, keys){
        itemModel->setItem(row, 0, new QStandardItem(key));
        itemModel->setItem(row, 1, new QStandardItem(settings.value(key).toString()));
        row++;
    }

    settings.endGroup();
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

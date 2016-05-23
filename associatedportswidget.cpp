#include "associatedportswidget.h"
#include "ui_associatedportswidget.h"

AssociatedPortsWidget::AssociatedPortsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AssociatedPortsWidget)
{
    ui->setupUi(this);
}

AssociatedPortsWidget::~AssociatedPortsWidget()
{
    delete ui;
}

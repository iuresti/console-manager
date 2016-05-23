#ifndef ASSOCIATEDPORTSWIDGET_H
#define ASSOCIATEDPORTSWIDGET_H

#include <QWidget>

namespace Ui {
class AssociatedPortsWidget;
}

class AssociatedPortsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AssociatedPortsWidget(QWidget *parent = 0);
    ~AssociatedPortsWidget();

private:
    Ui::AssociatedPortsWidget *ui;
};

#endif // ASSOCIATEDPORTSWIDGET_H

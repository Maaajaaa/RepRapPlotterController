#ifndef PLOTFUNCTIONDIALOG_H
#define PLOTFUNCTIONDIALOG_H

#include <QDialog>

namespace Ui {
class plotFunctionDialog;
}

class plotFunctionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit plotFunctionDialog(QWidget *parent = 0);
    ~plotFunctionDialog();

private:
    Ui::plotFunctionDialog *ui;
};

#endif // PLOTFUNCTIONDIALOG_H

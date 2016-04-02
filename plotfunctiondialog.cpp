#include "plotfunctiondialog.h"
#include "ui_plotfunctiondialog.h"

plotFunctionDialog::plotFunctionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::plotFunctionDialog)
{
    ui->setupUi(this);
}

plotFunctionDialog::~plotFunctionDialog()
{
    delete ui;
}

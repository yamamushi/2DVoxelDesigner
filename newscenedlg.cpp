#include "newscenedlg.h"
#include "ui_newscenedlg.h"

NewSceneDlg::NewSceneDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewSceneDlg)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
}

NewSceneDlg::~NewSceneDlg()
{
    delete ui;
}

int NewSceneDlg::getGridSize(){
    if(ui->optSize30->isChecked())
        return 30;
    else if(ui->optSize60->isChecked())
        return 60;
    else if(ui->optSize100->isChecked())
        return 100;
    else {
        return 30;
    }
}

int NewSceneDlg::getSpacingSize(){
    if(ui->optSpacing8->isChecked())
        return 8;
    else if(ui->optSpacing16->isChecked())
        return 16;
    else if(ui->optSpacing32->isChecked())
        return 32;
    else {
        return 32;
    }
}


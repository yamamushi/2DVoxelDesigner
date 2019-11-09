#ifndef NEWSCENEDLG_H
#define NEWSCENEDLG_H

#include <QDialog>

namespace Ui {
class NewSceneDlg;
}

class NewSceneDlg : public QDialog
{
    Q_OBJECT

public:
    explicit NewSceneDlg(QWidget *parent = nullptr);
    ~NewSceneDlg();

public:
    int getGridSize();
    int getSpacingSize();

private:
    Ui::NewSceneDlg *ui;
};

#endif // NEWSCENEDLG_H

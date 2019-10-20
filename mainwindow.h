#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QColorDialog>

#include "graphicsview.h"
#include "newscenedlg.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void updateLineColor();
    void initImageItemProperty();

private slots:
    // Button event's handler
    void onNew_triggered();
    void onSave_triggered();
    void onOpen_triggered();
    void onImportImage_triggered();
    void onClearImage_triggered();
    void onChooseLineColor_triggered();
    void onReactorGuide_triggered(bool);

private:
    Ui::MainWindow *ui;

    GraphicsView    *m_graphicsView;
    QColor          m_lineColor;
};

#endif // MAINWINDOW_H

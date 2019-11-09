#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Setup graphicsview
    m_graphicsView = new GraphicsView(this);
    ui->centralWidget->layout()->addWidget(m_graphicsView);
    initImageItemProperty();

    ui->widReactorPreview->setSpacingSize(m_graphicsView->getSpacingSize());
    ui->widReactorPreview->setSpacingUnit(m_graphicsView->getSpacingUnit());

    // Setup Line-Color from one node to another node
    m_lineColor = Qt::blue;
    updateLineColor();

    // Button's event-handler
    QObject::connect(ui->btnNew, SIGNAL(released()), this, SLOT(onNew_triggered()));
    QObject::connect(ui->btnOpen, SIGNAL(released()), this, SLOT(onOpen_triggered()));
    QObject::connect(ui->btnSave, SIGNAL(released()), this, SLOT(onSave_triggered()));
    QObject::connect(ui->btnModifyImage, SIGNAL(released()), this, SLOT(onImportImage_triggered()));
    QObject::connect(ui->btnClearImage, SIGNAL(released()), this, SLOT(onClearImage_triggered()));
    QObject::connect(ui->btnLineColor, SIGNAL(released()), this, SLOT(onChooseLineColor_triggered()));
    QObject::connect(ui->chkAspectRatio, SIGNAL(toggled(bool)), m_graphicsView, SLOT(onMaintainAspectRatio(bool)));
    QObject::connect(ui->chkLockImage, SIGNAL(toggled(bool)), m_graphicsView, SLOT(onLockImage(bool)));
    QObject::connect(ui->chkShowImage, SIGNAL(toggled(bool)), m_graphicsView, SLOT(onShowImage(bool)));
    QObject::connect(ui->btnReactorGuide, SIGNAL(toggled(bool)), this, SLOT(onReactorGuide_triggered(bool)));
    QObject::connect(ui->btnResetPoint, SIGNAL(released()), this, SLOT(onResetPoint_triggered()));

    // Draw Reactor-Preview
    QObject::connect(m_graphicsView, SIGNAL(reactorChanged(Node*, Node*, Node*, Node*)), ui->widReactorPreview, SLOT(drawReactorPreview(Node*, Node*, Node*, Node*)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initImageItemProperty()
{
    // LOCK/UNLOCK IMAGE
    ui->chkLockImage->setChecked(false);
    m_graphicsView->onLockImage(false);

    // MAINTAIN ASPECT RATIO
    ui->chkAspectRatio->setChecked(true);
    m_graphicsView->onMaintainAspectRatio(true);

    // SHOW/HIDE IMAGE
    ui->chkShowImage->setChecked(true);
    m_graphicsView->onShowImage(true);

}

void MainWindow::onNew_triggered(){
    // Open New-Dialog
    NewSceneDlg dlg(this);
    if(dlg.exec() == QDialog::Accepted){
        initImageItemProperty();

        // REACTOR MODE
        ui->btnReactorGuide->setChecked(false);
        m_graphicsView->onReactorGuide(false);
        ui->widReactorPreview->drawReactorPreview();

        // Create new workspace with grid-size and spacing-size
        m_graphicsView->onNew(dlg.getGridSize(), dlg.getSpacingSize());

        ui->widReactorPreview->setSpacingSize(m_graphicsView->getSpacingSize());
        ui->widReactorPreview->setSpacingUnit(m_graphicsView->getSpacingUnit());

    }
}

void MainWindow::onSave_triggered(){
    // Open Save-Dialog
    QString fileName = QFileDialog::getSaveFileName(
                this,
                tr("Save File"),
                nullptr,
                tr("Image Tracer Files (*.itr)"),
                nullptr,
                nullptr);

    if (!fileName.isEmpty())
        m_graphicsView->onSave(fileName);
}

void MainWindow::onOpen_triggered(){
    // Open Open-Dialog
    QString fileName = QFileDialog::getOpenFileName(
                this,
                tr("Open File"),
                nullptr,
                tr("Image Tracer Files (*.itr)"),
                nullptr,
                nullptr);


    if (!fileName.isEmpty()){
        initImageItemProperty();

        // REACTOR MODE
        ui->btnReactorGuide->setChecked(false);
        m_graphicsView->onReactorGuide(false);
        ui->widReactorPreview->drawReactorPreview();

        m_graphicsView->onOpen(fileName);

        ui->widReactorPreview->setSpacingSize(m_graphicsView->getSpacingSize());
        ui->widReactorPreview->setSpacingUnit(m_graphicsView->getSpacingUnit());

    }
}

void MainWindow::onImportImage_triggered(){
    // Open Open-Dialog
    QString fileName = QFileDialog::getOpenFileName(
                this,
                tr("Import an image"),
                nullptr,
                tr("Image Files (*.png *.jpg)"),
                nullptr,
                nullptr);

    if (!fileName.isEmpty()){
        initImageItemProperty();

        m_graphicsView->onLoadImage(fileName);

        // Set Image Property
        m_graphicsView->onLockImage(ui->chkLockImage->isChecked());
        m_graphicsView->onMaintainAspectRatio(ui->chkAspectRatio->isChecked());
        m_graphicsView->onShowImage(ui->chkShowImage->isChecked());
    }
}


void MainWindow::onClearImage_triggered(){
    // Remove Image
    m_graphicsView->onClearImage();
}

void MainWindow::onChooseLineColor_triggered()
{
    m_lineColor = QColorDialog::getColor(m_lineColor,parentWidget());

    if(m_lineColor.isValid()){
        QString buttonStyle = QString("QPushButton{ \
                background-color: %1; \
                border:none; \
                }\
                QPushButton:hover{ \
                background-color: %2; \
                border:none; \
                } \
                QPushButton:pressed{ \
                background-color: %3; \
                border:none; \
                }").arg(m_lineColor.name(), m_lineColor.lighter(115).name(), m_lineColor.lighter(120).name());
        ui->btnLineColor->setStyleSheet(buttonStyle);
        updateLineColor();
    }
}

void MainWindow::updateLineColor(){
    if(m_lineColor.isValid()){
        QString buttonStyle = QString("QPushButton{ \
                background-color: %1; \
                border:none; \
                }\
                QPushButton:hover{ \
                background-color: %2; \
                border:none; \
                } \
                QPushButton:pressed{ \
                background-color: %3; \
                border:none; \
                }").arg(m_lineColor.name(), m_lineColor.lighter(115).name(), m_lineColor.lighter(120).name());
        ui->btnLineColor->setStyleSheet(buttonStyle);
        m_graphicsView->onUpdateLineColor(m_lineColor);
    }
}

void MainWindow::onReactorGuide_triggered(bool f)
{
    m_graphicsView->onReactorGuide(f);
    // REACTOR GUIDE
    ui->widReactorPreview->drawReactorPreview();

    ui->widReactorPreview->setSpacingSize(m_graphicsView->getSpacingSize());
    ui->widReactorPreview->setSpacingUnit(m_graphicsView->getSpacingUnit());

}

void MainWindow::onResetPoint_triggered()
{
    m_graphicsView->onResetPoint();
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    if(event->matches(QKeySequence::Undo))
    {
        // Ctrl + Z
        m_graphicsView->onUndo();
        qDebug() << "Undo";
    }else if(event->matches(QKeySequence::Redo) || (event->key() == Qt::Key_R && event->modifiers() == Qt::ControlModifier))
    {
        // Shift + Ctrl + Z, Ctrl + R
        m_graphicsView->onRedo();
        qDebug() << "Redo";
    }

}

#include "mainwindow.h"
#include <QMenuBar>
#include <QToolBar>
#include <QAction>
#include <QPushButton>
#include <QStatusBar>
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QColorDialog>
#include <QGuiApplication>
#include <QDateTime>
#include <QScreen>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setGeometry(600, 200, 900, 600);

    //创建菜单栏
    createMenu();

    //创建工具栏
    createToolbar();

    //创建状态栏
    createStatusBar();

    // 创建住窗体
    createCenterWidget();

    setMouseTracking(true);

}

MainWindow::~MainWindow()
{
}

void MainWindow::createMenu()
{
    // 创建菜单栏
    m_menuBar = menuBar();

    // 文件菜单
    m_fileMenu = m_menuBar->addMenu(tr("&File"));

    // 新建
    m_newAct = new QAction(tr("&New"), this);
    m_newAct->setShortcuts(QKeySequence::New);
    m_fileMenu->addAction(m_newAct);
    connect(m_newAct, &QAction::triggered, this, &MainWindow::newFile);

    // 打开
    m_openAct = new QAction(tr("&Open"), this);
    m_openAct->setShortcuts(QKeySequence::Open);
    m_fileMenu->addAction(m_openAct);
    connect(m_openAct, &QAction::triggered, this, &MainWindow::openFile);


    // 保存
    m_saveAct = new QAction(tr("&Save"), this);
    m_saveAct->setShortcuts(QKeySequence::Save);
    m_fileMenu->addAction(m_saveAct);
    connect(m_saveAct, &QAction::triggered, this, &MainWindow::saveFile);

    // 另存为
//    m_saveAsAct = new QAction(tr("&SaveAs"), this);
//    m_saveAsAct->setShortcuts(QKeySequence::SaveAs);
//    m_fileMenu->addAction(m_saveAsAct);
//    connect(m_saveAsAct, &QAction::triggered, this, &MainWindow::saveAsFile);


}

void MainWindow::createToolbar()
{
    // 顶部工具条
    m_toolBar = new QToolBar(this);
    addToolBar(Qt::TopToolBarArea, m_toolBar);

    QPushButton* lineBtn = new QPushButton("Line", this);
    m_toolBar->addWidget(lineBtn);

    QPushButton* penBtn = new QPushButton("pen", this);
    m_toolBar->addWidget(penBtn);

    QPushButton* eraserBtn = new QPushButton("eraser", this);
    m_toolBar->addWidget(eraserBtn);

    QPushButton* textBtn = new QPushButton("text", this);
    m_toolBar->addWidget(textBtn);

    QPushButton* rectBtn = new QPushButton("rect", this);
    m_toolBar->addWidget(rectBtn);

    QPushButton* ellipseBtn = new QPushButton("ellipse", this);
    m_toolBar->addWidget(ellipseBtn);

//    QPushButton* arrowBtn = new QPushButton("arrow", this);
//    m_toolBar->addWidget(arrowBtn);

    QPushButton* colorBtn = new QPushButton("color", this);
    m_toolBar->addWidget(colorBtn);

   // QPushButton* startBtn = new QPushButton("start", this);
   //m_toolBar->addWidget(startBtn);

    connect(lineBtn, &QPushButton::clicked, this, &MainWindow::createLine);
    connect(penBtn, &QPushButton::clicked, this, &MainWindow::createPen);
    connect(eraserBtn, &QPushButton::clicked, this, &MainWindow::createEraser);
    connect(textBtn, &QPushButton::clicked, this, &MainWindow::createText);
    connect(rectBtn, &QPushButton::clicked, this, &MainWindow::createRect);
    connect(ellipseBtn, &QPushButton::clicked, this, &MainWindow::createEllipse);
//    connect(arrowBtn, &QPushButton::clicked, this, &MainWindow::createArrow);

    connect(colorBtn, &QPushButton::clicked, this, &MainWindow::setColor);


    //connect(startBtn, &QPushButton::clicked, this, &MainWindow::start);



}

void MainWindow::createStatusBar()
{
    m_statusBar = statusBar();
    setStatusBar(m_statusBar);

    m_mousePosLabel = new QLabel(this);
    m_statusBar->addWidget(m_mousePosLabel);

}

void MainWindow::createCenterWidget()
{
    m_mScene = new DrawScene(this);

    m_view = new QGraphicsView(m_mScene);
    m_view->setStyleSheet("padding: 0px; border: 0px;");
    m_view->setMouseTracking(true);

    setCentralWidget(m_view);

    // 设置scence的坐标原点
    qreal width = size().width();
    qreal height = size().height();
    //qDebug() << width << height;
    m_mScene->setSceneRect(0 ,0, width, height);
    m_mScene->setBackgroundBrush(QBrush(Qt::white));

    // 去掉滚动条
    m_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    connect(m_mScene, &DrawScene::statusPos, this, &MainWindow::updateStatus);
}

void MainWindow::setPenColor(QColor color)
{
    m_mScene->setPenColor(color);
}

void MainWindow::resizeEvent(QResizeEvent *ev)
{
    qreal width = ev->size().width();
    qreal height = ev->size().height();
    m_mScene->setSceneRect(0 ,0, width, height);
}

void MainWindow::closeEvent(QCloseEvent *ev)
{
    QMessageBox msgBox;
    msgBox.setText("Do you want to save your changes?");
    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Save);
    int ret = msgBox.exec();
    switch (ret) {
    case QMessageBox::Save:
    {
        saveFile();
        ev->accept();
    }
        break;
    case QMessageBox::Discard:
    {
        ev->accept();
    }
        break;
    case QMessageBox::Cancel:
    {
        ev->ignore();
    }
        break;
    default:
        break;
    }
}

void MainWindow::newFile()
{
    QMessageBox msgBox;
    msgBox.setInformativeText("Do you want to save your changes?");
    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Save);
    int ret = msgBox.exec();
    qDebug() << ret;
    switch (ret) {
    case QMessageBox::Save:
    {
        saveFile();
        m_mScene->newScene();
    }
        break;
    case QMessageBox::Discard:
    {
        m_mScene->newScene();
    }
        break;
    default:
        break;
    }
}

void MainWindow::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(
                   this, tr("Open image file"),
                   "./", tr("Image files(*.bmp *.jpg  *.png );;All files (*.*)"));

    if(fileName.isEmpty())
    {
       QMessageBox mesg;
       mesg.warning(this,"警告","打开图片失败!");
       return;
    }
    else
    {
        m_mScene->openFile(fileName);
    }
}

void MainWindow::saveFile()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                                               "drawBoarding.png",  tr("Image files(*.bmp *.jpg  *.png );;All files (*.*)"));

    QSize size = this->size();
    QImage image(size, QImage::Format_RGB32);
    QPainter painter(&image);
    m_mScene->render(&painter);   //关键函数
    image.save(fileName);
}

void MainWindow::saveAsFile()
{

}

void MainWindow::createLine()
{
    m_mScene->setType(DrawScene::Line);
}

void MainWindow::createPen()
{
    m_mScene->setType(DrawScene::Pen);
}

void MainWindow::createEraser()
{
    m_mScene->setType(DrawScene::Eraser);
}

void MainWindow::createText()
{
    m_mScene->setType(DrawScene::Text);
}

void MainWindow::createRect()
{
    m_mScene->setType(DrawScene::Rect);
}

void MainWindow::createEllipse()
{
    m_mScene->setType(DrawScene::Ellipse);
}

void MainWindow::createArrow()
{
    m_mScene->setType(DrawScene::Arrow);
}

void MainWindow::setColor()
{
    QColor col = QColorDialog::getColor(m_mScene->getPen().color(), this);
    qDebug() << col.name();
    setPenColor(col);
    //m_mScene->getPen().setColor(col);
    qDebug() << m_mScene->getPen().color();

}

void MainWindow::updateStatus(QPointF pt)
{
    QString mousPos = QString("%1, %2").arg(pt.x()).arg(pt.y());
    m_mousePosLabel->setText(mousPos);
}

void MainWindow::start()
{
    QScreen *screen = QGuiApplication::primaryScreen();

    QString filePathName = "C:/Users/Monty/Desktop/full-";

    filePathName += QDateTime::currentDateTime().toString("yyyy-MM-dd hh-mm-ss-zzz");

    filePathName += ".jpg";

    if(!screen->grabWindow(0).save(filePathName, "jpg"))
    {

        //cout<<"save full screen failed"<<endl;

    }
}





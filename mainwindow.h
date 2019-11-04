#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QLabel>

#include "drawscene.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;


    // 创建menu栏
    void createMenu();

    // 创建工具栏
    void createToolbar();

    // 创建状态栏
    void createStatusBar();

    void createCenterWidget();

    void setPenColor(QColor color);


protected:
    // 事件
    void resizeEvent(QResizeEvent *ev) override;
    void closeEvent (QCloseEvent *ev ) override;


public slots:
    void newFile();
    void openFile();
    void saveFile();
    void saveAsFile();


    // 创建
    void createLine();
    void createPen();
    void createEraser();
    void createText();
    void createRect();
    void createEllipse();
    void createArrow();

    void setColor();


    // 鼠标信号
    void updateStatus(QPointF pt);


    void start();
private:
    QMenuBar*       m_menuBar;
    QMenu*          m_fileMenu;
    QAction*        m_newAct;
    QAction*        m_openAct;
    QAction*        m_saveAct;
    QAction*        m_saveAsAct;

    QToolBar*       m_toolBar;

    // 状态栏信息
    QStatusBar*     m_statusBar;
    QLabel*         m_mousePosLabel;

    // 主体绘制区间
    DrawScene*      m_mScene;
    QGraphicsView*  m_view;

};
#endif // MAINWINDOW_H

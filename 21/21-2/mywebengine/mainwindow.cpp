#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QWebEngineView>
#include <QLineEdit>

//21-2
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    view = new QWebEngineView(this);
    view->load(QUrl("https://www.qter.org/"));
    setCentralWidget(view);
    resize(1200, 600);


    progress = 0;

    // 关联信号和槽
    connect(view, &QWebEngineView::loadProgress, this, &MainWindow::setProgress);
    connect(view, &QWebEngineView::titleChanged, this, &MainWindow::adjustTitle);
    connect(view, &QWebEngineView::loadFinished, this, &MainWindow::finishLoading);

    locationEdit = new QLineEdit(this);
    locationEdit->setText("https://www.qter.org/");
    connect(locationEdit, &QLineEdit::returnPressed, this, &MainWindow::changeLocation);

    // 向工具栏添加动作和部件
    ui->mainToolBar->addAction(view->pageAction(QWebEnginePage::Back));
    ui->mainToolBar->addAction(view->pageAction(QWebEnginePage::Forward));
    ui->mainToolBar->addAction(view->pageAction(QWebEnginePage::Reload));
    ui->mainToolBar->addAction(view->pageAction(QWebEnginePage::Stop));
    ui->mainToolBar->addWidget(locationEdit);

    //21-2
    connect(view, &QWebEngineView::iconUrlChanged,
            this, &MainWindow::handleIconUrlChanged);
    manager = new QNetworkAccessManager(this);


    QAction *zoomIn = new QAction(tr("放大"));
    zoomIn->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_Plus));
    connect(zoomIn, &QAction::triggered, this, [=]() {
        view->setZoomFactor(view->zoomFactor() + 0.1);
        ui->statusbar->showMessage(tr("缩放%1%").arg(view->zoomFactor()*100));
    });
    QAction *zoomOut = new QAction(tr("缩小"));
    zoomOut->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_Minus));
    connect(zoomOut, &QAction::triggered, this, [=]() {
        view->setZoomFactor(view->zoomFactor() - 0.1);
        ui->statusbar->showMessage(tr("缩放%1%").arg(view->zoomFactor()*100));
    });
    QAction *resetZoom = new QAction(tr("重置"));
    resetZoom->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_0));
    connect(resetZoom, &QAction::triggered, this, [=]() {
        view->setZoomFactor(1.0);
        ui->statusbar->showMessage(tr("缩放%1%").arg(view->zoomFactor()*100));
    });
    ui->mainToolBar->addAction(zoomIn);
    ui->mainToolBar->addAction(zoomOut);
    ui->mainToolBar->addAction(resetZoom);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::changeLocation()
{
    QUrl url = QUrl(locationEdit->text());
    view->load(url);
    view->setFocus();
}

void MainWindow::setProgress(int p)
{
    progress = p;
    adjustTitle();
}

void MainWindow::adjustTitle()
{
    if ( progress <= 0 || progress >= 100) {
        setWindowTitle(view->title());
    } else {
        setWindowTitle(QString("%1 (%2%)").arg(view->title()).arg(progress));
    }
}

void MainWindow::finishLoading(bool finished)
{
    if (finished) {
        progress = 100;
        setWindowTitle(view->title());
    } else {
        setWindowTitle("web page loading error!");
    }
}

//21-2
void MainWindow::handleIconUrlChanged(const QUrl &url)
{
    QNetworkRequest iconRequest(url);
    QNetworkReply *iconReply = manager->get(iconRequest);
    iconReply->setParent(this);
    connect(iconReply, &QNetworkReply::finished,
            this, &MainWindow::handleIconLoaded);
}


//21-2
void MainWindow::handleIconLoaded()
{
    QIcon icon;
    QNetworkReply *iconReply = qobject_cast<QNetworkReply*>(sender());
    if (iconReply && iconReply->error() == QNetworkReply::NoError) {
        QByteArray data = iconReply->readAll();
        QPixmap pixmap;
        pixmap.loadFromData(data);
        icon.addPixmap(pixmap);
        iconReply->deleteLater();
    } else {
        icon = QIcon(QStringLiteral("../mywebengine/defaulticon.png"));
    }
    setWindowIcon(icon);
}





















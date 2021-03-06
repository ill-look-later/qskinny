#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QskWindow.h>

class QQuickItem;

class MainWindow : public QskWindow
{
public:
    MainWindow();

private:
    QQuickItem* headerBar() const;
    QQuickItem* mainContent() const;
    QQuickItem* footerBar() const;
};

#endif

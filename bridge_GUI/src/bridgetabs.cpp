#include "include/bridgetabs.h"
#include "include/mainwindow.h"

BridgeTabs::BridgeTabs(QWidget *parent) :
    QTabWidget(parent)
{
    setTabsClosable(true);


}

void BridgeTabs::newConfig()
{
    MainWindow *newWindow = new MainWindow(this);
    this->addTab(newWindow, "Untitled");
    this->setCurrentWidget(newWindow);

}

void BridgeTabs::closeTab()
{
    removeTab(currentIndex());
}


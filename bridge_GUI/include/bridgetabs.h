#ifndef BRIDGETABS_H
#define BRIDGETABS_H

#include <QTabWidget>
#include <vector>

class MainWindow;

class BridgeTabs : public QTabWidget
{
    Q_OBJECT
private:
public:
    explicit BridgeTabs(QWidget *parent = 0);

signals:

public slots:
    void newConfig();
    void closeTab();

};

#endif // BRIDGETABS_H

#include "include/mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include "include/bridgetabs.h"

MainWindow::MainWindow(BridgeTabs* _tabs, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{


    tabs = _tabs;
    ui->setupUi(this);
    sessionLaunched = false;
    sameTmp = false;
    setWindowTitle(tr("YARP ROS BRIDGE GENERATOR"));
    createActions();
    createMenus();

    connect(ui->addRowButton, SIGNAL(clicked()), this, SLOT(addData()));
    connect(ui->removeRowButton, SIGNAL(clicked()), this, SLOT(removeData()));
    connect(ui->automaticCB,SIGNAL(stateChanged(int)),this,SLOT(trigger(int)));
    connect(ui->typeCB,SIGNAL(currentIndexChanged(int)),this,SLOT(trigger(int)));

    connect(ui->nameLE,SIGNAL(textChanged(QString)),this,SLOT(iniEdited()));
    connect(ui->topicLE,SIGNAL(textChanged(QString)),this,SLOT(iniEdited()));
    connect(ui->portLE,SIGNAL(textChanged(QString)),this,SLOT(iniEdited()));
    connect(ui->directionCB,SIGNAL(currentIndexChanged(QString)),this,SLOT(iniEdited()));
    connect(ui->typeCB,SIGNAL(currentIndexChanged(QString)),this,SLOT(iniEdited()));
    connect(ui->automaticCB,SIGNAL(stateChanged(int)),this,SLOT(iniEdited()));
    connect(ui->tableWidget,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(iniEdited()));
    connect(ui->addRowButton,SIGNAL(clicked()),this,SLOT(iniEdited()));
    connect(ui->removeRowButton,SIGNAL(clicked()),this,SLOT(iniEdited()));
}

void MainWindow::iniEdited()
{
    sameTmp = false;
}

MainWindow::~MainWindow()
{
    if (sessionLaunched)
    {
        if (contentsWindow->isVisible())
            contentsWindow->close();
        else
            terminateLaunch();
    }
    remove("/tmp/bridge_conf.ini");
    delete ui;
}


void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAct);

    fileMenu->addAction(openAct);

    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveAsAct);

    fileMenu->addSeparator();

    fileMenu->addAction(closeAct);
    fileMenu->addAction(quitAct);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(cutAct);
    editMenu->addAction(copyAct);
    editMenu->addAction(pasteAct);

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
}


void MainWindow::createActions()
{
    newAct = new QAction(tr("&New"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Create a new configuration"));
    connect(newAct, SIGNAL(triggered()), tabs, SLOT(newConfig()));

    openAct = new QAction(tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing configuration (.ini)"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    saveAct = new QAction(tr("&Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save the configuration as a .ini file"));
    connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

    saveAsAct = new QAction(tr("Sa&ve As"), this);
    saveAsAct->setObjectName("saveAsAct");
    saveAsAct->setStatusTip(tr("Save the configuration as a .ini file"));
    connect(saveAsAct, SIGNAL(triggered()), this, SLOT(save()));


    closeAct = new QAction(tr("Close"), this);
    closeAct->setShortcut(tr("Ctrl+W"));
    closeAct->setStatusTip(tr("Close the current tab"));
    connect(closeAct, SIGNAL(triggered()), this, SLOT(closeTab()));

    quitAct = new QAction(tr("Quit"), this);
    quitAct->setShortcut(tr("Ctrl+Q"));
    quitAct->setStatusTip(tr("Quit the bridge"));
    connect(quitAct, SIGNAL(triggered()), tabs, SLOT(close()));

    cutAct = new QAction(tr("Cu&t"), this);
    cutAct->setShortcuts(QKeySequence::Cut);
    cutAct->setStatusTip(tr("Cut the current selection's contents to the "
                            "clipboard"));
    connect(cutAct, SIGNAL(triggered()), this, SLOT(cut()));

    copyAct = new QAction(tr("&Copy"), this);
    copyAct->setShortcut(tr("Ctrl+C"));
    copyAct->setStatusTip(tr("Copy the current selection's contents to the "
                             "clipboard"));
    connect(copyAct, SIGNAL(triggered()), this, SLOT(copy()));

    pasteAct = new QAction(tr("&Paste"), this);
    pasteAct->setShortcuts(QKeySequence::Paste);
    pasteAct->setStatusTip(tr("Paste the clipboard's contents into the current "
                              "selection"));
    connect(pasteAct, SIGNAL(triggered()), this, SLOT(paste()));

    aboutAct = new QAction(tr("&About"), this);
    aboutAct->setStatusTip(tr("About the application"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(displayMessageBox()));

    connect(ui->launchButton,SIGNAL(clicked()), this, SLOT(launch()));
}

void MainWindow::addData() {
    int row = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(row);
}

void MainWindow::removeData() {
    int row = ui->tableWidget->currentRow();
    ui->tableWidget->removeRow(row);
}

void MainWindow::copy()
{
    QWidget *widget = this->focusWidget();
    if (widget->inherits("QLineEdit"))
    {
        QLineEdit *le = qobject_cast<QLineEdit*>(widget);
        le->copy();
    }
}

void MainWindow::cut()
{
    QWidget *widget = this->focusWidget();
    if (widget->inherits("QLineEdit"))
    {
        QLineEdit *le = qobject_cast<QLineEdit*>(widget);
        le->cut();
    }
}

void MainWindow::paste()
{
    QWidget *widget = this->focusWidget();
    if (widget->inherits("QLineEdit"))
    {
        QLineEdit *le = qobject_cast<QLineEdit*>(widget);
        le->paste();
    }
}

void MainWindow::trigger(int i)
{
    i = !ui->automaticCB->isChecked() && !ui->typeCB->currentIndex();
    ui->tableWidget->setEnabled(i);
}

void MainWindow::displayMessageBox()
{
    QString message("YARP ROS BRIDGE GENERATOR: v0.1\n\n");
    message.append("Copyright (C) 2013 MACSi Project\n");
    message.append("Author: Woody Rousseau\n");
    message.append("email: woody.rousseau@ensta-paristech.fr\n");
    message.append("website: www.macsi.isir.upmc.fr\n");
    message.append("Permission is granted to copy, distribute, and/or modify this program\n");
    message.append("under the terms of the GNU General Public License, version 2 or any\n");
    message.append("later version published by the Free Software Foundation.\n");
    message.append("\n");
    message.append("This program is distributed in the hope that it will be useful, but\n");
    message.append("WITHOUT ANY WARRANTY; without even the implied warranty of\n");
    message.append("MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General\n");
    message.append("Public License for more details\n");
    aboutBox->about(this,"About", message);
}

bool MainWindow::save(bool tmp)
{
    this->setFocus();
    if(checkFields())
    {
        std::map<std::string,std::string> data;
        std::map<std::string,std::string> groups;
        data.insert({"name", ui->nameLE->text().toStdString()});
        data.insert({"bridgetype",to_string(this->ui->directionCB->currentIndex())});
        data.insert({"datatype",to_string(this->ui->typeCB->currentIndex())});
        data.insert({"topic",this->ui->topicLE->text().toStdString()});
        data.insert({"port",this->ui->portLE->text().toStdString()});
        data.insert({"numberofgroups",to_string(ui->tableWidget->rowCount())});

        for (int i = 0; i < ui->tableWidget->rowCount(); i++)
        {
            groups.insert({ui->tableWidget->item(i,1)->text().toStdString(),ui->tableWidget->item(i,0)->text().toStdString()});
        }
        data.insert({"automatic",boolToNumberQString(ui->automaticCB->isChecked()).toStdString()});
        if ((QObject::sender()->objectName() == "saveAsAct" || currentFileName.isEmpty()) && !tmp)
        {
            QString defaultPath = "/home/";
            defaultPath.append(ui->nameLE->text());
            QString filename = QFileDialog::getSaveFileName(this,tr("Save Configuration"), defaultPath,tr("YARP Resources (*.ini)"));
            if (!filename.isEmpty())
            {
                currentFileName = filename;
                if (!filename.endsWith(".ini"))
                    currentFileName.append(".ini");
            }
        }
        if (!currentFileName.isEmpty() || tmp)
        {
            QString filename = tmp ? "/tmp/bridge_conf.ini" : currentFileName;
            if (!sameTmp)
            {
                IOHandler iniSave(data,groups,filename);
                iniSave.saveIni();
                if (tmp)
                    sameTmp = true;
            }
            QString title = "YARP ROS BRIDGE GENERATOR - ";
            title.append(currentFileName);
            setWindowTitle(title);
            return true;

        }
    }
    return false;
}

void MainWindow::closeTab()
{
    if (isPartiallyFilled())
    {
        bool hasChosen = false;
        while (!hasChosen)
        {
            int answer = aboutBox->question(this,"Save modifications?","Would you like to save this configuration ?",QMessageBox::Cancel, QMessageBox::Yes,QMessageBox::No);
            if (answer == QMessageBox::Cancel)
            {
                return;
            }
            else if (answer == QMessageBox::Yes)
                hasChosen = save();
            else
                hasChosen = true;
        }

    }
    if (tabs->count() > 1)
        tabs->closeTab();
    else
        tabs->close();
}

void MainWindow::open()
{
    if (isPartiallyFilled())
    {
        MainWindow *window = new MainWindow(tabs);
        doOpen(window);
        QStringList pathList = window->currentFileName.split("/");
        QString shortFileName = pathList.at(pathList.size()-1);
        tabs->addTab(window,shortFileName);
    }
    else
        doOpen(this);
}

void MainWindow::doOpen(MainWindow* window)
{
    QString filename = QFileDialog::getOpenFileName(this,tr("Open Configuration"), "/home",tr("YARP Resources (*.ini)"));
    if (!filename.isEmpty())
    {
        window->currentFileName = filename;
        map<string,string> data, types;
        IOHandler iniOpen(data,types,window->currentFileName);
        window->ui->nameLE->setText(iniOpen.getField("name"));
        window->ui->topicLE->setText(iniOpen.getField("topic"));
        window->ui->portLE->setText(iniOpen.getField("port"));
        window->ui->directionCB->setCurrentIndex(iniOpen.checkComboBox("bridgetype"));
        window->ui->typeCB->setCurrentIndex(iniOpen.checkComboBox("datatype"));
        window->ui->automaticCB->setChecked(iniOpen.getField("automatic").toInt());
        int rows = iniOpen.getField("numberofgroups").toInt();
        for (int i = 0; i < rows; i ++)
        {
            window->ui->tableWidget->setRowCount(rows);
            for (int ridx = 0 ; ridx < rows ; ridx++ )
            {
                  QTableWidgetItem* name = new QTableWidgetItem();
                  name->setText(iniOpen.getGroup("groups",ridx));
                  window->ui->tableWidget->setItem(ridx,1,name);
                  QTableWidgetItem* type = new QTableWidgetItem();
                  type->setText(iniOpen.getType(name->text()));
                  window->ui->tableWidget->setItem(ridx,0,type);
            }

        }
        QString title = "YARP ROS BRIDGE GENERATOR - ";
        title.append(window->currentFileName);
        setWindowTitle(title);
    }
}

void MainWindow::clearAllFields()
{
    ui->nameLE->clear();
    ui->topicLE->clear();
    ui->portLE->clear();
    ui->tableWidget->clearContents();
    while (ui->tableWidget->rowCount() > 0)
    {
        ui->tableWidget->removeRow(0);
    }
    ui->automaticCB->setChecked(false);

}



bool MainWindow::checkFields()
{
    QString message;
    if (ui->nameLE->text().isEmpty())
        message.append("Please state a name for the module\n");
    if (ui->topicLE->text().isEmpty())
        message.append("Please state the ROS topic used by the bridge\n");
    if (ui->portLE->text().isEmpty())
        message.append("Please state the YARP port used by the bridge\n");
    for (int i = 0; i < ui->tableWidget->rowCount(); i++)
    {
        if (ui->tableWidget->item(i,0) == 0 || ui->tableWidget->item(i,1) == 0)
            message.append("Some values were not filled (in the table)\n");
    }
    if (!message.isEmpty())
    {
        aboutBox->about(this,"Warning", message);
        return false;
    }
    QString devicePort = "/";
    devicePort.append(ui->portLE->text());
    return checkPort(ui->typeCB->currentIndex(), devicePort.toStdString());
}

bool MainWindow::isPartiallyFilled()
{
    if (!ui->nameLE->text().isEmpty())
        return true;
    if (!ui->topicLE->text().isEmpty())
        return true;
    if (!ui->portLE->text().isEmpty())
        return true;
    if (ui->tableWidget->rowCount() != 0)
        return true;
    return false;
}

inline const QString MainWindow::boolToNumberQString(bool b)
{
    return b ? QString("1") : QString("0");

}

bool MainWindow::checkPort(int dataType, std::string devicePort)
{
    QString message("You are not sending raw data and yet the device port you selected contains \"cam\".\n");
    message.append("Is this intentionnal ?");
    if (!dataType && devicePort.find("cam") != std::string::npos)
    {
        int answer = aboutBox->question(this, "Continue ?", message, QMessageBox::Yes, QMessageBox::No);
        if (answer == QMessageBox::No)
            return false;
    }
    return true;
}

void MainWindow::launch()
{
    if (sessionLaunched)
    {
        if (contentsWindow->isVisible())
            contentsWindow->close();
        else
            terminateLaunch();
    }

    if (save(true))
    {

        sessionLaunched = true;
        process = new QProcess(this);

        QString path;
        QFile file("gui.conf");
        bool locate = true;
        if(file.exists())
        {
            file.open(QIODevice::ReadOnly | QIODevice::Text);
            path = file.readLine();
            QFile test(path+"/yarprosbridge");
            if (test.exists())
                locate = false;
            file.close();
        }
        if (locate)
        {
            path.clear();
            path = QFileDialog::getExistingDirectory(this,tr("Please locate the directory in which \"yarprosbridge\" is stored"), "/home");
            QFile test(path+"/yarprosbridge");
            if (!test.exists())
                return;
            file.open(QIODevice::WriteOnly | QIODevice::Text);
            QTextStream out(&file);
            out << path;
            file.close();
        }
        process->setWorkingDirectory(path);


        contentsWindow = new ConsoleWindow();
        contentsWindow->show();

        process->start(getenv("SHELL"), QStringList() << "--login");

        setpgid(process->pid(),0);
        contentsWindow->setProcessPid(process->pid());

        if (process->waitForStarted(-1))
        {
            connect(contentsWindow,SIGNAL(aboutToClose()),this,SLOT(terminateLaunch()));
            process->write("./yarprosbridge --context /tmp --from bridge_conf.ini --gui\n");
            connect(process,SIGNAL(readyReadStandardOutput()),this,SLOT(readyReadStandardOutput()));
            connect(process,SIGNAL(readyReadStandardError()),this,SLOT(readyReadStandardError()));
        }        
    }
}

void MainWindow::readyReadStandardOutput(){
    if (contentsWindow != 0)
    {
        contentsWindow->setTextColor(QColor(Qt::blue));
        contentsWindow->append(QString::fromUtf8(process->readAllStandardOutput()));
    }
}

void MainWindow::readyReadStandardError()
{
    if (contentsWindow != 0)
    {
        contentsWindow->setTextColor(QColor(Qt::red));
        contentsWindow->append(QString::fromUtf8(process->readAllStandardError()));
    }
}

void MainWindow::terminateLaunch()
{
    pid_t pid = process->pid();
    contentsWindow->deleteLater();
    process->blockSignals(true);
    process->close();
    if (pid != 0)
    {
        kill(-pid,9);
        kill(-pid,9);
        kill(-pid,9);
    }
    delete(process);
    sessionLaunched = false;
}

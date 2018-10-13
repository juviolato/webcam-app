#ifndef MENU_H
#define MENU_H
#include <QApplication>
#include <QObject>
#include <QWidget>
#include <QInputDialog>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <iostream>
using namespace std;

const QString INPUT_BOX_TITLE = "Enter input";
const QString INPUT_BOX_PROMPT = "Your input:";
const QString READ_ERROR_MESSAGE = "Could not read input";

class Dialog    // Widgets to communicate with the user
{
public:
    QString readUserInput();
    void showMessage(QString message);

    Dialog() {}
    ~Dialog() {}
};

class Menu
{
private:
    QWidget *window;
    QVBoxLayout *organizer;
    int xPosition;
    int yPosition;

public:
    template <class T, class C> void addButton(QString buttonName, T* receiver, void(C::*callback)()) // Adds button to menu interface and connects its clicked signal to the callback function received
    {
        QPushButton *button = new QPushButton(buttonName);
        button->setGeometry(QRect(QPoint(100, 100), QSize(200, 50)));
        organizer->addWidget(button);
        button->show();
        QObject::connect(button, &QPushButton::clicked, receiver, callback);
    }
    void display();
    void setPosition(int x, int y);
    void getPosition(int& x, int& y);

    Menu();
    ~Menu();
};

#endif // MENU_H

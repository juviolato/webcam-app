#include "gui.h"

void Menu::display()
{
    window->setWindowTitle("Menu");
    window->setGeometry(xPosition, yPosition, 200, window->height());
    window->show();
}

void Menu::setPosition(int x, int y)
{
    xPosition = x;
    yPosition = y;
}

void Menu::getPosition(int& x, int& y)
{
    x = xPosition;
    y = yPosition;
}

Menu::Menu()
{
    window = new QWidget();
    organizer = new QVBoxLayout(window);
    xPosition = 0;
    yPosition = 0;
}

Menu::~Menu()
{
    window->~QWidget();
    organizer->~QVBoxLayout();
}

QString Dialog::readUserInput()
{
    QInputDialog *popup = new QInputDialog(nullptr, nullptr);
    QString input;
    bool read;

    do
    {
        input = popup->getText(nullptr, INPUT_BOX_TITLE, INPUT_BOX_PROMPT, QLineEdit::Normal, "", &read);
        if (read && input.isEmpty())
            QMessageBox::information(nullptr, "ERROR", READ_ERROR_MESSAGE);

    } while (read && input.isEmpty());

    if (read)
        return input;
    else
        return nullptr;     // Returns nullptr when the user clicks the "Cancel" button
}

void Dialog::showMessage(QString message)
{
    QMessageBox *popup = new QMessageBox();
    popup->setWindowTitle("Message");
    popup->setText(message);
    popup->setButtonText(0, "Ok");
    popup->show();
}

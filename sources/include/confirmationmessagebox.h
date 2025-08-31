#ifndef CONFIRMATIONMESSAGEBOX_H
#define CONFIRMATIONMESSAGEBOX_H

#include <QMessageBox>
#include <qpushbutton.h>

class ConfirmationMessageBox : public QMessageBox
{
public:
    explicit ConfirmationMessageBox(QWidget *parent, QString text)
        : QMessageBox(parent)
    {
        setIcon(QMessageBox::Question);
        setWindowTitle("Удаление материала");
        setText(text);
        setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        button(QMessageBox::Yes)->setText("Да");
        button(QMessageBox::No)->setText("Нет");
        setWindowIcon(QIcon(":/images/logo/images/icon.ico"));
    }
};

#endif // CONFIRMATIONMESSAGEBOX_H

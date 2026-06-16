#ifndef DIALOG1_H
#define DIALOG1_H

#include <QDialog>

class QLineEdit;
class QPushButton;

class Dialog1 : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog1(QWidget* parent = nullptr);
    ~Dialog1() override = default;

private slots:
    void onOkClicked();

private:
    QLineEdit* m_lineEdit = nullptr;
    QPushButton* m_okButton = nullptr;
};

#endif // DIALOG1_H
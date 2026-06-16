#ifndef DIALOG3_H
#define DIALOG3_H

#include <QDialog>

class QLineEdit;
class QPushButton;

class Dialog3 : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog3(QWidget* parent = nullptr);
    ~Dialog3() override = default;

private slots:
    void onOkClicked();

private:
    QLineEdit* m_lineEdit = nullptr;
    QPushButton* m_okButton = nullptr;
};

#endif // DIALOG3_H
#ifndef DIALOG2_H
#define DIALOG2_H

#include <QDialog>

class QLineEdit;
class QPushButton;

class Dialog2 : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog2(QWidget* parent = nullptr);
    ~Dialog2() override = default;

private slots:
    void onOkClicked();

private:
public:
    QLineEdit* m_fltEditYwAlmH = nullptr;
    QLineEdit* m_fltEditYwAlmL = nullptr;
    QLineEdit* m_fltEditYwWorkH = nullptr;
    QLineEdit* m_fltEditYwWorkL = nullptr;
    QPushButton* m_okButton = nullptr;
};

#endif // DIALOG2_H
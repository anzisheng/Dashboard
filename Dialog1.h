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

//private:
    public:
    QLineEdit* m_fltEditPressAlmH = nullptr;
    QLineEdit* m_fltEditPressAlmL = nullptr;
	QLineEdit* m_fltEditParaP = nullptr;
	QLineEdit* m_fltEditParaI = nullptr;
	QLineEdit* m_fltEditParaD = nullptr;
    QPushButton* m_okButton = nullptr;
};

#endif // DIALOG1_H
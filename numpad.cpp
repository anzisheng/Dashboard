#include "numpad.h"
#include <QGridLayout>
#include <QPushButton>

NumPad::NumPad(QWidget *parent)
    : QWidget(parent), m_display(nullptr)
{
    setupUI();
}

void NumPad::setupUI()
{
    QGridLayout *layout = new QGridLayout(this);
    layout->setSpacing(5);

    const QStringList keys = {
        "7", "8", "9", "⌫",
        "4", "5", "6", "C",
        "1", "2", "3", "↵",
        "0", ".", "", ""
    };

    int row = 0, col = 0;
    for (const QString &key : keys) {
        if (key.isEmpty()) {
            QWidget *spacer = new QWidget(this);
            spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            layout->addWidget(spacer, row, col);
        } else {
            QPushButton *btn = new QPushButton(key, this);
            btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            btn->setMinimumSize(50, 40);

            if (key == "⌫") {
                btn->setObjectName("backspace");
                connect(btn, &QPushButton::clicked, this, &NumPad::onBackspaceClicked);
            } else if (key == "C") {
                btn->setObjectName("clear");
                connect(btn, &QPushButton::clicked, this, &NumPad::onClearClicked);
            } else if (key == "↵") {
                btn->setObjectName("enter");
                connect(btn, &QPushButton::clicked, this, &NumPad::onEnterClicked);
            } else {
                btn->setObjectName("digit");
                connect(btn, &QPushButton::clicked, this, &NumPad::onDigitButtonClicked);
            }
            layout->addWidget(btn, row, col);
        }
        col++;
        if (col > 3) { col = 0; row++; }
    }

    setStyleSheet(R"(
        QPushButton#digit {
            background-color: #f0f0f0;
            border: 1px solid #ccc;
            border-radius: 4px;
            font-size: 18px;
        }
        QPushButton#digit:hover { background-color: #e0e0e0; }
        QPushButton#digit:pressed { background-color: #d0d0d0; }
        QPushButton#backspace, QPushButton#clear {
            background-color: #f9d7d7;
            border: 1px solid #d99;
            border-radius: 4px;
            font-size: 18px;
        }
        QPushButton#backspace:hover, QPushButton#clear:hover { background-color: #f5c2c2; }
        QPushButton#enter {
            background-color: #b7d7b7;
            border: 1px solid #8b8;
            border-radius: 4px;
            font-size: 18px;
        }
        QPushButton#enter:hover { background-color: #a5c9a5; }
    )");
}

void NumPad::setDisplay(QLineEdit *display)
{
    if (m_display) {
        disconnect(this, nullptr, m_display, nullptr);
    }
    m_display = display;
    if (m_display) {
        connect(this, &NumPad::digitClicked, m_display, [this](const QString &d) {
            m_display->insert(d);
        });
        connect(this, &NumPad::backspaceClicked, m_display, [this]() {
            m_display->backspace();
        });
        connect(this, &NumPad::clearClicked, m_display, [this]() {
            m_display->clear();
        });
        // enterClicked 不自动处理，由对话框接管
    }
}

void NumPad::onDigitButtonClicked()
{
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    if (btn) emit digitClicked(btn->text());
}

void NumPad::onBackspaceClicked() { emit backspaceClicked(); }
void NumPad::onClearClicked()     { emit clearClicked(); }
void NumPad::onEnterClicked()     { emit enterClicked(); }
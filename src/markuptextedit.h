#pragma once

#include <QTextEdit>

class MarkupTextEdit : public QTextEdit
{
    Q_OBJECT
public:
    MarkupTextEdit(QWidget * parent = Q_NULLPTR);

private slots:
    void onTextChanged();

};

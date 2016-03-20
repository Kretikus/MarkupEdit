#include "markuptextedit.h"

#include <QDebug>

#include <QTextBlock>
#include <QTextCharFormat>
#include <QTextCursor>
#include <QTextBlockFormat>
#include <QSignalBlocker>

namespace Format {

    QTextCharFormat defaultCharFormat;
    QTextBlockFormat defaultBlockFormat;

    QTextBlockFormat indentFmt() {
        QTextBlockFormat ret = defaultBlockFormat;
        ret.setIndent(1);
        return ret;
    }

    QTextCharFormat h1Format() {
        QTextCharFormat ret = defaultCharFormat;
        QFont font = ret.font();
        font.setBold(true);
        ret.setFont(font);
        return ret;
    }

    static void setFormat(const QTextCursor & cursor, const QTextCharFormat & charFormat,
                          const QTextBlockFormat & blockFormat = Format::defaultBlockFormat)
    {
        // change font for current block's fragments
        for (QTextBlock::iterator it = cursor.block().begin(); !(it.atEnd()); ++it)
        {
            QTextCursor tempCursor = cursor;
            tempCursor.setPosition(it.fragment().position());
            tempCursor.setPosition(it.fragment().position() + it.fragment().length(), QTextCursor::KeepAnchor);
            tempCursor.setCharFormat(charFormat);
            tempCursor.setBlockFormat(blockFormat);
        }
    }
}


MarkupTextEdit::MarkupTextEdit(QWidget * parent)
    : QTextEdit(parent)
{
    connect(this, SIGNAL(textChanged()), SLOT(onTextChanged()));
    Format::defaultCharFormat = textCursor().blockCharFormat();
    Format::defaultBlockFormat = textCursor().blockFormat();
    Format::defaultBlockFormat.setIndent(2);
    Format::setFormat(textCursor(), Format::defaultCharFormat);
}

void MarkupTextEdit::onTextChanged()
{
    QSignalBlocker sBlocker(this);
    QTextCursor cursor = textCursor();
    QTextBlock block = cursor.block();
    const QString t = block.text();
    qDebug() << t;
    if (t.length()==1) {
        Format::setFormat(cursor, Format::defaultCharFormat);
    } else if (t.startsWith("# ")) {
         Format::setFormat(cursor, Format::h1Format(), Format::indentFmt());
    } else if (t.startsWith("## ")) {
        Format::setFormat(cursor, Format::h1Format());
   }
   else if (t.startsWith("### ")) {
            Format::setFormat(cursor, Format::h1Format());
   }
}


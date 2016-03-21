#include "markuptextedit.h"

#include <QDebug>

#include <QTextBlock>
#include <QTextBlockFormat>
#include <QTextCharFormat>
#include <QTextCursor>
#include <QSignalBlocker>

namespace Format {

    QTextCharFormat defaultCharFormat;
    QTextBlockFormat defaultBlockFormat;

    const qreal defaultMargin = 40.0;
    qreal defaultIndent = 0.0;

    QTextCharFormat h1Format() {
        QTextCharFormat ret = defaultCharFormat;
        QFont font = ret.font();
        font.setBold(true);
        ret.setFont(font);

        return ret;
    }

    QTextBlockFormat h1BlockFormat() {
        QFontMetrics fm(h1Format().font());
        QTextBlockFormat ret = defaultBlockFormat;
        qreal newMargin = ret.leftMargin() - fm.width("# ");
        qDebug() << "Old Margin " << ret.leftMargin() << " new " << newMargin;
        ret.setLeftMargin(newMargin);
        return ret;
    }

    QTextCharFormat h2Format() {
        QTextCharFormat ret = defaultCharFormat;
        QFont font = ret.font();
        font.setPointSize(font.pointSize() + 1);
        font.setBold(true);
        ret.setFont(font);

        return ret;
    }

    QTextBlockFormat h2BlockFormat() {
        QFontMetrics fm(h2Format().font());
        QTextBlockFormat ret = defaultBlockFormat;
        qreal newMargin = ret.leftMargin() - fm.width("## ");
        qDebug() << "Old Margin " << ret.leftMargin() << " new " << newMargin;
        ret.setLeftMargin(newMargin);
        return ret;
    }

    QTextCharFormat h3Format() {
        QTextCharFormat ret = defaultCharFormat;
        QFont font = ret.font();
        font.setPointSize(font.pointSize() + 3);
        font.setBold(true);
        ret.setFont(font);

        return ret;
    }

    QTextBlockFormat h3BlockFormat() {
        QFontMetrics fm(h3Format().font());
        QTextBlockFormat ret = defaultBlockFormat;
        qreal newMargin = ret.leftMargin() - fm.width("### ");
        qDebug() << "Old Margin " << ret.leftMargin() << " new " << newMargin;
        ret.setLeftMargin(newMargin);
        return ret;
    }

    QTextCharFormat bulletFormat() {
        QTextCharFormat ret = defaultCharFormat;
        //QFont font = ret.font();
        //font.setPointSize(font.pointSize() + 3);
        //font.setBold(true);
        //ret.setFont(font);
        return ret;
    }

    QTextBlockFormat bulletBlockFormat() {
        QFontMetrics fm(bulletFormat().font());
        QTextBlockFormat ret = defaultBlockFormat;
        qreal newMargin = ret.leftMargin() - fm.width("- ");
        ret.setLeftMargin(newMargin);
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

    QTextCursor cursor = textCursor();

    Format::defaultCharFormat = textCursor().blockCharFormat();
    Format::defaultBlockFormat = textCursor().blockFormat();
    QFontMetrics fm(cursor.blockCharFormat().font());
    Format::defaultIndent = Format::defaultMargin + fm.width("99999. ");
    Format::defaultBlockFormat.setLeftMargin(Format::defaultIndent);

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
         Format::setFormat(cursor, Format::h1Format(), Format::h1BlockFormat());
    } else if (t.startsWith("## ")) {
        Format::setFormat(cursor, Format::h2Format(), Format::h2BlockFormat());
    } else if (t.startsWith("### ")) {
        Format::setFormat(cursor, Format::h3Format(), Format::h3BlockFormat());
    } else if (t.startsWith("- ")) {
        Format::setFormat(cursor, Format::bulletFormat(), Format::bulletBlockFormat());
    }
}

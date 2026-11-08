#include "purchase_delegate.h"
#include <QDebug>
#include <QLineEdit>
#include <QPainter>
#include <QPen>
#include <QColor>
#include <QPalette>
#include <QStyle>

PurchaseDelegate::PurchaseDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
    qDebug() << "[DELEGATE] создан PurchaseDelegate";
}

QWidget *PurchaseDelegate::createEditor(QWidget *parent,
                                        const QStyleOptionViewItem &,
                                        const QModelIndex &) const
{
    qDebug() << "[EDITOR] createEditor: открыт QLineEdit";
    return new QLineEdit(parent);
}

void PurchaseDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    const QString value = index.data(Qt::EditRole).toString();
    if (auto *le = qobject_cast<QLineEdit*>(editor)) {
        le->setText(value);
        qDebug() << "[EDITOR] setEditorData:" << value;
    }
}

void PurchaseDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                    const QModelIndex &index) const
{
    if (auto *le = qobject_cast<QLineEdit*>(editor)) {
        const QString newValue = le->text();
        model->setData(index, newValue, Qt::EditRole);
        qDebug() << "[EDITOR] setModelData: сохранено в модель:" << newValue;
    }
}

void PurchaseDelegate::updateEditorGeometry(QWidget *editor,
                                            const QStyleOptionViewItem &option,
                                            const QModelIndex &) const
{
    editor->setGeometry(option.rect);
}

void PurchaseDelegate::paint(QPainter *painter,
                             const QStyleOptionViewItem &option,
                             const QModelIndex &index) const
{
    const int gutter = 18;

    if (option.state & QStyle::State_Selected) {
        painter->save();
        painter->fillRect(option.rect, option.palette.brush(QPalette::Highlight));
        painter->restore();
    }

    QStyleOptionViewItem opt = option;
    opt.rect.adjust(gutter, 0, 0, 0);
    QStyledItemDelegate::paint(painter, opt, index);

    painter->save();
    const QRect r = option.rect;
    const int dot = 6;
    painter->setRenderHint(QPainter::Antialiasing, true);

    painter->setBrush(option.state & QStyle::State_Selected
                          ? option.palette.color(QPalette::HighlightedText)
                          : QColor("#2ecc71"));
    painter->setPen(Qt::NoPen);
    painter->drawEllipse(r.left() + (gutter - dot) / 2,
                         r.center().y() - dot / 2,
                         dot, dot);

    painter->setBrush(Qt::NoBrush);
    painter->setPen(QPen(QColor(0, 0, 0, 40), 1));
    painter->drawRect(r.adjusted(1, 1, -1, -1));

    painter->restore();

    qDebug() << "[PAINT] ячейка row=" << index.row()
             << " текст=" << index.data(Qt::DisplayRole).toString();
}
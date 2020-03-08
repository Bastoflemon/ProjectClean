#ifndef ITEMDELEGATE_H
#define ITEMDELEGATE_H

#include <QObject>
#include <QWidget>
#include <QStyledItemDelegate>
class ItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    ItemDelegate(QObject *parent=0);
    virtual ~ItemDelegate(){};
signals:
    void deleteItem(const QModelIndex &index); // 发射删除信号项


    // QAbstractItemDelegate interface
public:
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) Q_DECL_OVERRIDE;
};

#endif // ITEMDELEGATE_H

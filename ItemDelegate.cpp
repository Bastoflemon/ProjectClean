#include "ItemDelegate.h"
#include <QPainter>
#include <QMouseEvent>
#include <QApplication>
#include <QToolTip>
ItemDelegate::ItemDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{

}

void ItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItem viewOption(option);
    if (viewOption.state & QStyle::State_HasFocus)
    {
        viewOption.state = viewOption.state ^ QStyle::State_HasFocus;
    }
    QStyledItemDelegate::paint(painter,viewOption,index);
    QPixmap pixmap = QPixmap(":/Window/Images/Delete.png"); // 加载图片
    int height = (viewOption.rect.height() - 9) / 2; // 计算矩形高，中心点
    QRect decorationRect = QRect(viewOption.rect.left() + viewOption.rect.width() - 30, // x坐标位置
                                 viewOption.rect.top() + height, // y坐标位置
                                 10, // 矩形宽
                                 10);// 矩形高
    painter->drawPixmap(decorationRect,pixmap); // 在指定位置绘制图片
}

bool ItemDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    int height = (option.rect.height() - 9) / 2;
    QRect decorationRect = QRect(option.rect.left() + option.rect.width() - 30, option.rect.top() + height, 10, 10);
    QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
    if(event->type() == QEvent::MouseButtonPress && decorationRect.contains(mouseEvent->pos())) // 判断鼠标是否按下，并且处于指定区域
        emit deleteItem(index); // 发射删除信号

    // 判断鼠标是否在指定区域内
    // 1、在：改变鼠标为手型，并提示信息
    // 2、否：还原鼠标样式
    if(event->type() == QEvent::MouseMove && decorationRect.contains(mouseEvent->pos()))
    {
        QCursor cursor(Qt::PointingHandCursor); // 手型手势
        QApplication::setOverrideCursor(cursor);
        QString strText = "删除帐号信息";
        QToolTip::showText(mouseEvent->globalPos(),strText);
    }
    else
    {
        QCursor cursor(Qt::ArrowCursor);
        QApplication::setOverrideCursor(cursor);
    }
    return QStyledItemDelegate::editorEvent(event,model,option,index);
}

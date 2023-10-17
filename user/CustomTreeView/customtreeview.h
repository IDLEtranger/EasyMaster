#ifndef CUSTOMTREEVIEW_H
#define CUSTOMTREEVIEW_H

#include <QApplication>
#include <QTreeView>
#include <QStandardItemModel>
#include <QMouseEvent>

class CustomTreeView : public QTreeView
{
public:
    CustomTreeView(QWidget *parent = nullptr)
        : QTreeView(parent) {}

protected:
    void mousePressEvent(QMouseEvent *event) override
    {
        QModelIndex item = indexAt(event->pos());
        if (!item.isValid())
        {
            clearSelection();
            setCurrentIndex(QModelIndex());
        }
        else
            QTreeView::mousePressEvent(event);
    }
};

#endif // CUSTOMTREEVIEW_H

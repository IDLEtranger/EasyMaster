#ifndef STAYOPENMENU_H
#define STAYOPENMENU_H

#include <QMenu>
#include <QMouseEvent>

class StayOpenMenu : public QMenu
{
    Q_OBJECT
public:
    explicit StayOpenMenu(QWidget *parent = nullptr) : QMenu(parent) {}

protected:
    void mouseReleaseEvent(QMouseEvent *e) override
    {
        QAction *action = actionAt(e->pos()); // return curses at position action
        if (action)
        {
            action->trigger();
            return;
        }
        QMenu::mouseReleaseEvent(e);
    }
};

#endif // STAYOPENMENU_H

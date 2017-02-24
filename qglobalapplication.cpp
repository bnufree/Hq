#include "qglobalapplication.h"
#include <QKeyEvent>
#include <QDebug>
GlobalApplication::GlobalApplication(int &argc,char **argv):
QApplication(argc,argv)
{

}

GlobalApplication::~GlobalApplication()
{

}

void GlobalApplication::setWindowInstance(QWidget *wnd)
{
     widget = wnd;
}

bool GlobalApplication::notify(QObject *obj, QEvent *e)
{
    const QMetaObject* objMeta = obj->metaObject();
    QString clName = objMeta->className();

    if(e->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(e);
        if(keyEvent->key() == Qt::Key_X && keyEvent->modifiers() && Qt::AltModifier)
        {
            QDBG<< clName;
            QDBG << "ALT+X";
        }
     }

    return QApplication::notify(obj,e);
}


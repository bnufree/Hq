#ifndef QTHOOK
#define QTHOOK

/**********************************************************
  不用调用dll就可以执行的全局钩子示例:
  只留下F12做为程序退出,屏蔽键盘和鼠标,<a href="http://lib.csdn.net/base/softwaretest" class='replace_word' title="软件测试知识库" target='_blank' style='color:#df3434; font-weight:bold;'>测试</a>时请注意
  ********************************************************/
#include <QApplication>
#include <Windows.h>
#include <QWidget>
#include <QDebug>
HHOOK keyHook=NULL;
HHOOK mouseHook=NULL;
QWidget*     gWidget = NULL;
//声明卸载函数,以便调用
void unHook();
//键盘钩子过程
LRESULT CALLBACK keyProc(int nCode,WPARAM wParam,LPARAM lParam )
{


    //在WH_KEYBOARD_LL模式下lParam 是指向KBDLLHOOKSTRUCT类型地址
    KBDLLHOOKSTRUCT *pkbhs = (KBDLLHOOKSTRUCT *) lParam;
    qDebug()<<"flag>>7:"<<(pkbhs->flags>>7);
    if(pkbhs->vkCode == VK_F12 && (pkbhs->flags >> 7) == 1)
    {

        //void unHook();
        //qApp->quit();
        if(gWidget)
        {
            gWidget->setVisible(!gWidget->isVisible());
        }
    } else if(pkbhs->vkCode == VK_ESCAPE)
    {
        qApp->quit();
    }
    return 0;//返回1表示截取消息不再传递,返回0表示不作处理,消息继续传递

}
//鼠标钩子过程
LRESULT CALLBACK mouseProc(int nCode,WPARAM wParam,LPARAM lParam )
{
    return 0;

}
//卸载钩子
void unHook()
{
    UnhookWindowsHookEx(keyHook);
    UnhookWindowsHookEx(mouseHook);

}
//安装钩子,调用该函数即安装钩子
void setHook(QWidget *w=NULL)
{
    gWidget = w;
    //这两个底层钩子,不要DLL就可以全局
    //                         底层键盘钩子
    keyHook = SetWindowsHookEx( WH_KEYBOARD_LL,keyProc,GetModuleHandle(NULL),0);
    //                          底层鼠标钩子
    mouseHook = SetWindowsHookEx( WH_MOUSE_LL,mouseProc,GetModuleHandle(NULL),0);
}

#endif // QTHOOK


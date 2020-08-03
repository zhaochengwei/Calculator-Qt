#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QWidget>
#include <string.h>
#include <stdio.h>
#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MyWidget;
}
QT_END_NAMESPACE

class QErrorMessage;

class MyWidget : public QWidget
{
    Q_OBJECT

public:
    // 带参数的构造函数，创建时调用
    MyWidget(QWidget *parent = nullptr);
    // 析构函数，销毁时貂绒
    ~MyWidget();
    // 定义槽函数
private slots:
    // CE按键处理函数
    void on_pushButtonCe_clicked();
    // C按键处理函数
    void on_pushButtonC_clicked();
    // Del按键处理函数
    void on_pushButtonDe_clicked();
    // 除法按键处理函数
    void on_pushButtonChu_clicked();
    // 7 按键处理函数
    void on_pushButton7_clicked();
    // 8 按键处理函数
    void on_pushButton8_clicked();
    // 9 按键处理函数
    void on_pushButton9_clicked();
    // 乘 按键处理函数
    void on_pushButtonChen_clicked();
    // 4 按键处理函数
    void on_pushButton4_clicked();
    // 5 按键处理函数
    void on_pushButton5_clicked();
    // 6 按键处理函数
    void on_pushButton6_clicked();
    // 减法 按键处理函数
    void on_pushButtonJian_clicked();
    // 1 按键处理函数
    void on_pushButton1_clicked();
    // 2 按键处理函数
    void on_pushButton2_clicked();
    // 3 按键处理函数
    void on_pushButton3_clicked();
    // 加法 按键处理函数
    void on_pushButtonJia_clicked();
    // enter 按键处理函数
    void on_pushButtonHuan_clicked();
    // 0 按键处理函数
    void on_pushButton0_clicked();
    // 点 按键处理函数
    void on_pushButtonDian_clicked();
    // 等于 按键处理函数
    void on_pushButtonDenyu_clicked();
    // 加函数
    void AddNum(int i);
    // 展示数字 处理函数
    void ShowNum();
    // 操作符 处理函数
    void Operator(int i);
    //  处理函数
    void TwiceOperator();
    // 操作符处理函数
    double OpreatorTrue();
    // 9 数字转字符串
    void DToStr(double c);

private:
// 局部变量ui
    Ui::MyWidget *ui;
//  错误信息
    QErrorMessage *errordlg;
// 展示结果的字符串
    char showstr[100];
// 操作数变量
    double a = 0, b = 0;
    // wei表示小数点后多少位，10表示1位，100表示2位
    int cha = 0, wei = 10, showlen = 0, oper;
    // sign为true表示当前是否有运算正在进行
    // signa为 true表示当前a为正 false表示当前a为负
    // signb为 true表示当前b为正，false表示当前b为负
    // starta 表示a被占用，startb表示b被占用
    bool sign = false, signdian = false, starta = false, startb = false, signa = true, signb = true;
};
#endif // MYWIDGET_H

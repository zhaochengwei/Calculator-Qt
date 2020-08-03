#include "mywidget.h"
#include "ui_mywidget.h"
#include<QtDebug>
#include<string>
#include<QString>
#include<QErrorMessage>

const double lin = 0.0000000001;

MyWidget::MyWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MyWidget)
{
    // 初始化UI内容
    ui->setupUi(this);
    // 定义错误弹窗
    errordlg = new QErrorMessage(this);
}

MyWidget::~MyWidget()
{
    // 删除ui对象
    delete ui;
}

void MyWidget::DToStr(double c){
    // format double c in string
    sprintf(showstr, "%.8lf", c);
    // 字符串长度截断
    showstr[8] = '\0';
    // 展示有效长度设置为8
    showlen = 8;
    // 清理showstr，如果有0截断，如果有.结束
    for(int i=showlen-1; i>0; i--){
        if(showstr[i] == '0'){
            showstr[i] = '\0';
            showlen--;
        }else if(showstr[i] == '.'){
            showstr[i] = '\0';
            showlen--;
            break;
        }else{
            break;
        }
    }
}

void MyWidget::ShowNum(){
    // 打印showstr变量
    qDebug() << showstr;
    if(showlen == 0){
        // 如果展示的len为0，显示0
        ui->lcdNumber->display(0);
    }else{
        // lcdNumber显示
        ui->lcdNumber->display(showstr);
    }
}

void MyWidget::AddNum(int i){
    // 偏移长度
    showstr[showlen++] = char(48+i);
    showstr[showlen] = '\0';
    // 有点标志数长度10位，无点标志长度9位，截断
    if((signdian && showlen == 10) || (!signdian && showlen == 9)){
        errordlg->setWindowTitle(tr("错误提示"));
        errordlg->showMessage(tr("输入超限"));
        showstr[--showlen] = '\0';
        return;
    }
    // 保留传入的值
    cha = i;
    // 
    if(!sign){
        if(!starta){
            // 将数字转化为字符
            showstr[0] = char(48+i);
            // 截断
            showstr[1] = '\0';
            // 长度设为1
            showlen = 1;
            // a开始
            starta = true;
        }
        if(signdian){
            // 如果是小数点
            a = a + double(i) / wei;
            // 刷新小数点后面位数
            wei *= 10;
        }else{
            // 如果不是小数点，直接在原来基础上加个位数
            a = a * 10 + i;
        }
    }else{
        // 同上
        if(!startb){
            showstr[0] = char(48+i);
            showstr[1] = '\0';
            showlen = 1;
            startb = true;
        }
        if(signdian){
            b = b + double(i) / wei;
            wei *= 10;
        }else{
            b = b * 10 + i;
        }
    }
    // 展示数字
    ShowNum();
}

void MyWidget::on_pushButtonCe_clicked()
{
    // 数字b
    if(sign){
        TwiceOperator();
        b = 0;
        wei = 10;
        // 清除小数点标志
        signdian = false;
        startb = false;
        DToStr(b);
        ShowNum();
    }else{
        //数字a
        a = 0;
        wei = 10;
        // 清除小数点位置
        signdian = false;
        starta = false;
        DToStr(a);
        ShowNum();
    }
}

void MyWidget::on_pushButtonC_clicked()
{
    // 点击清空之后，恢复所有变量到初始状态，并展示0
    a = 0; b = 0;
    cha = 0; wei = 10; showlen=0; oper=0;
    showstr[0] = '\0';
    sign = false; signdian = false; starta = false; startb = false; signa = true; signb = true;
    DToStr(0.0);
    ShowNum();
}

void MyWidget::on_pushButtonDe_clicked()
{
    // a标志，删除a的最后一位
    if(!sign){
        if(signdian){
            a = a * wei / 10;
            a -= cha;
            a /= wei;
            wei /= 10;
            if(wei == 1){
                signdian = false;
                wei = 10;
                //showstr[--showlen] = '\0';
            }
        }else{
            a -= cha;
            a /= 10;
            cha = int(a) % 10;
        }
    }else{
        // 删除b的最后一位
        if(signdian){
            b = b * wei;
            b -= cha;
            cha = (int(b) % 100) / 10;
            b /= wei;
            wei /= 10;
            if(wei == 1){
                signdian = false;
                wei = 10;
            }
        }else{
            b -= cha;
            b /= 10;
            cha = int(b) % 10;
        }
    }
    // 截取最后一位
    if(showlen>0){
        showstr[--showlen] = '\0';
    }
    // 显示数字
    ShowNum();
}
// 执行运算操作
double MyWidget::OpreatorTrue(){
    // 执行运算
    switch (oper) {
    case 1 : return a + b;
    case 2 : return a - b;
    case 3 : return a * b;
    case 4 : {
        // 错误告警，除数不合法
        if(b>=-lin && b<=lin){
            errordlg->setWindowTitle(tr("错误提示"));
            errordlg->showMessage(tr("除数不能为0"));
            return a;
        } return a / b;
    }
    }
    return a;
}

void MyWidget::TwiceOperator(){
    //切换到a的运算上
    b = 0;
    signb = true;
    signa = true;
    starta = true;
    startb = false;
    wei = 10;
    showlen=0;
    showstr[0] = '\0';
    signdian = false;
}
// 处理传递过来的运算符
void MyWidget::Operator(int i){
    switch(i){
    case 1:{
        // 如果开始运算
        if(sign){
            // 判断a b 正负并处理
            if(!signa) a = 0 - a;
            if(!signb) b = 0 - b;
            // 计算结果赋值给a
            a = OpreatorTrue();
            // 清空上一轮操作
            TwiceOperator();
            oper = 1;
            DToStr(a);
            // 展示数字
            ShowNum();
        }else{
            // 如果还没开始运算
            // 判断 a b处理
            if(!signa) a = 0 - a;
            oper = 1;
            // 表示开始运算
            sign = true;
            // 清空上一轮的值
            TwiceOperator();
        }
    } break;
    case 2:{
        // 如果开始运算
        if(sign){
            // 如果b没有值
            if(!startb){
                // 标志b有值
                startb = true;
                // 为负数
                signb = false;
                showstr[0] = '-';
                showstr[++showlen] = '\0';
                ShowNum();
            }else{
                // b有值的情况下，根据正负赋值
                if(!signa) a = 0 - a;
                if(!signb) b = 0 - b;
                // 计算结果
                a = OpreatorTrue();
                // 清理操作
                TwiceOperator();
                oper = 2;
                DToStr(a);
                ShowNum();
            }
        }else{
            // 如果还没开始
            // 如果a没有值
            if(!starta){
                // a有值
                // a为负
                starta = true;
                signa = false;
                showstr[0] = '-';
                showstr[++showlen] = '\0';
                // 显示负号
                ShowNum();
            }else{
                // 正负赋值
                if(!signa) a = 0 - a;
                oper = 2;
                // 清理操作
                TwiceOperator();
                sign = true;
            }
        }
    } break;
    case 3:{
        if(sign){
            // 如果开始运算，正负赋值
            if(!signa) a = 0 - a;
            if(!signb) b = 0 - b;
            // 执行运算
            a = OpreatorTrue();
            // 清理操作
            TwiceOperator();
            oper = 3;
            DToStr(a);
            ShowNum();
        }else{
            // 如果还没开始
            // 正负赋值
            if(!signa) a = 0 - a;
            oper = 3;
            // 清理操作
            TwiceOperator();
            // 表示开始运算
            sign = true;
        }
    } break;
    case 4:{
        if(sign){
            if(!signa) a = 0 - a;
            if(!signb) b = 0 - b;
            a = OpreatorTrue();
            TwiceOperator();
            oper = 4;
            DToStr(a);
            ShowNum();
        }else{
            if(!signa) a = 0 - a;
            oper = 4;
            TwiceOperator();
            sign = true;
        }
    } break;
    }
}
// 正负切换标志
void MyWidget::on_pushButtonHuan_clicked()
{
    // 如果开始执行
    if(sign){
        // 如果b有值
        if(startb){
            // 如果b为正
            if(signb){
                // b置位负
                signb = false;
                // 显示数字加入-
                for(int i=(++showlen); i>0; i--){
                    showstr[i] = showstr[i-1];
                }
                // 置最左侧为负
                showstr[0] = '-';
                ShowNum();
            }else{
                // 如果为负，置位正
                signb = true;
                // 删除-号
                for(int i=0; i<showlen; i++){
                    showstr[i] = showstr[i+1];
                }
                // 长度减1
                showlen--;
                ShowNum();
            }
        }else{
            // 如果b无值，置位b有值
            startb = true;
            // 置位b负
            signb = false;
            // 加入-号
            showstr[0] = '-';
            showstr[++showlen] = '\0';
            // 显示
            ShowNum();
        }
    }else{
        // 如果还未开始执行
        // 如果a有值
        if(starta){
            // 如果a为正
            if(signa){
                // 置位a为负
                signa = false;
                // 加入-
                for(int i=(++showlen); i>0; i--){
                    showstr[i] = showstr[i-1];
                }
                showstr[0] = '-';
                ShowNum();
            }else{
                // 如果a为负
                signa = true;
                // 去除-
                for(int i=0; i<showlen; i++){
                    showstr[i] = showstr[i+1];
                }
                showlen--;
                ShowNum();
            }
        }else{
            //如果还未开始执行
            // 置位a有值
            starta = true;
            // 置位a负值
            signa = false;
            // 加入-
            showstr[0] = '-';
            showstr[++showlen] = '\0';
            ShowNum();
        }
    }
}

void MyWidget::on_pushButtonDian_clicked()
{
    // 如果是有点的标志，则置位点的标志，并加入显示字符串中显示
    if(!signdian){
        signdian = true;
        showstr[showlen++] = '.';
        showstr[showlen] = '\0';
        ShowNum();
    }
}

void MyWidget::on_pushButtonDenyu_clicked()
{
    // 如果开始执行
    if(sign){
        // 标志未执行
        sign = false;
        // 正负赋值
        if(!signa) a = 0 - a;
        if(!signb) b = 0 - b;
        // 运算显示结果
        a = OpreatorTrue();
        // 清理操作
        TwiceOperator();
        DToStr(a);
        ShowNum();
    }else{
        // 如果还未开始执行
        // 正负赋值a
        if(!signa) a = 0 - a;
        // 清理操作
        TwiceOperator();
        // 置位开始执行
        sign = true;
    }
}

void MyWidget::on_pushButton1_clicked()
{
    AddNum(1);
}

void MyWidget::on_pushButton2_clicked()
{
    AddNum(2);
}

void MyWidget::on_pushButton3_clicked()
{
    AddNum(3);
}

void MyWidget::on_pushButton4_clicked()
{
    AddNum(4);
}

void MyWidget::on_pushButton5_clicked()
{
    AddNum(5);
}

void MyWidget::on_pushButton6_clicked()
{
    AddNum(6);
}

void MyWidget::on_pushButton7_clicked()
{
    AddNum(7);
}

void MyWidget::on_pushButton8_clicked()
{
    AddNum(8);
}

void MyWidget::on_pushButton9_clicked()
{
    AddNum(9);
}

void MyWidget::on_pushButton0_clicked()
{
    // 新增0输入
    AddNum(0);
}
// 除法运算符
void MyWidget::on_pushButtonChu_clicked()
{
    Operator(4);
}
// 乘法运算符符处理
void MyWidget::on_pushButtonChen_clicked()
{
    Operator(3);
}
// 减法运算符处理
void MyWidget::on_pushButtonJian_clicked()
{
    Operator(2);
}
// 加法运算符处理
void MyWidget::on_pushButtonJia_clicked()
{
    Operator(1);
}

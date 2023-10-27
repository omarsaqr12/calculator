
#include "calculator.h"
#include "ui_calculator.h"
double CalcVal=0;
bool division=false;
bool multi=false;
bool add=false;
bool subs=false;
bool diff= false;
bool inte=false;

calculator::calculator(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::calculator)
{
    ui->setupUi(this);
    ui->res->setText(QString::number(CalcVal));
    QPushButton* buttons[10];
    for(int i=0;i<10;i++){
        QString name="b"+QString::number(i);
        buttons[i]=calculator::findChild<QPushButton*>(name);
        connect(buttons[i],SIGNAL(released()),this,SLOT(NumPressed()));
    }
    connect(ui->ad,SIGNAL(released()),this,SLOT(operbutpressed()));
    connect(ui->diffrentiate,SIGNAL(released()),this,SLOT(operbutpressed()));
    connect(ui->integ,SIGNAL(released()),this,SLOT(operbutpressed()));
    connect(ui->div,SIGNAL(released()),this,SLOT(operbutpressed()));
    connect(ui->mult,SIGNAL(released()),this,SLOT(operbutpressed()));
    connect(ui->subs,SIGNAL(released()),this,SLOT(operbutpressed()));
    connect(ui->eq,SIGNAL(released()),this,SLOT(equpressed()));









}

calculator::~calculator()
{
    delete ui;
}
void calculator::NumPressed(){
    QPushButton *button=(QPushButton *)sender();
    QString newval=button->text();
    QString oldval=ui->res->text();
    if(oldval=="0"||oldval=="0.0"){ui->res->setText(newval);}
    else{ newval=oldval+newval;
        double doubnewval=newval.toDouble();
        ui->res->setText(QString::number(doubnewval,'g',16));
    }

}
void calculator::operbutpressed(){
    division=false;
     multi=false;
     add=false;
     subs=false;
     diff= false;
    inte=false;
     QString dis=ui->res->text();
    CalcVal=dis.toDouble();
     QPushButton *button=(QPushButton*)sender();
    if(QString::compare(button->text(),"/")==0){division=true;}
    else if(QString::compare(button->text(),"*")==0){multi=true;}
    else if(QString::compare(button->text(),"+")==0){add=true;}
    else if(QString::compare(button->text(),"-")==0){subs=true;}
    else if(QString::compare(button->text(),"*")==0){multi=true;}
    else if(QString::compare(button->text(),"integrate")==0){inte=true;}
    else{diff=true;}


    ui->res->setText("");
}
void calculator::equpressed(){
    double solution;
    QString cur=ui->res->text();
    double curr=cur.toDouble();
    if(add==true){solution=curr+CalcVal;}
    else if(multi==true){solution=curr*CalcVal;}
    else if(division==true){solution=CalcVal/curr;}
    else if(subs==true){solution=CalcVal-curr;}

    ui->res->setText(QString::number(solution,'g',16));
}


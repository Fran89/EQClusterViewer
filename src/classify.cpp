#include "classify.h"
#include "ui_classify.h"

Classify::Classify(QWidget *parent, QPair<clusT,trendT> *result) :
    QDialog(parent),
    ui(new Ui::Classify)
{
    res = result;
    res->first = unk;
    res->second = none;
    ui->setupUi(this);
    ui->typebox->addItem("Unknown",unk);
    ui->typebox->addItem("Swarm",swm);
    ui->typebox->addItem("Sequence",seq);
    ui->trendbox->addItem("None",none);
    ui->trendbox->addItem("NS",ns);
    ui->trendbox->addItem("EW",ew);
    ui->trendbox->addItem("NS-EW",nsew);
    ui->trendbox->addItem("Deep",deep);
    ui->trendbox->addItem("Shallow",shal);
    this->setWindowTitle("Choose classification");
}

Classify::~Classify()
{
    delete ui;
}

void Classify::on_typebox_currentIndexChanged(int index)
{
    res->first = ui->typebox->itemData(index).value<clusT>();
}

void Classify::on_trendbox_currentIndexChanged(int index)
{
    res->second = ui->trendbox->itemData(index).value<trendT>();
}

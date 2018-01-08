#ifndef CLASSIFY_H
#define CLASSIFY_H

#include <QDialog>
#include <QPair>
#include "cluster.h"

namespace Ui {
class Classify;
}

class Classify : public QDialog
{
    Q_OBJECT

public:
    Classify(QWidget *parent = 0, QPair<clusT,trendT> *result = 0);
    ~Classify();

    QPair<clusT,trendT> * res;

private slots:
    void on_typebox_currentIndexChanged(int index);

    void on_trendbox_currentIndexChanged(int index);

private:
    Ui::Classify *ui;
};

#endif // CLASSIFY_H

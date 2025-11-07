#ifndef PREDICTIONWIDGET_H
#define PREDICTIONWIDGET_H

#include <QWidget>
#include "prediction.h"

namespace Ui {
class PredictionWidget;
}

class PredictionWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PredictionWidget(QWidget *parent = nullptr);
    ~PredictionWidget();

private slots:
    void predire();

private:
    Ui::PredictionWidget *ui;
    Prediction *prediction;
};

#endif // PREDICTIONWIDGET_H

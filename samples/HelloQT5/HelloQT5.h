#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_HelloQT5.h"

class HelloQT5 : public QMainWindow
{
    Q_OBJECT

public:
    HelloQT5(QWidget *parent = Q_NULLPTR);

private:
    Ui::HelloQT5Class ui;
};

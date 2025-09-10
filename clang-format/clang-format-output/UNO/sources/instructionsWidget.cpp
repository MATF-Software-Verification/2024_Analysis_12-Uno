#include "headers/instructionsWidget.h"
#include "headers/mainWindow.h"
#include "ui_instructionsWidget.h"

#include <QApplication>
#include <QScreen>

InstructionsWidget::InstructionsWidget(QWidget* parent)
  : QWidget(parent)
  , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowTitle("How to play");
}

InstructionsWidget::~InstructionsWidget()
{
    delete ui;
}

void
InstructionsWidget::closeEvent(QCloseEvent* event)
{
    auto* mw = new MainWindow();
    mw->adjustSize();
    mw->move(QWidget::screen()->availableGeometry().center() -
             mw->rect().center());
    mw->show();
    this->close();
}

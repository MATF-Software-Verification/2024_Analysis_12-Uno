#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class InstructionsWidget : public QWidget
{
    Q_OBJECT

  public:
    InstructionsWidget(QWidget* parent = nullptr);
    ~InstructionsWidget();
    void closeEvent(QCloseEvent* event);

  private:
    Ui::Widget* ui;
};
#endif // WIDGET_H

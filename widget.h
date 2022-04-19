#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QTimer>
#include "gpio.h"

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

public slots:
    void updateButtonState();
    void changeAction(short pin);
    void getFlank();
    void sustainAction();
private:            
    QLabel* dirlabel = new QLabel("Direction: Right");
    QList<QLabel*> m_input_display;
    QTimer* m_timer;
    QTimer* m_ledtimer;
    gpio* m_gpio;
    QList<bool> m_oldstates;
    QList<bool> m_states;
    short m_counter;
    short m_pickaction;
    bool m_onoff;
    short m_oldaction;
    short m_counteraction;
};

#endif // WIDGET_H

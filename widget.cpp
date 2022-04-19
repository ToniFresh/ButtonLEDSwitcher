#include "config.h"
#include "widget.h"
#include <QGridLayout>
#include <QDebug>


Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    // Widget s &QTimer::timeoutstructure
    setWindowTitle("ButtonLEDMonitor");
    resize(300, 100);
    QGridLayout* grid = new QGridLayout(this);
    setLayout(grid);

    // Labels for input pins / buttons AND var initalization
    m_counter = 0;
    m_pickaction = 0;
    m_onoff = 0;
    int column = 0;
    m_oldstates = {0, 0, 0};
    m_states = {0, 0, 0};
    m_counteraction = 0;
    m_oldaction = 0;

    for (auto pin : BUTTONS) {
        // pin numbers
        QLabel* label = new QLabel("GPIO " + QString::number(pin), this);
        label->setAlignment(Qt::AlignCenter);
        grid->addWidget(label, 0, column); // links oben (1. Zeile / 1. - 3. Spalte)

        // pin states
        QLabel* state = new QLabel("0", this);
        state->setAlignment(Qt::AlignCenter);
        grid->addWidget(state, 1, column++); // links oben (2. Zeile / 1. - 3. Spalte)
        m_input_display.append(state);
    }

    dirlabel->setAlignment(Qt::AlignCenter);
    grid->addWidget(dirlabel,2,1);

    // initialize hardware
    m_gpio = new gpio();

    m_timer = new QTimer(this);
    m_ledtimer = new QTimer(this);
    // Connect QTimer (Signal) with GUI function (Slot) "update display"
    connect(m_timer, &QTimer::timeout, this, &Widget::updateButtonState);
    connect(m_timer, &QTimer::timeout, this, &Widget::getFlank);
    connect(m_ledtimer, &QTimer::timeout, this, &Widget::sustainAction);
    m_timer->start(T_UPDATE);
    m_ledtimer->start(T_LEDUPDATE);
}

Widget::~Widget()
{
    for(int n = 0; n < sizeof(LEDS); n++) m_gpio->set(LEDS[n], 0);
}

void Widget::getFlank()
{
    short cnt = 0;
    for (auto pin : BUTTONS) m_states[cnt++] = !m_gpio->get(pin);

    cnt = 0;
    for (auto pin : BUTTONS){               
        if(m_oldstates[cnt] !=  m_states[cnt++]) changeAction(pin);
    }
}

void Widget::changeAction(short pin)
{
    if(pin == BUTTONS[0]) m_pickaction = 0;
    if(pin == BUTTONS[1])
    {

       if(m_counteraction % 2 == 0)
       {
           m_onoff = !m_onoff;
           if(m_counteraction == 0 && m_onoff == 1 && m_pickaction != 1) m_oldaction = m_pickaction;
           m_pickaction = 1;
           if(m_counteraction == 0 && m_onoff == 0) m_pickaction = m_oldaction;
       }
       m_counteraction++;
       if(m_counteraction > 1) m_counteraction = 0;
    }
    if(pin == BUTTONS[2]) m_pickaction = 2;

    for(int n = 0; n < 3; n++) m_oldstates[n] = m_states[n];
}

void Widget::sustainAction()
{
    bool state = 0;
    for(int n = 0; n < sizeof(LEDS); n++) m_gpio->set(LEDS[n], state);

    if(m_pickaction == 1 && m_onoff == 1) for(int n = 0; n < sizeof(LEDS); n++) m_gpio->set(LEDS[n], state);

    else if(m_pickaction == 0 && m_onoff == 0)
    {
        state = 1;
        m_gpio->set(LEDS[m_counter++], state);
        if(m_counter > 3) m_counter = 0;
        dirlabel->setText("Direction: Right!");
    }  

    else if(m_pickaction == 2 && m_onoff == 0)
    {
        state = 1;
        m_gpio->set(LEDS[m_counter--], state);
        if(m_counter < 0) m_counter = 3;
        dirlabel->setText("Direction: Left!");
    }
}

void Widget::updateButtonState()
{
    int n = 0;
    for (auto pin : BUTTONS){
        int state = !m_gpio->get(pin);
        m_input_display[n++]->setText(QString::number(state));
    }
}

#include <QImage>
#include <QColor>
#include <QPainter>
#include <QSGGeometryNode>
#include <QSGGeometry>
#include <QSGVertexColorMaterial>
#include <QThread>
#include <QString>

#include <cassert>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <string>

#include "screen.h"
#include "logging.h"
#include "gameboyinterface.h"

using std::vector;
using std::shared_ptr;
using std::unordered_map;
using std::string;

const unordered_map<int, GameBoyInterface::JoyPadButton> Screen::BUTTON_MAP = {
    { Qt::Key_Left,  GameBoyInterface::JOYPAD_LEFT   },
    { Qt::Key_Right, GameBoyInterface::JOYPAD_RIGHT  },
    { Qt::Key_Down,  GameBoyInterface::JOYPAD_DOWN   },
    { Qt::Key_Up,    GameBoyInterface::JOYPAD_UP     },
    { Qt::Key_A,     GameBoyInterface::JOYPAD_A      },
    { Qt::Key_B,     GameBoyInterface::JOYPAD_B      },
    { Qt::Key_Space, GameBoyInterface::JOYPAD_START  },
    { Qt::Key_F,     GameBoyInterface::JOYPAD_SELECT },
};

Screen::Screen(QQuickItem *parent)
    : QQuickPaintedItem(parent),
      m_width(512),
      m_height(256),
      //m_width(LCD_SCREEN_WIDTH),
      //m_height(LCD_SCREEN_HEIGHT),
      m_canvas(m_width + 5, m_height, QImage::Format_RGBA8888),
      m_console(GameBoyInterface::Instance()),
      m_stopped(false)
{
    assert(m_console);

    setFlag(ItemHasContents, true);

    QObject::connect(&m_timer, SIGNAL(timeout()), this, SLOT(onTimeout()));

    m_timer.setInterval(10);
    m_timer.start();

    QStringList args = QCoreApplication::arguments();

    string filename = (args.size() < 2) ? "" : args.at(1).toStdString();
    m_console->load(filename);
    m_console->start();
}

Screen::~Screen()
{
    stop();
}

void Screen::stop()
{
    if (m_stopped) { return; }

    m_timer.stop();

    m_console->stop();
}

void Screen::keyPressEvent(QKeyEvent *event)
{
    auto iterator = BUTTON_MAP.find(event->key());
    if (BUTTON_MAP.end() != iterator) {
        m_console->setButton(iterator->second);
    }
}

void Screen::keyReleaseEvent(QKeyEvent *event)
{
    auto iterator = BUTTON_MAP.find(event->key());
    if (BUTTON_MAP.end() != iterator) {
        m_console->clrButton(iterator->second);
    }
}

void Screen::paint(QPainter *painter)
{
    painter->drawImage(0, 0, m_canvas.scaled(width(), height()));
}

void Screen::onTimeout()
{
    static const int thickness = 5;
    
    ColorArray rgb = m_console->getRGB();
    if (rgb.empty()) { return; }
    
    for (size_t i = 0; i < rgb.size(); i++) {
        shared_ptr<GB::RGB> color = rgb.at(i);
        if (!color) { assert(0); continue; }
        
        int x = i % m_width;
        int y = i / m_width;

        if (x >= (m_width/2)) { x += thickness; }
        m_canvas.setPixel(x, y, qRgba(color->red, color->green, color->blue, color->alpha));
    }

    for (int i = 0; i < m_height; i++) {
        for (int j = 0; j < thickness; j++) {
            m_canvas.setPixel(m_width/2 + j, i, qRgba(0, 0, 0, 0xFF));
        }
    }
    update();
}

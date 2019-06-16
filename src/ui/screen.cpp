#include <QImage>
#include <QColor>
#include <QPainter>
#include <QSGGeometryNode>
#include <QSGGeometry>
#include <QSGVertexColorMaterial>

#include <vector>

#include "screen.h"

using std::vector;

Screen::Screen(QQuickItem *parent)
    : QQuickPaintedItem(parent),
      m_width(LCD_SCREEN_WIDTH),
      m_height(LCD_SCREEN_HEIGHT),
      m_canvas(m_width, m_height, QImage::Format_RGBA8888)
{
    setFlag(ItemHasContents, true);

    QObject::connect(&m_timer, SIGNAL(timeout()), this, SLOT(onTimeout()));

    m_timer.setInterval(1000);
    m_timer.start();
}

void Screen::paint(QPainter *painter)
{
    vector<GPU::RGB> rgb = m_console.getRGB();
    for (size_t i = 0; i < rgb.size(); i++) {
        const GPU::RGB & color = rgb.at(i);

        int x = i % m_width;
        int y = i / m_width;

        m_canvas.setPixel(x, y, qRgba(color.red, color.green, color.blue, color.alpha));
    }

    painter->drawImage(0, 0, m_canvas.scaled(width(), height()));
}

void Screen::onTimeout()
{
    update();
}

#if 0
QSGNode *Screen::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *)
{
    QSGGeometryNode *root = static_cast<QSGGeometryNode*>(oldNode);
    
    if (!root) {
        root = new QSGGeometryNode();
        root->setFlag(QSGNode::OwnsMaterial, true);
        root->setFlag(QSGNode::OwnsGeometry, true);
    }

    QSGGeometry *geometry = new QSGGeometry(QSGGeometry::defaultAttributes_ColoredPoint2D(), m_width * m_height);
    QSGGeometry::ColoredPoint2D *points = geometry->vertexDataAsColoredPoint2D();

    vector<GPU::RGB> rgb = m_console.getRGB();
    for (size_t i = 0; i < rgb.size(); i++) {
        const GPU::RGB & color = rgb.at(i);

        int x = i % m_width;
        int y = i / m_width;

        points[i].set(x, y, color.red, color.green, color.blue, color.alpha);
    }

    root->setGeometry(geometry);

    QSGVertexColorMaterial *material = new QSGVertexColorMaterial();
    root->setMaterial(material);
 
    return root;
}
#endif

#include "servergame.h"

ServerGame::ServerGame(PoolGame *game, ServerDisplay &display, Server &server) :
    PoolGame(*game),
    m_socket(server.m_socket), m_ip(&server.clientSet), m_pos(),
    m_clicked(false), m_indexOfBall(-1), m_states(), m_originator()
{
    connect(&display,&ServerDisplay::mousePressed,this,&ServerGame::mousePressed);
    connect(&display,&ServerDisplay::mouseMoved,this,&ServerGame::mouseMoved);
    connect(&display,&ServerDisplay::mouseReleased,this,&ServerGame::mouseReleased);
    connect(&display,&ServerDisplay::keyPressed,this,&ServerGame::keyPressed);
    connect(&display,&ServerDisplay::keyReleased,this,&ServerGame::keyReleased);
}

void ServerGame::draw(QPainter &p)
{
    PoolGame::draw(p);
    if (m_clicked)
    {
        p.drawLine(m_balls[m_indexOfBall]->position().toPointF(), m_pos.toPointF());
    }
}

void ServerGame::mousePressed(QMouseEvent *event)
{
    for (size_t i = 0; i < m_balls.size(); ++i) {
        if ((QVector2D(event->pos())-m_balls[i]->position()).length() < m_balls[i]->radius())
        {
            m_indexOfBall = i;
            break;
        }
    }

    if (m_indexOfBall > -1)
    {
        m_clicked = true;
        m_pos = QVector2D(event->localPos());
    }
}

void ServerGame::mouseMoved(QMouseEvent *event)
{
    if (m_clicked)
    {
        m_pos = QVector2D(event->localPos());
    }
}

void ServerGame::mouseReleased(QMouseEvent *event)
{
    if (m_clicked)
    {
        m_clicked = false;
        m_pos = QVector2D(event->localPos());
        //memento stuff
        Ball *b = m_balls[m_indexOfBall];
        b->setVelocity(1.5*(b->position()-m_pos));
        m_indexOfBall = -1;
    }
}

void ServerGame::keyPressed(QKeyEvent *event)
{
    if (event->key() == Qt::Key_R)
    {
        //memento stuff
    }
}

void ServerGame::keyReleased(QKeyEvent *event)
{

}

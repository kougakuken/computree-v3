#include "dm_actionshandler.h"

#include "dm_guimanager.h"
#include <QEvent>

DM_ActionsHandler::DM_ActionsHandler(const CDM_ActionsManager &actionManager)
{
    m_maxActions = 100;
    m_activeAction = NULL;
    m_defaultAction = NULL;
    m_actionManager = (CDM_ActionsManager*)&actionManager;
}

DM_ActionsHandler::~DM_ActionsHandler()
{
    while(!m_actions.isEmpty())
    {
        ActionInterface *ac = m_actions.takeFirst();

        if(!GUI_MANAGER->getActionsManager()->existActionCompareAddress(ac))
            delete ac;
    }

    if(!GUI_MANAGER->getActionsManager()->existActionCompareAddress(m_defaultAction))
        delete m_defaultAction;
}

CDM_ActionsManager* DM_ActionsHandler::actionManager() const
{
    return m_actionManager;
}

void DM_ActionsHandler::setMaxActions(const int &maxActions)
{
    m_maxActions = maxActions;
}

void DM_ActionsHandler::setCurrentAction(ActionInterface *action)
{
    if(action != NULL)
        actionManager()->addAction(action);

    ActionInterface *backup = m_activeAction;
    ActionInterface *actionFound = (action != NULL ? findAction(action->uniqueName()) : NULL);

    if((m_activeAction != NULL) && (m_activeAction != actionFound))
    {
        if(!m_activeAction->isSuspended())
            m_activeAction->suspend();

        m_activeAction->hideOptions();
        m_activeAction = NULL;
    }

    if(m_activeAction == NULL)
    {
        m_activeAction = actionFound;

        if(m_activeAction != NULL)
        {
            if(!GUI_MANAGER->getActionsManager()->existActionCompareAddress(action))
                delete action;

            m_actions.removeOne(m_activeAction);
            m_actions.append(m_activeAction);

            if(m_activeAction->isSuspended())
                m_activeAction->resume();

            m_activeAction->showOptions();
        }
        else if(action != NULL)
        {
            m_activeAction = action;

            m_activeAction->init();
            m_activeAction->trigger();

            if(m_activeAction->isFinished())
            {
                if(!GUI_MANAGER->getActionsManager()->existActionCompareAddress(m_activeAction))
                    delete m_activeAction;

                m_activeAction = NULL;

                if(!m_actions.isEmpty())
                {
                    m_activeAction = m_actions.last();
                    m_activeAction->resume();
                }
            }
            else
            {
                m_actions.append(m_activeAction);

                if(m_actions.size() > maxActions())
                {
                    ActionInterface *ac = m_actions.takeFirst();
                    ac->finish();

                    if(!GUI_MANAGER->getActionsManager()->existActionCompareAddress(ac))
                        delete ac;
                }
            }

            m_activeAction->showOptions();
        }
    }

    if(backup != m_activeAction)
        emit currentActionChanged(m_activeAction);
}

ActionInterface* DM_ActionsHandler::currentAction() const
{
    return m_activeAction;
}

void DM_ActionsHandler::removeActions(const QString &uniqueName)
{
    bool currentActionDeleted = false;

    QMutableListIterator<ActionInterface*> it(m_actions);

    while(it.hasNext())
    {
        ActionInterface *ac = it.next();

        if(ac->uniqueName() == uniqueName)
        {
            it.remove();

            if(ac == m_activeAction)
            {
                currentActionDeleted = true;
            }
            else
            {
                ac->finish();

                if(!GUI_MANAGER->getActionsManager()->existActionCompareAddress(ac))
                    delete ac;
            }
        }
    }

    if(currentActionDeleted)
    {
        ActionInterface *ac = m_activeAction;
        m_activeAction = NULL;

        ac->finish();

        if(!GUI_MANAGER->getActionsManager()->existActionCompareAddress(ac))
            delete ac;

        setCurrentAction(NULL);
        emit currentActionChanged(NULL);
    }

    if((m_defaultAction != NULL)
            && (m_defaultAction->uniqueName() == uniqueName))
    {
        m_defaultAction->finish();

        if(!GUI_MANAGER->getActionsManager()->existActionCompareAddress(m_defaultAction))
            delete m_defaultAction;

        m_defaultAction = NULL;

        emit defaultActionChanged(NULL);
    }
}

void DM_ActionsHandler::setDefaultAction(ActionInterface *action)
{
    if(m_defaultAction != NULL)
    {
        m_defaultAction->finish();

        if(!GUI_MANAGER->getActionsManager()->existActionCompareAddress(m_defaultAction))
            delete m_defaultAction;
    }

    m_defaultAction = action;

    if(m_defaultAction != NULL)
    {
        m_defaultAction->init();
        m_defaultAction->trigger();

        if(m_defaultAction->isFinished())
        {
            if(!GUI_MANAGER->getActionsManager()->existActionCompareAddress(m_defaultAction))
                delete m_defaultAction;

            m_defaultAction = NULL;
        }
    }

    emit defaultActionChanged(m_defaultAction);
}

ActionInterface *DM_ActionsHandler::defaultAction() const
{
    return m_defaultAction;
}

int DM_ActionsHandler::maxActions() const
{
    return m_maxActions;
}

bool DM_ActionsHandler::eventToAction(QEvent *e)
{
    bool val = false;
    bool resume = e->type() == QEvent::WindowActivate;

    if(m_activeAction != NULL)
    {
        if(resume && m_activeAction->isSuspended())
            m_activeAction->resume();

        val = m_activeAction->event(e);
    }

    if((m_defaultAction != NULL)
            && resume && m_defaultAction->isSuspended())
         m_defaultAction->resume();


    if(!val && (m_defaultAction != NULL))
        val = m_defaultAction->event(e);

    return val;
}

bool DM_ActionsHandler::enterEvent(QEvent *e)
{
    Q_UNUSED(e)

    if((m_activeAction != NULL) && m_activeAction->isSuspended())
        m_activeAction->resume();

    if((m_defaultAction != NULL) && m_defaultAction->isSuspended())
        m_defaultAction->resume();

    return false;
}

bool DM_ActionsHandler::leaveEvent(QEvent *e)
{
    Q_UNUSED(e)

    if(m_activeAction != NULL)
        m_activeAction->suspend();

    if(m_defaultAction != NULL)
        m_defaultAction->suspend();

    return false;
}

bool DM_ActionsHandler::mousePressEvent(QMouseEvent *e)
{
    return callEventStaticCastT<ActionInterface, QMouseEvent*>(&ActionInterface::mousePressEvent, e);
}

bool DM_ActionsHandler::mouseMoveEvent(QMouseEvent *e)
{
    return callEventStaticCastT<ActionInterface, QMouseEvent*>(&ActionInterface::mouseMoveEvent, e);
}

bool DM_ActionsHandler::mouseReleaseEvent(QMouseEvent *e)
{
    return callEventStaticCastT<ActionInterface, QMouseEvent*>(&ActionInterface::mouseReleaseEvent, e);
}

bool DM_ActionsHandler::mouseDoubleClickEvent(QMouseEvent *e)
{
    return callEventStaticCastT<ActionInterface, QMouseEvent*>(&ActionInterface::mouseDoubleClickEvent, e);
}

bool DM_ActionsHandler::wheelEvent(QWheelEvent *e)
{
    return callEventStaticCastT<ActionInterface, QWheelEvent*>(&ActionInterface::wheelEvent, e);
}

bool DM_ActionsHandler::keyPressEvent(QKeyEvent *e)
{
    return callEventStaticCastT<ActionInterface, QKeyEvent*>(&ActionInterface::keyPressEvent, e);
}

bool DM_ActionsHandler::keyReleaseEvent(QKeyEvent *e)
{
    return callEventStaticCastT<ActionInterface, QKeyEvent*>(&ActionInterface::keyReleaseEvent, e);
}

ActionInterface* DM_ActionsHandler::findAction(const QString &uniqueName) const
{
    QListIterator<ActionInterface*> it(m_actions);

    while(it.hasNext())
    {
        ActionInterface *action = it.next();

        if(action->uniqueName() == uniqueName)
            return action;
    }

    return NULL;
}

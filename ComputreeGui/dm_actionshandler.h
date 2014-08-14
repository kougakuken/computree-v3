#ifndef DM_ACTIONSHANDLER_H
#define DM_ACTIONSHANDLER_H

#include "interfaces.h"
#include "cdm_actionsmanager.h"

class DM_ActionsHandler : public QObject
{
    Q_OBJECT

public:
    DM_ActionsHandler(const CDM_ActionsManager &actionManager);
    ~DM_ActionsHandler();

    /**
     * @brief Return the action manager
     */
    CDM_ActionsManager* actionManager() const;

    /**
     * @brief Return the maximum number of actions to backup
     */
    int maxActions() const;

    /**
     * @brief Set max actions to backup
     */
    void setMaxActions(const int &maxActions);

    /**
     * @brief Set the current action that receives event
     *
     *        The handler is responsible of the life of the action.
     */
    void setCurrentAction(ActionInterface *action);

    /**
     * @brief Return the current action
     */
    ActionInterface* currentAction() const;

    /**
     * @brief Remove actions that have the uniqueName passed in parameter. The default
     *        action is also concerned.
     */
    void removeActions(const QString &uniqueName);

    /**
     * @brief Set the default action. The old action is deleted.
     */
    void setDefaultAction(ActionInterface *action);

    /**
     * @brief Return the default action
     */
    ActionInterface* defaultAction() const;

    bool eventToAction(QEvent *e);
    bool enterEvent(QEvent *e);
    bool leaveEvent(QEvent *e);
    bool mousePressEvent(QMouseEvent *e);
    bool mouseMoveEvent(QMouseEvent *e);
    bool mouseReleaseEvent(QMouseEvent *e);
    bool mouseDoubleClickEvent(QMouseEvent *e);
    bool wheelEvent(QWheelEvent *e);
    bool keyPressEvent(QKeyEvent *e);
    bool keyReleaseEvent(QKeyEvent *e);

private:
    QList<ActionInterface*>     m_actions;
    int                         m_maxActions;
    ActionInterface             *m_activeAction;
    ActionInterface             *m_defaultAction;
    CDM_ActionsManager          *m_actionManager;

    ActionInterface* findAction(const QString &uniqueName) const;

protected:

    template<typename ACTION, typename PARAMETER>
    bool callEventDynamicCastT(bool (ACTION::*func)(PARAMETER param), PARAMETER p)
    {
        bool val = false;

        if(m_activeAction != NULL)
            val = (dynamic_cast<ACTION*>(m_activeAction)->*func)(p);

        if(!val && (m_defaultAction != NULL))
            val = (dynamic_cast<ACTION*>(m_defaultAction)->*func)(p);

        return val;
    }

    template<typename ACTION, typename PARAMETER>
    bool callEventStaticCastT(bool (ACTION::*func)(PARAMETER param), PARAMETER p)
    {
        bool val = false;

        if(m_activeAction != NULL)
            val = (((ACTION*)m_activeAction)->*func)(p);

        if(!val && (m_defaultAction != NULL))
            val = (((ACTION*)m_defaultAction)->*func)(p);

        return val;
    }

signals:
    void currentActionChanged(ActionInterface *action);
    void defaultActionChanged(ActionInterface *acrion);
};

#endif // DM_ACTIONSHANDLER_H

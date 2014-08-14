#ifndef DM_ITEMDRAWABLETREEVIEWCONTROLLER_H
#define DM_ITEMDRAWABLETREEVIEWCONTROLLER_H

#include <QStandardItemModel>
#include <QTimer>

#include "interfaces.h"

#include "tools/treeview/dm_iitemdrawablestandarditembuilder.h"
#include "tools/treeview/dm_itreeviewmanager.h"

/**
 * @brief Class that control the build/manage of a QStandardItemModel that must contains ItemDrawable. When you finish to add
 *        ItemDrawable the controller request the gui manager a exclusive asynchronous operation to perform the creation of
 *        the list of QStandardItem in a thread. When the thread finish all QStandardItem will be added to the model.
 *
 *        You can call construcModel() to (re)construct completely the model. The controller request the list of
 *        ItemDrawable to add to the model at the DM_ITreeViewManager with the help of the method itemDrawableForTreeView().
 *
 *        When you finish to remove ItemDrawable the controller reconstruct the model if the number of ItemDrawable to remove
 *        is greater than the return of the method maxRemoveToDoInGuiThread().
 */
class DM_ItemDrawableTreeViewController : public QObject
{
    Q_OBJECT

public:
    DM_ItemDrawableTreeViewController(QObject *parent = 0);

    /**
     * @brief Set the model of the TreeView
     */
    void setModel(const QStandardItemModel *model);

    /**
     * @brief Set the QStandardItem builder
     */
    void setStandardItemBuilder(const DM_IItemDrawableStandardItemBuilder *builder);

    /**
     * @brief Set the view that control the QTreeView
     */
    void setTreeViewManager(const DM_ITreeViewManager *manager);

    /**
     * @brief If the number of ItemDrawable to remove is greater than this parameter the controller recontruct the model instead
     *        remove in gui thread. (default : 100)
     */
    void setMaxRemoveToDoInGuiThread(const int &n);

    /**
     * @brief Return the number of ItemDrawable to remove in gui thread.
     */
    int maxRemoveToDoInGuiThread() const;

    /**
     * @brief Call this methods when the controller must add one or many ItemDrawable to the TreeView. You can call this method
     *        in a thread.
     */
    void beginAddMultipleItemDrawable();
    void addItemDrawable(ItemDrawable &item);
    void endAddMultipleItemDrawable();

    /**
     * @brief Call this methods when the controller must remove one or many ItemDrawable from the TreeView. You can call this method
     *        in a thread.
     */
    void beginRemoveMultipleItemDrawable();
    void removeItemDrawable(ItemDrawable &item);
    void endRemoveMultipleItemDrawable();

private:

    QStandardItemModel                      *m_model;

    QVector< QList<QStandardItem*> >        m_collection;
    QVector< QList<QStandardItem*> >        m_collectionUpdate;

    bool                                    m_addMultiple;
    QList<ItemDrawable*>                    m_tmpItemsToAdd;
    QTimer                                  m_timerAddToView;

    bool                                    m_removeMultiple;
    QList<ItemDrawable*>                    m_tmpItemsToRemove;
    QTimer                                  m_timerRemoveFromView;

    QList<QPair<QStandardItem *, ItemDrawable *> >  m_tmpItemsToUpdate;

    int                                     m_maxRemoveInGuiThread;

    DM_IItemDrawableStandardItemBuilder     *m_standardItemBuilder;
    DM_ITreeViewManager                     *m_treeViewManager;

    /**
     * @brief Called from slotModelBuilderFinished and slotModelBuilderAddFinished
     */
    void addElementsOfCollection();

    /**
     * @brief Called from slotModelBuilderUpdateFinished
     */
    void updateElemensOfCollection();

public slots:

    /**
     * @brief Call to (re)construct all the model
     */
    void constructModel();

    /**
     * @brief Call to refresh children of this QStandardItem
     * @param item : the item to update
     * @param itemDrawable : the ItemDrawable represented by the QStandardItem
     */
    void refresh(const QList<QPair<QStandardItem *, ItemDrawable *> > &list);

private slots:

    /**
     * @brief Called when the model builder is finished (called in the gui thread)
     */
    void slotModelBuilderFinished(bool canceled);

    /**
     * @brief Called when the model builder has finish to update (called in the gui thread)
     */
    void slotModelBuilderUpdateFinished(bool canceled);

    /**
     * @brief Called when the model builder is finished (called in the gui thread)
     */
    void slotModelBuilderAddFinished(bool canceled);

    /**
     * @brief Called when the model builder is finished (called in the gui thread)
     */
    void slotModelBuilderRemoveFinished(bool canceled);

    /**
     * @brief Called when the timer time out and when we must add ItemDrawable of the
     *        m_tmpItemsToAdd list to the treeview
     */
    void slotAddTemporaryItemsInTable();

    /**
     * @brief Called when the timer time out and when we must remove ItemDrawable of the
     *        m_tmpItemsToRemove list from the treeview
     */
    void slotRemoveTemporaryItemsInTable();

signals:
    void mustStartAddTimer();
    void mustStartRemoveTimer();
    void updated();
};

#endif // DM_ITEMDRAWABLETREEVIEWCONTROLLER_H

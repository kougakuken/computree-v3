#ifndef CT_ABSTRACTGLOBALCLOUDMANAGERT_H
#define CT_ABSTRACTGLOBALCLOUDMANAGERT_H

#include "ct_cloudindex/tools/abstract/ct_abstractcloudindexregistrationmanagert.h"
#include "ct_cloud/tools/abstract/ct_abstractcloudsynctoglobalcloudmanagert.h"

/**
 * Utility class who manage a global cloud of T (point, face, etc...). It will inform the CT_AbstractCloudIndexRegistrationManagerT and all
 * CT_AbstractCloudSyncToGlobalCloudManagerT when elements of the cloud is deleted or added. So we can sync index and
 * other cloud with this global cloud.
 */
template<typename T>
class CT_AbstractGlobalCloudManagerT
{
public:

    typedef typename CT_AbstractCloudIndexRegistrationManagerT<T>::CT_AbstractModifiableCIR         CT_AbstractModifiableCIR;
    typedef typename CT_AbstractCloudIndexRegistrationManagerT<T>::CT_AbstractNotModifiableCIR      CT_AbstractNotModifiableCIR;
    typedef typename CT_AbstractCloudIndexRegistrationManagerT<T>::CT_AbstractCIR                   CT_AbstractCIR;

    enum IndexOptimization {
        MemoryOptimized = 0,
        AccessSpeedOptimized
    };

    CT_AbstractGlobalCloudManagerT();
    virtual ~CT_AbstractGlobalCloudManagerT() {}

    /**
     * @brief Add listener of global cloud change
     */
    void addGlobalCloudListener(const IGlobalCloudListener *listener);

    /**
     * @brief globalCloud
     * @return Retourne le nuage global (il contient tout les nuages créés)
     */
    virtual CT_AbstractCloudT<T>* globalAbstractCloud() const = 0;

protected:

    QList< IGlobalCloudListener* >                                  m_listener;
    QList< CT_AbstractCloudIndexRegisteredT<T>* >                   m_cirArray;
    QList< CT_AbstractCloudIndexRegisteredT<T>* >                   m_cirArrayUnsync;

    void informThatCloudAdded(const size_t &size);
    void informThatCloudDeleted(const size_t &begin, const size_t &size);
    void shiftAllCloudIndexFrom(const int &from, const size_t &offset, const bool &negativeOffset);

    void lockAllDocuments();
    void unlockAllDocuments();

    template<typename CI>
    std::vector< typename CI::size_type >* getInternalDataOfIndex(CI *index) const;

    template<typename CIR, typename CI>
    CIR* createNewIndexRegistered(CI *index) const;

    friend class CT_Mesh;

    // called by CT_Mesh (and must only be called by CT_Mesh)
    void setEnableSyncForCIR(CT_AbstractCloudIndexRegisteredT<T> *cir, bool enable);

    // called by inherited class to know index to ignore to sync
    bool nextIndexToIgnoreToSync(const size_t &currentIndex, size_t &beginIndex, size_t &endIndex);
};

#include "ct_cloud/tools/abstract/ct_abstractglobalcloudmanagert.hpp"

#endif // CT_ABSTRACTGLOBALCLOUDMANAGERT_H

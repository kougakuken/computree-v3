#ifndef CT_MESHALLOCATORT_HPP
#define CT_MESHALLOCATORT_HPP

#include "ct_mesh/tools/ct_meshallocatort.h"

#include "ct_global/ct_context.h"

#include "ct_cloudindex/registered/abstract/ct_abstractnotmodifiablecloudindexregisteredt.h"
#include "ct_pointcloudindex/ct_pointcloudindexvector.h"
#include "ct_mesh/cloud/ct_edgecloudindexvector.h"
#include "ct_mesh/cloud/ct_facecloudindexvector.h"

#include "ct_cloud/tools/ct_globalpointcloudmanager.h"

template<typename Mesh>
typename CT_MeshAllocatorT<Mesh>::VertexIndexIterator CT_MeshAllocatorT<Mesh>::AddVerticeIndexes(Mesh &mesh, const size_t &n)
{
    CT_PointCloudIndexVector *v = (CT_PointCloudIndexVector*)mesh.m_pVert;
    size_t lastSize = 0;

    if(v == NULL)
    {
        v = new CT_PointCloudIndexVector(n);
        mesh.m_vert = PS_REPOSITORY->registerCloudIndex<CT_Point>(v);
        mesh.m_pVert = v;
    }
    else
    {
        lastSize = v->size();
        v->resize(lastSize + n);
    }

    return mesh.m_vert->begin()+lastSize;
}

template<typename Mesh>
typename CT_MeshAllocatorT<Mesh>::FaceIndexIterator CT_MeshAllocatorT<Mesh>::AddFaceIndexes(Mesh &mesh, const size_t &n)
{
    CT_FaceCloudIndexVector *f = (CT_FaceCloudIndexVector*)mesh.m_pFace;
    size_t lastSize = 0;

    if(f == NULL)
    {
        f = new CT_FaceCloudIndexVector(n);
        mesh.m_face = PS_REPOSITORY->registerCloudIndex<CT_Face>(f);
        mesh.m_pFace = f;
    }
    else
    {
        f->resize(f->size() + n);
    }

    return mesh.m_face->begin()+lastSize;
}

template<typename Mesh>
typename CT_MeshAllocatorT<Mesh>::HEdgeIndexIterator CT_MeshAllocatorT<Mesh>::AddHEdgeIndexes(Mesh &mesh, const size_t &n)
{
    CT_EdgeCloudIndexVector *e = (CT_EdgeCloudIndexVector*)mesh.m_pHedge;
    size_t lastSize = 0;

    if(e == NULL)
    {
        e = new CT_EdgeCloudIndexVector(n);
        mesh.m_hedge = PS_REPOSITORY->registerCloudIndex<CT_Edge>(e);
        mesh.m_pHedge = e;
    }
    else
    {
        e->resize(e->size() + n);
    }

    return mesh.m_hedge->begin()+lastSize;
}

template<typename Mesh>
typename CT_MeshAllocatorT<Mesh>::VertexIndexIterator CT_MeshAllocatorT<Mesh>::AddVertices(Mesh &mesh, const size_t &n)
{
    CT_PointCloudIndexVector *v = ((CT_PointCloudIndexVector*)mesh.m_pVert);

    typename CT_MeshAllocatorT<Mesh>::VertexIndexIterator it = addT<CT_Point, CT_PointCloudIndexVector, VertexIndexIterator>(n,
                                                                    mesh.m_newVert,
                                                                    mesh.m_vert,
                                                                    &v);
    mesh.m_pVert = v;

    return it;
}

template<typename Mesh>
typename CT_MeshAllocatorT<Mesh>::FaceIndexIterator CT_MeshAllocatorT<Mesh>::AddFaces(Mesh &mesh, const size_t &n)
{
    CT_FaceCloudIndexVector *f = ((CT_FaceCloudIndexVector*)mesh.m_pFace);

    typename CT_MeshAllocatorT<Mesh>::FaceIndexIterator  it = addT<CT_Face, CT_FaceCloudIndexVector, FaceIndexIterator>(n,
                                                                mesh.m_newFace,
                                                                mesh.m_face,
                                                                &f);
    mesh.m_pFace = f;

    return it;
}

template<typename Mesh>
typename CT_MeshAllocatorT<Mesh>::HEdgeIndexIterator CT_MeshAllocatorT<Mesh>::AddHEdges(Mesh &mesh, const size_t &n)
{
    CT_EdgeCloudIndexVector *e = ((CT_EdgeCloudIndexVector*)mesh.m_pHedge);

    typename CT_MeshAllocatorT<Mesh>::HEdgeIndexIterator it = addT<CT_Edge, CT_EdgeCloudIndexVector, HEdgeIndexIterator>(n,
                                                                 mesh.m_newEdge,
                                                                 mesh.m_hedge,
                                                                 &e);
    mesh.m_pHedge = e;

    return it;
}

template<typename Mesh>
template<typename T, typename CloudIndex, typename Iterator>
Iterator CT_MeshAllocatorT<Mesh>::addT(const size_t &n,
                                       std::vector< QSharedPointer< CT_AbstractNotModifiableCloudIndexRegisteredT<T> > > &meshCIRCollection,
                                       typename CT_AbstractCloudIndexRegistrationManagerT<T>::CT_AbstractModifiableCIR &meshCir,
                                       CloudIndex **meshCI)
{
    QSharedPointer< CT_AbstractNotModifiableCloudIndexRegisteredT<T> > cir;

    // first index in the global cloud for the new cloud
    typename CloudIndex::size_type firstPIndex = 0;

    // if the mesh has already a cloud created by the cloud manager
    if(!meshCIRCollection.empty())
    {
        QSharedPointer< CT_AbstractNotModifiableCloudIndexRegisteredT<T> > tmpCir = meshCIRCollection[meshCIRCollection.size()-1];

        size_t s = tmpCir->size();
        firstPIndex = tmpCir->last()+1;

        // resize last point cloud index added to mesh if it was at the end of the global points cloud (resize is the same time the global points cloud)
        cir = PS_REPOSITORY->resizeCloudIndexAndGlobalCloud<T>(tmpCir, s+n);
    }

    // if the mesh don't have a points cloud or if the last created can't be resized
    if(cir.data() == NULL)
    {
        // create a new points cloud
        cir = PS_REPOSITORY->createNewCloud<T>(n);

        firstPIndex = cir->first();

        // add them to the collection
        meshCIRCollection.push_back(cir);
    }

    // if the mesh don't have a cloud index we create one
    if(meshCir.data() == NULL)
    {
        *meshCI = new CloudIndex();
        meshCir = PS_REPOSITORY->registerCloudIndex<T>((CloudIndex*)(*meshCI));
    }

    CloudIndex *index = (CloudIndex*)(*meshCI);

    // begin of the first new index
    size_t begin = index->size();

    // resize the index
    index->resize(index->size() + n);

    // end of the index
    size_t s = index->size();

    typename CloudIndex::size_type a = firstPIndex;

    for(size_t i=begin; i<s; ++i)
    {
        index->replaceIndex(i, a, false);
        ++a;
    }

    Iterator iterator = index->begin() + begin;
    return iterator;
}

#endif // CT_MESHALLOCATORT_HPP
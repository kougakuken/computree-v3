/****************************************************************************

 Copyright (C) 2010-2012 the Office National des Forêts (ONF), France
                     and the Association de Recherche Technologie et Sciences (ARTS), Ecole Nationale Suprieure d'Arts et Métiers (ENSAM), Cluny, France.
                     All rights reserved.

 Contact : alexandre.piboule@onf.fr

 Developers : Michal KREBS (ARTS/ENSAM)

 This file is part of PluginShared library 2.0.

 PluginShared is free library: you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 PluginShared is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with PluginShared.  If not, see <http://www.gnu.org/licenses/lgpl.html>.

*****************************************************************************/

#include "ct_abstractitemdrawable.h"
#include "ct_abstractresult.h"
#include "ct_outabstractitemmodel.h"
#include "ct_virtualabstractstep.h"
#include "ct_global/ct_context.h"
#include "ct_model/tools/ct_modelsearchhelper.h"

#include <math.h>

quint64 CT_AbstractItemDrawable::NEXTID = 1;
QMap<QString, QString> CT_AbstractItemDrawable::NAMEMAP;
QMutex CT_AbstractItemDrawable::NAMEMAP_Mutex;

CT_AbstractItemDrawable::CT_AbstractItemDrawable() : CT_AbstractItem()
{
    _id = 0;
    _selected = false;
    _displayed = false;
    _centerCoordinate(0) = 0;
    _centerCoordinate(1) = 0;
    _centerCoordinate(2) = 0;
    _autoDelete = true;
    _parent = NULL;
    _baseDrawManager = NULL;
    _alternativeDrawManager = NULL;
}

CT_AbstractItemDrawable::CT_AbstractItemDrawable(const CT_OutAbstractItemModel *model, const CT_AbstractResult *result) : CT_AbstractItem(model, result)
{
    _id = NEXTID++;
    _selected = false;
    _displayed = false;
    _centerCoordinate(0) = 0;
    _centerCoordinate(1) = 0;
    _centerCoordinate(2) = 0;
    _autoDelete = true;
    _parent = NULL;
    _baseDrawManager = NULL;
    _alternativeDrawManager = NULL;
}

CT_AbstractItemDrawable::CT_AbstractItemDrawable(const QString &modelUniqueName, const CT_AbstractResult *result) : CT_AbstractItem(modelUniqueName, result)
{
    _id = NEXTID++;
    _selected = false;
    _displayed = false;
    _centerCoordinate(0) = 0;
    _centerCoordinate(1) = 0;
    _centerCoordinate(2) = 0;
    _autoDelete = true;
    _parent = NULL;
    _baseDrawManager = NULL;
    _alternativeDrawManager = NULL;
}

CT_AbstractItemDrawable::~CT_AbstractItemDrawable()
{
    while(!_documentList.isEmpty())
        _documentList.takeFirst()->removeItemDrawable(*this);
}

void CT_AbstractItemDrawable::setId(quint64 id)
{
    _id = id;
}

QString CT_AbstractItemDrawable::internalVerifyModel(const CT_OutAbstractModel *model) const
{
    if(dynamic_cast<const CT_OutAbstractItemModel*>(model) == NULL)
        return tr("Model passed in parameter is not a CT_OutAbstractItemModel");

    return QString();
}

void CT_AbstractItemDrawable::setDisplayableName(const QString &displayableName)
{
    _name = displayableName;
}

void CT_AbstractItemDrawable::setSelected(bool value)
{
    if(_selected != value)
    {
        _selected = value;

        emit selectChange(_selected, this);
    }
}

void CT_AbstractItemDrawable::setDisplayed(bool value)
{
    if(_displayed != value)
    {
        if(value)
        {
            if(result() != NULL)
                result()->incrementItemVisible();
        }
        else
        {
            if(result() != NULL)
                result()->decrementItemVisible();
        }

        _displayed = value;

        emit displayChange(_displayed, this);
    }
}

void CT_AbstractItemDrawable::changeResult(const CT_AbstractResult *newRes)
{
    internalSetResult(newRes);
}

QList<DocumentInterface*> CT_AbstractItemDrawable::document() const
{
    return _documentList;
}

bool CT_AbstractItemDrawable::addDocumentParent(DocumentInterface *doc)
{
    if((doc != NULL)
            && !_documentList.contains(doc))
    {
        _documentList.append(doc);

        setDisplayed(true);

        if(abstractModel() != NULL)
            abstractModel()->incrementVisibleInDocument(doc);

        return true;
    }

    return false;
}

void CT_AbstractItemDrawable::removeDocumentParent(DocumentInterface *doc)
{
    int index;

    if((doc != NULL)
        && ((index = _documentList.indexOf(doc)) >= 0))
    {
        _documentList.removeAt(index);

        if(_documentList.isEmpty())
        {
            setDisplayed(false);
            setSelected(false);
        }

        if(abstractModel() != NULL)
            abstractModel()->decrementVisibleInDocument(doc);
    }
}

QString CT_AbstractItemDrawable::name() const
{
    return metaObject()->className();
}

QString CT_AbstractItemDrawable::staticName()
{
    return staticMetaObject.className();
}

QString CT_AbstractItemDrawable::displayableName() const
{
    if(!_name.isEmpty())
        return _name;

    if(abstractModel() != NULL)
        return QString("%1_%2").arg(abstractModel()->displayableName()).arg(QVariant(id()).toString());

    return QString("%1_%2").arg(name()).arg(QVariant(id()).toString());
}

QString CT_AbstractItemDrawable::staticGetType()
{
    return "ItemDrawable/CT_AbstractItemDrawable";

}

void CT_AbstractItemDrawable::setAutoDelete(bool autoDelete)
{
    _autoDelete = autoDelete;
}

CT_OutAbstractItemModel* CT_AbstractItemDrawable::model() const
{
    return (CT_OutAbstractItemModel*)abstractModel();
}

CT_AbstractItemDrawable* CT_AbstractItemDrawable::itemParent() const
{
    return _parent;
}

void CT_AbstractItemDrawable::setAlternativeDrawManager(const CT_AbstractItemDrawableDrawManager *drawManager)
{
    _alternativeDrawManager = (CT_AbstractItemDrawableDrawManager*)drawManager;

    if((_alternativeDrawManager != NULL) && (_alternativeDrawManager->getDrawConfiguration() == NULL))
        _alternativeDrawManager->initDrawConfiguration();
}

const CT_AbstractItemDrawableDrawManager* CT_AbstractItemDrawable::getAlternativeDrawManager() const
{
    return _alternativeDrawManager;
}

const CT_AbstractItemDrawableDrawManager* CT_AbstractItemDrawable::getBaseDrawManager() const
{
    return _baseDrawManager;
}

void CT_AbstractItemDrawable::setItemDrawableParent(const CT_AbstractItemDrawable *parent)
{
    _parent = (CT_AbstractItemDrawable*)parent;
}

void CT_AbstractItemDrawable::setBaseDrawManager(const CT_AbstractItemDrawableDrawManager *drawManager)
{
    _baseDrawManager = (CT_AbstractItemDrawableDrawManager*)drawManager;

    if((_baseDrawManager != NULL) && (_baseDrawManager->getDrawConfiguration() == NULL))
        _baseDrawManager->initDrawConfiguration();
}

bool CT_AbstractItemDrawable::isSelected() const
{
    return _selected;
}

bool CT_AbstractItemDrawable::isDisplayed() const
{
    return _displayed;
}

void CT_AbstractItemDrawable::setCenterX(double x)
{
    _centerCoordinate(0) = x;
}

void CT_AbstractItemDrawable::setCenterY(double y)
{
    _centerCoordinate(1) = y;
}

void CT_AbstractItemDrawable::setCenterZ(double z)
{
    _centerCoordinate(2) = z;
}

double CT_AbstractItemDrawable::getCenterX() const
{
    return _centerCoordinate(0);
}

double CT_AbstractItemDrawable::getCenterY() const
{
    return _centerCoordinate(1);
}

double CT_AbstractItemDrawable::getCenterZ() const
{
    return _centerCoordinate(2);
}

void CT_AbstractItemDrawable::getBoundingBox(Eigen::Vector3d &min, Eigen::Vector3d &max) const
{
    min(0) = 0;
    min(1) = 0;
    min(2) = 0;

    max(0) = 0;
    max(1) = 0;
    max(2) = 0;
}

QString CT_AbstractItemDrawable::getInfo()
{
    return QString("%1 %2").arg(id()).arg(name());
}

void CT_AbstractItemDrawable::draw(GraphicsViewInterface &view, PainterInterface &painter)
{
    if(_alternativeDrawManager != NULL)
    {
        _alternativeDrawManager->preDraw(view, painter, *this);
        _alternativeDrawManager->draw(view, painter, *this);
        _alternativeDrawManager->postDraw(view, painter, *this);
    }
    else if(_baseDrawManager != NULL)
    {
        _baseDrawManager->preDraw(view, painter, *this);
        _baseDrawManager->draw(view, painter, *this);
        _baseDrawManager->postDraw(view, painter, *this);
    }
}

CT_ItemDrawableConfiguration* CT_AbstractItemDrawable::getViewConfiguration()
{
    if(_alternativeDrawManager != NULL)
        return (CT_ItemDrawableConfiguration*)_alternativeDrawManager->getDrawConfiguration();

    if(_baseDrawManager != NULL)
        return (CT_ItemDrawableConfiguration*)_baseDrawManager->getDrawConfiguration();

    return NULL;
}

QList<CT_ItemDrawableConfiguration*> CT_AbstractItemDrawable::getDependantViewConfigurations()
{
    return QList<CT_ItemDrawableConfiguration*>();
}

void CT_AbstractItemDrawable::addNameTypeCorresp(QString type, QString name)
{
    QMutexLocker locker(&NAMEMAP_Mutex);

    NAMEMAP.insert(type, name);
}

QString CT_AbstractItemDrawable::getNameFromType(QString type)
{
    QMutexLocker locker(&NAMEMAP_Mutex);

    return NAMEMAP.value(type);
}

bool CT_AbstractItemDrawable::isAutoDelete() const
{
    return _autoDelete;
}

void CT_AbstractItemDrawable::setModel(const CT_OutAbstractItemModel *model)
{
    internalSetModel(model);
}

void CT_AbstractItemDrawable::setModel(const QString &modelUniqueName)
{
    CT_AbstractResult *result = this->result();
    Q_ASSERT_X(!modelUniqueName.isEmpty(), "CT_AbstractItem constructor", "When you create a Element the modelName must not be empty !");
    Q_ASSERT_X((result != NULL), "CT_AbstractItem constructor", "When you create a Element with a modelName the result must not be NULL !");
    Q_ASSERT_X((result->parentStep() != NULL), "CT_AbstractItem constructor", "When you create a Element with a modelName the result must know its parent step");

    CT_OutAbstractModel *model = PS_MODELS->searchModelForCreation(modelUniqueName, result);

    Q_ASSERT_X(model != NULL, "CT_AbstractItem constructor", "You created a CT_AbstractItem with a modelName but the model was not found");

    internalSetModel(model);
}

#ifdef USE_BOOST_OLD
BOOST_CLASS_EXPORT_IMPLEMENT(CT_AbstractItemDrawable)
#endif

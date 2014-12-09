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

#include "ct_abstractshape2d.h"
#include "ct_global/ct_context.h"


CT_AbstractShape2D::CT_AbstractShape2D() : CT_AbstractItemDrawableWithoutPointCloud()
{
    _data = NULL;
}

CT_AbstractShape2D::CT_AbstractShape2D(const CT_OutAbstractSingularItemModel *model,
                                   const CT_AbstractResult *result,
                                   CT_Shape2DData *data) : CT_AbstractItemDrawableWithoutPointCloud(model, result)
{
    _data = data;
    if (_data != NULL)
    {
        QVector2D min, max;
        data->getBoundingBox(min, max);

        _minCoordinates.setX(min.x());
        _minCoordinates.setY(min.y());
        _minCoordinates.setZ(0);

        _maxCoordinates.setX(max.x());
        _maxCoordinates.setY(max.y());
        _maxCoordinates.setZ(0);
    }
}

CT_AbstractShape2D::CT_AbstractShape2D(const QString &modelName,
                                   const CT_AbstractResult *result,
                                   CT_Shape2DData *data) : CT_AbstractItemDrawableWithoutPointCloud(modelName, result)
{
    _data = data;
    if (_data != NULL)
    {
        QVector2D min, max;
        data->getBoundingBox(min, max);

        _minCoordinates.setX(min.x());
        _minCoordinates.setY(min.y());
        _minCoordinates.setZ(0);

        _maxCoordinates.setX(max.x());
        _maxCoordinates.setY(max.y());
        _maxCoordinates.setZ(0);
    }
}

CT_AbstractShape2D::~CT_AbstractShape2D()
{
    delete _data;
}

QString CT_AbstractShape2D::getType() const
{
    return staticGetType();
}

QString CT_AbstractShape2D::staticGetType()
{
    return CT_AbstractItemDrawableWithoutPointCloud::staticGetType() + "/CT_AbstractShape2D";
}

void CT_AbstractShape2D::setCenterX(float x)
{
    getDataNotConst()->_center.setX(x);
}

void CT_AbstractShape2D::setCenterY(float y)
{
    getDataNotConst()->_center.setY(y);
}

float CT_AbstractShape2D::getCenterX() const
{
    return getDataNotConst()->getCenter().x();
}

float CT_AbstractShape2D::getCenterY() const
{
    return getDataNotConst()->getCenter().y();
}

const CT_Shape2DData* CT_AbstractShape2D::getPointerData() const
{
    return _data;
}

const CT_Shape2DData& CT_AbstractShape2D::getData() const
{
    return *_data;
}

const QVector2D &CT_AbstractShape2D::getCenter() const
{
    return _data->getCenter();
}

void CT_AbstractShape2D::getBoundingBox(QVector3D &min, QVector3D &max) const
{
    CT_AbstractItemDrawableWithoutPointCloud::getBoundingBox(min, max);
    float zplane = CT_Context::staticInstance()->getZPlaneFor2DShapes();

    min.setZ(zplane);
    max.setZ(zplane);
}

CT_Shape2DData* CT_AbstractShape2D::getDataNotConst() const
{
    return _data;
}
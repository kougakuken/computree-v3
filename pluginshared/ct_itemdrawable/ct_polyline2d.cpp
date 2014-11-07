/****************************************************************************

 Copyright (C) 2010-2012 the Office National des Forêts (ONF), France
                     and the Association de Recherche Technologie et Sciences (ARTS), Ecole Nationale Suprieure d'Arts et Métiers (ENSAM), Cluny, France.
                     All rights reserved.

 Contact : alexandre.piboule@onf.fr

 Developers : Alexandre PIBOULE (ONF)

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

#include "ct_polyline2d.h"

#include <limits>

CT_DEFAULT_IA_INIT(CT_Polyline2D)

const CT_StandardPolyline2DDrawManager CT_Polyline2D::POLYLINE2D_DRAW_MANAGER;

CT_Polyline2D::CT_Polyline2D() : CT_AbstractShape2D()
{
    setBaseDrawManager(&POLYLINE2D_DRAW_MANAGER);
}

CT_Polyline2D::CT_Polyline2D(const CT_OutAbstractSingularItemModel *model,
                         const CT_AbstractResult *result,
                         CT_Polyline2DData *data) : CT_AbstractShape2D(model, result, data)
{
    setBaseDrawManager(&POLYLINE2D_DRAW_MANAGER);

    if (data != NULL)
    {
        _minCoordinates.setX(data->_minX);
        _minCoordinates.setY(data->_minY);

        _maxCoordinates.setX(data->_maxX);
        _maxCoordinates.setY(data->_maxY);
    } else {
        _minCoordinates.setX(0);
        _minCoordinates.setY(0);

        _maxCoordinates.setX(0);
        _maxCoordinates.setY(0);
    }
    _minCoordinates.setZ(0);
    _maxCoordinates.setZ(0);
}

CT_Polyline2D::CT_Polyline2D(const QString &modelName,
                         const CT_AbstractResult *result,
                         CT_Polyline2DData *data) : CT_AbstractShape2D(modelName, result, data)
{
    setBaseDrawManager(&POLYLINE2D_DRAW_MANAGER);

    if (data != NULL)
    {
        _minCoordinates.setX(data->_minX);
        _minCoordinates.setY(data->_minY);

        _maxCoordinates.setX(data->_maxX);
        _maxCoordinates.setY(data->_maxY);
    } else {
        _minCoordinates.setX(0);
        _minCoordinates.setY(0);

        _maxCoordinates.setX(0);
        _maxCoordinates.setY(0);
    }
    _minCoordinates.setZ(0);
    _maxCoordinates.setZ(0);
}

const QVector<QVector2D*>& CT_Polyline2D::getVertices() const
{
    return ((const CT_Polyline2DData&)getData()).getVertices();
}

QString CT_Polyline2D::getType() const
{
    return staticGetType();
}

QString CT_Polyline2D::staticGetType()
{
    return CT_AbstractShape2D::staticGetType() + "/CT_Polyline2D";
}

CT_AbstractItemDrawable* CT_Polyline2D::copy(const CT_OutAbstractItemModel *model,
                                           const CT_AbstractResult *result,
                                           CT_ResultCopyModeList copyModeList)
{
    Q_UNUSED(copyModeList);
    CT_Polyline2D *polygon = new CT_Polyline2D((const CT_OutAbstractSingularItemModel *)model, result, (getPointerData() != NULL) ? ((const CT_Polyline2DData&)getData()).clone() : NULL);
    polygon->setId(id());

    polygon->setAlternativeDrawManager(getAlternativeDrawManager());

    return polygon;
}


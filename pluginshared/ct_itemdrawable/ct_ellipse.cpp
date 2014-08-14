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

#include "ct_ellipse.h"

#include "ct_standarditemdataref.h"
#include "ct_tools/itemdrawable/ct_idvdcreator.h"

CT_INIT_DEFAULT_IA(0, CT_Ellipse)
CT_INIT_DEFAULT_IA(1, CT_Ellipse)
CT_INIT_DEFAULT_IA(2, CT_Ellipse)
CT_INIT_DEFAULT_IA(3, CT_Ellipse)
CT_INIT_DEFAULT_IA(4, CT_Ellipse)
CT_INIT_DEFAULT_IA(5, CT_Ellipse)
CT_INIT_DEFAULT_IA(6, CT_Ellipse)
CT_INIT_DEFAULT_IA(7, CT_Ellipse)
CT_INIT_DEFAULT_IA(8, CT_Ellipse)
CT_INIT_DEFAULT_IA(9, CT_Ellipse)
CT_INIT_DEFAULT_IA(10, CT_Ellipse)
CT_INIT_DEFAULT_IA(11, CT_Ellipse)
CT_INIT_DEFAULT_IA(12, CT_Ellipse)
CT_INIT_DEFAULT_IA(13, CT_Ellipse)
CT_INIT_DEFAULT_IA(14, CT_Ellipse)
CT_INIT_DEFAULT_IA(15, CT_Ellipse)
CT_INIT_DEFAULT_IA(16, CT_Ellipse)
CT_INIT_DEFAULT_IA(17, CT_Ellipse)
const CT_StandardEllipseDrawManager CT_Ellipse::ELLIPSE_DRAW_MANAGER;

CT_Ellipse::CT_Ellipse(): CT_AbstractShape()
{
    setBaseDrawManager(&ELLIPSE_DRAW_MANAGER);
}

CT_Ellipse::CT_Ellipse(const CT_OutAbstractItemModel *model,
                       const CT_AbstractResult *result,
                       CT_EllipseData *data) : CT_AbstractShape(model, result, data)
{
    setBaseDrawManager(&ELLIPSE_DRAW_MANAGER);
}

CT_Ellipse::CT_Ellipse(const QString &modelName,
                       const CT_AbstractResult *result,
                       CT_EllipseData *data) : CT_AbstractShape(modelName, result, data)
{
    setBaseDrawManager(&ELLIPSE_DRAW_MANAGER);
}

QString CT_Ellipse::getType() const
{
    return staticGetType();
}

QString CT_Ellipse::staticGetType()
{
    return CT_AbstractShape::staticGetType() + "/CT_Ellipse";
}

const CT_LineData& CT_Ellipse::getAxisA() const
{
    return ((const CT_EllipseData&)getData()).getAxisA();
}

const CT_LineData& CT_Ellipse::getAxisB() const
{
    return ((const CT_EllipseData&)getData()).getAxisB();
}

float CT_Ellipse::getError() const
{
    return ((const CT_EllipseData&)getData()).getError();
}

double CT_Ellipse::getAxisRatio() const
{
    return getAxisA().length()/getAxisB().length();
}

double CT_Ellipse::getAxisAP1X() const
{
    return getAxisA().getP1().x();
}

double CT_Ellipse::getAxisAP1Y() const
{
    return getAxisA().getP1().y();
}

double CT_Ellipse::getAxisAP1Z() const
{
    return getAxisA().getP1().z();
}

double CT_Ellipse::getAxisAP2X() const
{
    return getAxisA().getP2().x();
}

double CT_Ellipse::getAxisAP2Y() const
{
    return getAxisA().getP2().y();
}

double CT_Ellipse::getAxisAP2Z() const
{
    return getAxisA().getP2().z();
}

double CT_Ellipse::getAxisALength() const
{
    return getAxisA().length();
}

double CT_Ellipse::getAxisBP1X() const
{
    return getAxisB().getP1().x();
}

double CT_Ellipse::getAxisBP1Y() const
{
    return getAxisB().getP1().y();
}

double CT_Ellipse::getAxisBP1Z() const
{
    return getAxisB().getP1().z();
}

double CT_Ellipse::getAxisBP2X() const
{
    return getAxisB().getP2().x();
}

double CT_Ellipse::getAxisBP2Y() const
{
    return getAxisB().getP2().y();
}

double CT_Ellipse::getAxisBP2Z() const
{
    return getAxisB().getP2().z();
}

double CT_Ellipse::getAxisBLength() const
{
    return getAxisB().length();
}

CT_AbstractItemDrawable* CT_Ellipse::copy(const CT_OutAbstractItemModel *model,
                                          const CT_AbstractResult *result,
                                          CT_ResultCopyModeList copyModeList)
{
    Q_UNUSED(copyModeList);
    CT_Ellipse *el = new CT_Ellipse(model, result, (getPointerData() != NULL) ? ((const CT_EllipseData&)getData()).clone() : NULL);
    el->setId(id());
    el->setAlternativeDrawManager(getAlternativeDrawManager());

    return el;
}

CT_Ellipse* CT_Ellipse::staticCreateZAxisAlignedEllipseFromPointCloud(const CT_OutAbstractItemModel *model,
                                                                      quint64 id,
                                                                      const CT_AbstractResult *result,
                                                                      const CT_AbstractPointCloud *pointCloud,
                                                                      const CT_AbstractPointCloudIndex *pointCloudIndex)
{
    Q_UNUSED(id)

    CT_EllipseData *data = CT_EllipseData::staticCreateZAxisAlignedEllipseDataFromPointCloud(pointCloud, pointCloudIndex);

    if(data == NULL)
        return NULL;

    return new CT_Ellipse(model, result, data);
}

#ifdef USE_BOOST_OLD
BOOST_CLASS_EXPORT_IMPLEMENT(CT_EllipseData)
BOOST_CLASS_EXPORT_IMPLEMENT(CT_Ellipse)
#endif

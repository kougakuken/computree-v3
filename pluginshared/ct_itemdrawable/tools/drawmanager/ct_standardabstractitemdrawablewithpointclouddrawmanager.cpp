#include "ct_standardabstractitemdrawablewithpointclouddrawmanager.h"

#include "ct_abstractitemdrawablewithpointcloud.h"

#include "ct_pointcloudindex/abstract/ct_abstractpointcloudindex.h"

const QString CT_StandardAbstractItemDrawableWithPointCloudDrawManager::INDEX_CONFIG_POINTS_VISIBLE = CT_StandardAbstractItemDrawableWithPointCloudDrawManager::staticInitConfigPointsVisible();
const QString CT_StandardAbstractItemDrawableWithPointCloudDrawManager::INDEX_CONFIG_BOUNDING_SHAPE_VISIBLE = CT_StandardAbstractItemDrawableWithPointCloudDrawManager::staticInitConfigBoundingShapeVisible();
const QString CT_StandardAbstractItemDrawableWithPointCloudDrawManager::INDEX_CONFIG_BOUNDING_SHAPE_POINT_SIZE = CT_StandardAbstractItemDrawableWithPointCloudDrawManager::staticInitConfigBoundingShapePointSize();
const QString CT_StandardAbstractItemDrawableWithPointCloudDrawManager::INDEX_CONFIG_BOUNDING_SHAPE_CENTER_POINT_VISIBLE = CT_StandardAbstractItemDrawableWithPointCloudDrawManager::staticInitConfigBoundingShapeCenterPointVisible();

CT_StandardAbstractItemDrawableWithPointCloudDrawManager::CT_StandardAbstractItemDrawableWithPointCloudDrawManager(QString drawConfigurationName) : CT_AbstractItemDrawableDrawManager(drawConfigurationName.isEmpty() ? "CT_AbstractItemDrawableWithPointCloud" : drawConfigurationName)
{
    
}

CT_StandardAbstractItemDrawableWithPointCloudDrawManager::~CT_StandardAbstractItemDrawableWithPointCloudDrawManager()
{
}

void CT_StandardAbstractItemDrawableWithPointCloudDrawManager::draw(GraphicsViewInterface &view, PainterInterface &painter, const CT_AbstractItemDrawable &itemDrawable) const
{
    const CT_AbstractItemDrawableWithPointCloud &item = dynamic_cast<const CT_AbstractItemDrawableWithPointCloud&>(itemDrawable);

    if(item.getPointCloudIndex() == NULL)
        return;

    if(getDrawConfiguration()->getVariableValue(INDEX_CONFIG_POINTS_VISIBLE).toBool())
    {
        painter.drawPointCloud( PS_REPOSITORY->globalPointCloud(),
                                item.getPointCloudIndex(),
                                item.getFastestIncrement());
    }

    if(getDrawConfiguration()->getVariableValue(INDEX_CONFIG_BOUNDING_SHAPE_VISIBLE).toBool())
    {
        painter.setPointSize(getDrawConfiguration()->getVariableValue(INDEX_CONFIG_BOUNDING_SHAPE_POINT_SIZE).toDouble());

        painter.enableSetPointSize(false);
        painter.enableSetColor(false);

        painter.setColor(Qt::yellow);
        painter.setPointSize(10);
        painter.drawPoint(item.minX(), item.minY(), item.minZ());
        painter.drawPoint(item.maxX(), item.minY(), item.minZ());
        painter.drawPoint(item.minX(), item.maxY(), item.minZ());
        painter.drawPoint(item.minX(), item.minY(), item.maxZ());
        painter.drawPoint(item.maxX(), item.maxY(), item.minZ());
        painter.drawPoint(item.minX(), item.maxY(), item.maxZ());
        painter.drawPoint(item.maxX(), item.minY(), item.maxZ());
        painter.drawPoint(item.maxX(), item.maxY(), item.maxZ());
        painter.restoreDefaultPointSize();

        // B
        painter.drawLine(item.minX(), item.minY(), item.minZ(), item.maxX(), item.minY(), item.minZ());
        painter.drawLine(item.maxX(), item.minY(), item.minZ(), item.maxX(), item.maxY(), item.minZ());
        painter.drawLine(item.maxX(), item.maxY(), item.minZ(), item.minX(), item.maxY(), item.minZ());
        painter.drawLine(item.minX(), item.maxY(), item.minZ(), item.minX(), item.minY(), item.minZ());

        // H
        painter.drawLine(item.minX(), item.minY(), item.maxZ(), item.maxX(), item.minY(), item.maxZ());
        painter.drawLine(item.maxX(), item.minY(), item.maxZ(), item.maxX(), item.maxY(), item.maxZ());
        painter.drawLine(item.maxX(), item.maxY(), item.maxZ(), item.minX(), item.maxY(), item.maxZ());
        painter.drawLine(item.minX(), item.maxY(), item.maxZ(), item.minX(), item.minY(), item.maxZ());

        // G
        painter.drawLine(item.minX(), item.minY(), item.minZ(), item.minX(), item.minY(), item.maxZ());
        painter.drawLine(item.minX(), item.minY(), item.maxZ(), item.minX(), item.maxY(), item.maxZ());
        painter.drawLine(item.minX(), item.maxY(), item.maxZ(), item.minX(), item.maxY(), item.minZ());
        painter.drawLine(item.minX(), item.maxY(), item.minZ(), item.minX(), item.minY(), item.minZ());

        // D
        painter.drawLine(item.maxX(), item.minY(), item.minZ(), item.maxX(), item.minY(), item.maxZ());
        painter.drawLine(item.maxX(), item.minY(), item.maxZ(), item.maxX(), item.maxY(), item.maxZ());
        painter.drawLine(item.maxX(), item.maxY(), item.maxZ(), item.maxX(), item.maxY(), item.minZ());
        painter.drawLine(item.maxX(), item.maxY(), item.minZ(), item.maxX(), item.minY(), item.minZ());


        painter.enableSetColor(true);
        painter.enableSetPointSize(true);

        painter.restoreDefaultPointSize();
    }

    if(getDrawConfiguration()->getVariableValue(INDEX_CONFIG_BOUNDING_SHAPE_CENTER_POINT_VISIBLE).toBool())
    {
        painter.setPointSize(getDrawConfiguration()->getVariableValue(INDEX_CONFIG_BOUNDING_SHAPE_POINT_SIZE).toDouble());

        painter.drawPoint(item.getCenterX(),
                          item.getCenterY(),
                          item.getCenterZ());

        painter.restoreDefaultPointSize();
    }
}

CT_ItemDrawableConfiguration CT_StandardAbstractItemDrawableWithPointCloudDrawManager::createDrawConfiguration(QString drawConfigurationName) const
{
    CT_ItemDrawableConfiguration item = CT_ItemDrawableConfiguration(drawConfigurationName);

    item.addAllConfigurationOf(CT_AbstractItemDrawableDrawManager::createDrawConfiguration(drawConfigurationName));
    item.addNewConfiguration(CT_StandardAbstractItemDrawableWithPointCloudDrawManager::staticInitConfigPointsVisible(),"Points", CT_ItemDrawableConfiguration::Bool, true);
    item.addNewConfiguration(CT_StandardAbstractItemDrawableWithPointCloudDrawManager::staticInitConfigBoundingShapeVisible(),"Bounding Shape", CT_ItemDrawableConfiguration::Bool, false);
    item.addNewConfiguration(CT_StandardAbstractItemDrawableWithPointCloudDrawManager::staticInitConfigBoundingShapeCenterPointVisible(), "Centre de la Bounding Shape", CT_ItemDrawableConfiguration::Bool, false);
    item.addNewConfiguration(CT_StandardAbstractItemDrawableWithPointCloudDrawManager::staticInitConfigBoundingShapePointSize(), "Taille des points (Bounding Shape)", CT_ItemDrawableConfiguration::Double, 10.0);

    return item;
}

// PROTECTED //

QString CT_StandardAbstractItemDrawableWithPointCloudDrawManager::staticInitConfigPointsVisible()
{
    return "WPC_PV";
}

QString CT_StandardAbstractItemDrawableWithPointCloudDrawManager::staticInitConfigBoundingShapeVisible()
{
    return "WPC_BSV";
}

QString CT_StandardAbstractItemDrawableWithPointCloudDrawManager::staticInitConfigBoundingShapePointSize()
{
    return "WPC_BSPS";
}

QString CT_StandardAbstractItemDrawableWithPointCloudDrawManager::staticInitConfigBoundingShapeCenterPointVisible()
{
    return "WPC_BSCPS";
}
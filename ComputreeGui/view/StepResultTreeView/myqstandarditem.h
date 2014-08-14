/****************************************************************************

 Copyright (C) 2010-2012 the Office National des Forêts (ONF), France
                     and the Association de Recherche Technologie et Sciences (ARTS), Ecole Nationale Suprieure d'Arts et Métiers (ENSAM), Cluny, France.
                     All rights reserved.

 Contact : alexandre.piboule@onf.fr

 Developers : MichaÃ«l KREBS (ARTS/ENSAM)

 This file is part of Computree version 2.0.

 Computree is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 Computree is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with Computree.  If not, see <http://www.gnu.org/licenses/>.

*****************************************************************************/


#ifndef MYQSTANDARDITEM_H
#define MYQSTANDARDITEM_H

#include <QObject>
#include <QStandardItem>

class Step;
class Result;
class ItemDrawable;

/**
  * Classe permettant de grer les tapes et les rsultats dans le modele de l'arbre
  */
class MyQStandardItem : public QObject, public QStandardItem
{
    Q_OBJECT

public:


    /**
      * Type de colonne que reprsente cette item
      */
    enum ColumnType {
       StepName,                // Nom de l'tape
       StepProgress,            // Progression de l'tape
       StepElapsedTime,         // Temps coul de l'tape
       StepDebug,               // Debug pour l'tape
       ResultName,              // Nom du rsultat
       ResultProgress,          // Progression du dchargement du rsultat de la mmoire
       ResultVisibility,        // Visibilit du rsultat
       ResultEmpty,             // Case vide pour un rsultat
       ItemDrawableId,          // Id de l'itemdrawable
       ItemDrawableName,        // Nom de l'itemdrawable
       ItemDrawableVisibility,  // Visibilit de l'itemdrawable
       ItemDrawableSelection,   // Slection de l'itemdrawable
       ItemDrawableColor,       // Couleur de l'itemdrawable
       ItemDrawableZCoodinate,  // Coordonne en Z de l'itemdrawable
       ItemDrawableNotEditableData
    };

    enum MyItemDataRole {
        ColumnTypeRole = Qt::UserRole + 1,
        InvisibleBoolTypeRole = ColumnTypeRole + 1
    };

    MyQStandardItem(Step *step,
                    Result *res,
                    MyQStandardItem::ColumnType type,
                    const QString &text);

    MyQStandardItem(ItemDrawable *id,
                    MyQStandardItem::ColumnType type,
                    const QString &text);

    MyQStandardItem(Step *step,
                    Result *res,
                    MyQStandardItem::ColumnType type,
                    const QVariant &value);

    MyQStandardItem(ItemDrawable *id,
                    MyQStandardItem::ColumnType type,
                    const QVariant &value);

    Step* step() { return _step; }
    Result* result() { return _result; }
    void removeResult() { _result = NULL; }
    ItemDrawable* itemDrawable(){ return _itemDrawable; }
    void removeItemDrawable() { _itemDrawable = NULL; }

    MyQStandardItem::ColumnType columnType() const { return _columnType; }

    void setData(const QVariant &value, int role);
    void setDataWithoutSignal(const QVariant &value, int role);

private:

    Step            *_step;
    Result          *_result;
    ItemDrawable    *_itemDrawable;

    ColumnType      _columnType;

public slots:

    /**
      * Des slots qui permettent d'tre connects au signaux d'tape ou de rsultat
      */
    void setStringData(QString data, Qt::ItemDataRole role = Qt::DisplayRole);
    void setIntData(int data, Qt::ItemDataRole role = Qt::DisplayRole);
    void setFloatData(float data, Qt::ItemDataRole role = Qt::DisplayRole);
    void setBoolData(bool data, Qt::ItemDataRole role = Qt::CheckStateRole);
    void setBoolDataInvisible(bool data);
    void setColorData(QColor data, Qt::ItemDataRole role = Qt::TextColorRole);
    void slotSetEnabled(bool enable);
    void slotSetDisabled(bool disable);
    void setBoolDataToFalseWhenDataIsTrue(bool data, Qt::ItemDataRole role = Qt::CheckStateRole);

private slots:

    void stepDestroyed();
    void itemDrawableDestroyed();
    void resultDestroyed();

signals:

    void dataChanged(QStandardItem *item);
    void checkStateChanged(bool checked);
    void itemDrawableToBeDestroyed();
};

#endif // MYQSTANDARDITEM_H

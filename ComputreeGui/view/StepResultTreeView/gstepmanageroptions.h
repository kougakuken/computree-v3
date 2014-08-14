/****************************************************************************

 Copyright (C) 2010-2012 the Office National des Forêts (ONF), France
                     and the Association de Recherche Technologie et Sciences (ARTS), Ecole Nationale Supérieure d'Arts et Métiers (ENSAM), Cluny, France.
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


#ifndef GSTEPMANAGEROPTIONS_H
#define GSTEPMANAGEROPTIONS_H

#include "cdm_stepmanageroptions.h"

#include <QWidget>

namespace Ui {
    class GStepManagerOptions;
}

class GStepManagerOptions : public QWidget
{
    Q_OBJECT

public:
    explicit GStepManagerOptions(QWidget *parent = 0);
    ~GStepManagerOptions();

    void setOptions(CDM_StepManagerOptions options);
    CDM_StepManagerOptions getOptions();

private:
    Ui::GStepManagerOptions     *ui;
    CDM_StepManagerOptions      _options;

    //bool checkAutoSaveDirectory(QString path);

private slots:

    void on_pushButtonAutoSave_clicked();
    void on_cb_locale_currentIndexChanged(const QString &arg1);
signals:
    void localeChanged(QString locale);
};

#endif // GSTEPMANAGEROPTIONS_H

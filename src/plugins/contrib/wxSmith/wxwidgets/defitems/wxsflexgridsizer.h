/** \file wxsflexgridsizer.h
*
* This file is part of wxSmith plugin for Em::Blocks Studio
* Copyright (C) 2006-2007  Bartlomiej Swiecki
*
* wxSmith is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 3 of the License, or
* (at your option) any later version.
*
* wxSmith is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with wxSmith. If not, see <http://www.gnu.org/licenses/>.
*
* $Revision: 4 $
* $Id: wxsflexgridsizer.h 4 2013-11-02 15:53:52Z gerard $
* $HeadURL: svn://svn.berlios.de/codeblocks/trunk/src/plugins/contrib/wxSmith/wxwidgets/defitems/wxsflexgridsizer.h $
*/

#ifndef WXSFLEXGRIDSIZER_H
#define WXSFLEXGRIDSIZER_H

#include "../wxssizer.h"

class wxsFlexGridSizer: public wxsSizer
{
    public:

        wxsFlexGridSizer(wxsItemResData* Data);

    private:

        virtual wxSizer* OnBuildSizerPreview(wxWindow* Parent);
        virtual void OnBuildSizerCreatingCode();
        virtual void OnEnumSizerProperties(long Flags);
        virtual bool OnIsPointer() { return true; }

        long Cols;
        long Rows;
        wxsDimensionData VGap;
        wxsDimensionData HGap;
        wxString GrowableRows;
        wxString GrowableCols;
};

#endif
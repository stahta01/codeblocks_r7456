/*
 * This file is part of the Em::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 */

#ifndef ASTYLECONFIGDLG_H
#define ASTYLECONFIGDLG_H

#include <wx/intl.h>
#include "astylepredefinedstyles.h"
#include "configurationpanel.h"

class AstyleConfigDlg : public cbConfigurationPanel
{
	public:
		AstyleConfigDlg(wxWindow* parent);
		virtual ~AstyleConfigDlg();

	protected:
        void OnStyleChange(wxCommandEvent& event);
        void OnPreview(wxCommandEvent& event);

        virtual wxString GetTitle() const { return _("Source formatter"); }
        virtual wxBitmap GetBitmap() { return wxXmlResource::Get()->LoadBitmap(_T("bmp_astyle"));}
        virtual void OnApply(){ SaveSettings(); }
        virtual void OnCancel(){}

        void LoadSettings();
        void SaveSettings();
        void SetStyle(AStylePredefinedStyle style);

	private:
        DECLARE_EVENT_TABLE()
};

#endif // ASTYLECONFIGDLG_H

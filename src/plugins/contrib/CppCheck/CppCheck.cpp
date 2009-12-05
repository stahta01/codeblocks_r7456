/***************************************************************
 * Name:      CppCheck.cpp
 * Purpose:   Code::Blocks CppCheck plugin: main functions
 * Author:    Lieven de Cock (aka killerbot)
 * Created:   12/11/2009
 * Copyright: (c) Lieven de Cock (aka killerbot)
 * License:   GPL
  **************************************************************/
#include "sdk.h"
#ifndef CB_PRECOMP
#include <wx/arrstr.h>
#include <wx/dir.h>
#include <wx/file.h>
#include <wx/fs_zip.h>
#include <wx/intl.h>
#include <wx/menu.h>
#include <wx/string.h>
#include <wx/xrc/xmlres.h>
#include "cbproject.h"
#include "cbplugin.h"
#include "manager.h"
#include "logmanager.h"
#include "pluginmanager.h"
#include "projectmanager.h"
#endif
#include <wx/filefn.h>
#include "tinyxml/tinyxml.h"
#include "loggers.h"
#include "CppCheck.h"
#include "CppCheckListLog.h"

// Register the plugin
namespace
{
    PluginRegistrant<CppCheck> reg(_T("CppCheck"));
};

CppCheck::CppCheck()
{
    if(!Manager::LoadResource(_T("CppCheck.zip")))
    {
        NotifyMissingFile(_T("CppCheck.zip"));
    }
    m_LogPageIndex = 0; // good init value ???
    m_CppCheckLog = 0;
    m_ListLog = 0;
    m_ListLogPageIndex = 0;
} // end of constructor

CppCheck::~CppCheck()
{
} // end of destruccor

void CppCheck::OnAttach()
{
	// do whatever initialization you need for your plugin
	// NOTE: after this function, the inherited member variable
	// IsAttached() will be TRUE...
	// You should check for it in other functions, because if it
	// is FALSE, it means that the application did *not* "load"
	// (see: does not need) this plugin...
	if(LogManager* LogMan = Manager::Get()->GetLogManager())
	{
		m_CppCheckLog = new TextCtrlLogger();
		m_LogPageIndex = LogMan->SetLog(m_CppCheckLog);
		LogMan->Slot(m_LogPageIndex).title = _("CppCheck");
		CodeBlocksLogEvent evtAdd1(cbEVT_ADD_LOG_WINDOW, m_CppCheckLog, LogMan->Slot(m_LogPageIndex).title);
		Manager::Get()->ProcessEvent(evtAdd1);

		wxArrayString Titles;
		wxArrayInt Widths;
		Titles.Add(_("File"));
		Titles.Add(_("Line"));
		Titles.Add(_("Message"));
		Widths.Add(128);
		Widths.Add(48);
		Widths.Add(640);
		m_ListLog = new CppCheckListLog(Titles, Widths);
		m_ListLogPageIndex = LogMan->SetLog(m_ListLog);
		LogMan->Slot(m_ListLogPageIndex).title = _("CppCheck messages");
		CodeBlocksLogEvent evtAdd2(cbEVT_ADD_LOG_WINDOW, m_ListLog, LogMan->Slot(m_ListLogPageIndex).title);
		Manager::Get()->ProcessEvent(evtAdd2);
	}
} // end of OnAttach

void CppCheck::OnRelease(bool appShutDown)
{
    // do de-initialization for your plugin
    // if appShutDown is false, the plugin is unloaded because Code::Blocks is being shut down,
    // which means you must not use any of the SDK Managers
    // NOTE: after this function, the inherited member variable
    // IsAttached() will be FALSE...
	if(Manager::Get()->GetLogManager())
	{
		if(m_CppCheckLog)
		{
			CodeBlocksLogEvent evt(cbEVT_REMOVE_LOG_WINDOW, m_CppCheckLog);
			Manager::Get()->ProcessEvent(evt);
		}
		if(m_ListLog)
		{
			CodeBlocksLogEvent evt(cbEVT_REMOVE_LOG_WINDOW, m_ListLog);
			Manager::Get()->ProcessEvent(evt);
		}
	}
	m_CppCheckLog = 0;
	m_ListLog = 0;
} // end of OnRelease

void CppCheck::WriteToLog(const wxString& Text)
{
	m_CppCheckLog->Clear();
	AppendToLog(Text);
} // end of WriteToLog

void CppCheck::AppendToLog(const wxString& Text)
{
    if(LogManager* LogMan = Manager::Get()->GetLogManager())
    {
		CodeBlocksLogEvent evtSwitch(cbEVT_SWITCH_TO_LOG_WINDOW, m_CppCheckLog);
		Manager::Get()->ProcessEvent(evtSwitch);
    	LogMan->Log(Text, m_LogPageIndex);
    }
} // end of AppendToLog

namespace
{
bool CheckRequirements()
{
    cbProject* Project = Manager::Get()->GetProjectManager()->GetActiveProject();
   // if no project open, exit
	if (!Project)
	{
		wxString msg = _("You need to open a project\nbefore using the plugin!");
		cbMessageBox(msg, _("Error"), wxICON_ERROR | wxOK, Manager::Get()->GetAppWindow());
		Manager::Get()->GetLogManager()->DebugLog(msg);
		return false;
	}
	return true;
}  // end of CheckRequirements
} // namespace

void CppCheck::DoCppCheckVersion()
{
	const wxString CommandLine = _("cppcheck --version");
	WriteToLog(CommandLine);
	wxArrayString Output, Errors;
	wxExecute(CommandLine, Output, Errors);
	int Count = Output.GetCount();
	for(int idxCount = 0; idxCount < Count; ++idxCount)
	{
		AppendToLog(Output[idxCount]);
	} // end for : idx: idxCount
	Count = Errors.GetCount();
	for(int idxCount = 0; idxCount < Count; ++idxCount)
	{
		AppendToLog(Errors[idxCount]);
	} // end for : idx: idxCount
	// and clear the list
	m_ListLog->Clear();
} // end of DoCppCheckVersion


int CppCheck::Execute()
{
	if(!CheckRequirements())
	{
		return -1;
	}
	DoCppCheckVersion();

	cbProject* Project = Manager::Get()->GetProjectManager()->GetActiveProject();
	::wxSetWorkingDirectory(Project->GetBasePath());
	const long Files = Project->GetFilesCount();
	wxString ListOfFileNames;
	for (int File = 0; File < Files; ++File)
	{
		ProjectFile* pf = Project->GetFile(File);
		ListOfFileNames += _T("\"") + pf->relativeFilename + _T("\" ");
	}

	const wxString CommandLine = _T("cppcheck --verbose --all --style --xml ") + ListOfFileNames.Trim();

	AppendToLog(CommandLine);
	wxArrayString Output, Errors;
	wxExecute(CommandLine, Output, Errors, wxEXEC_SYNC | wxEXEC_NODISABLE);
	size_t Count = Output.GetCount();
	for(size_t idxCount = 0; idxCount < Count; ++idxCount)
	{
		AppendToLog(Output[idxCount]);
	} // end for : idx: idxCount
	wxString Xml;
	Count = Errors.GetCount();
	for(size_t idxCount = 0; idxCount < Count; ++idxCount)
	{
		Xml += Errors[idxCount];
		AppendToLog(Errors[idxCount]);
	} // end for : idx: idxCount
	const bool UseXml = true;
	if(UseXml)
	{
		TiXmlDocument Doc;
		Doc.Parse(Xml.ToAscii());
		if(!Doc.Error())
		{
			bool ErrorsPresent = false;
			TiXmlHandle Handle(&Doc);
			Handle = Handle.FirstChildElement("results");
			for(const TiXmlElement* Error = Handle.FirstChildElement("error").ToElement(); Error;
					Error = Error->NextSiblingElement("error"))
			{
				wxString File;
				if(const char* FileValue = Error->Attribute("file"))
				{
					File = wxString::FromAscii(FileValue);
				}
				wxString Line;
				if(const char* LineValue = Error->Attribute("line"))
				{
					Line = wxString::FromAscii(LineValue);
				}
				wxString Id;
				if(const char* IdValue = Error->Attribute("id"))
				{
					Id = wxString::FromAscii(IdValue);
				}
				wxString Severity;
				if(const char* SeverityValue = Error->Attribute("severity"))
				{
					Severity = wxString::FromAscii(SeverityValue);
				}
				wxString Message;
				if(const char* MessageValue = Error->Attribute("msg"))
				{
					Message = wxString::FromAscii(MessageValue);
				}
				const wxString FulllMessage = Id + _T(" : ") + Severity + _T(" : ") + Message;
				if(!File.IsEmpty() && !Line.IsEmpty() && !FulllMessage.IsEmpty())
				{
					wxArrayString Arr;
					Arr.Add(File);
					Arr.Add(Line);
					Arr.Add(FulllMessage);
					m_ListLog->Append(Arr);
					ErrorsPresent = true;

				}
			}
			if(ErrorsPresent)
			{
				if(Manager::Get()->GetLogManager())
				{
					CodeBlocksLogEvent evtSwitch(cbEVT_SWITCH_TO_LOG_WINDOW, m_ListLog);
					Manager::Get()->ProcessEvent(evtSwitch);
				}
			}
		}
	}

	return 0;
} // end of Execute
//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  GAMOS software  is  copyright of the Copyright  Holders  of *
// * the GAMOS Collaboration.  It is provided  under  the  terms  and *
// * conditions of the GAMOS Software License,  included in the  file *
// * LICENSE and available at  http://fismed.ciemat.es/GAMOS/license .*
// * These include a list of copyright holders.                       *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GAMOS collaboration.                       *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the GAMOS Software license.           *
// ********************************************************************
//

#include "CDataSetManager.h"

#include "CUserParameters.h"
#include "C3Vector.h"
#include "CEvent.h"
#include "CCone.h"

template <>
void
CDataSetManager<C3Vector>::CleanCurrentMap()
{
	std::map<long, C3Vector*>::iterator mapiter;
	for (mapiter = m_dataSetMap.begin(); mapiter != m_dataSetMap.end(); mapiter++)
	{
		C3Vector* dataElement = ((*mapiter).second);
		delete dataElement;
	}
	m_dataSetMap.clear();
}

template <>
void
CDataSetManager<CEvent>::CleanCurrentMap()
{
	std::map<long, CEvent*>::iterator mapiter;
	for (mapiter = m_dataSetMap.begin(); mapiter != m_dataSetMap.end(); mapiter++)
	{
		CEvent* dataElement = ((*mapiter).second);
		delete dataElement;
	}
	m_dataSetMap.clear();
}

template <>
void
CDataSetManager<CCone>::CleanCurrentMap()
{
	std::map<long, CCone*>::iterator mapiter;
	for (mapiter = m_dataSetMap.begin(); mapiter != m_dataSetMap.end(); mapiter++)
	{
		CCone* dataElement = ((*mapiter).second);
		delete dataElement;
	}
	m_dataSetMap.clear();
}

template <>
void
CDataSetManager<C3Vector>::GetSetBaseFileName(std::string& io_fbasename) const
{
	io_fbasename = "CStateVector_";
}

template <>
void
CDataSetManager<CEvent>::GetSetBaseFileName(std::string& io_fbasename) const
{
	io_fbasename = "CEventMap_";
}

template <>
void
CDataSetManager<CCone>::GetSetBaseFileName(std::string& io_fbasename) const
{
	io_fbasename = "CConeMap_";
}

template <>
void
CDataSetManager<C3Vector>::GetCurrentFirstAndLastEventID(int& out_firstEventID, int& out_lastEventID) const
{
	std::map<long, C3Vector*>::const_iterator mapiter;
	mapiter = m_dataSetMap.begin();
	out_firstEventID = ((*mapiter).first);

	mapiter = m_dataSetMap.end();
  	mapiter--;
	out_lastEventID = ((*mapiter).first);

	assert(out_lastEventID - out_firstEventID + 1 > 0);
	assert(out_lastEventID - out_firstEventID + 1 == m_dataSetMap.size());
}

template <>
void
CDataSetManager<CEvent>::GetCurrentFirstAndLastEventID(int& out_firstEventID, int& out_lastEventID) const
{
	std::map<long, CEvent*>::const_iterator mapiter;
	mapiter = m_dataSetMap.begin();
	out_firstEventID = ((*mapiter).first);

	mapiter = m_dataSetMap.end();
  	mapiter--;
	out_lastEventID = ((*mapiter).first);

	assert(out_lastEventID - out_firstEventID + 1 > 0);
	assert(out_lastEventID - out_firstEventID + 1 == m_dataSetMap.size());
}

template <>
void
CDataSetManager<CCone>::GetCurrentFirstAndLastEventID(int& out_firstEventID, int& out_lastEventID) const
{
	std::map<long, CCone*>::const_iterator mapiter;
	mapiter = m_dataSetMap.begin();
	out_firstEventID = ((*mapiter).first);

	mapiter = m_dataSetMap.end();
  	mapiter--;
	out_lastEventID = ((*mapiter).first);

	assert(out_lastEventID - out_firstEventID + 1 > 0);
	assert(out_lastEventID - out_firstEventID + 1 == m_dataSetMap.size());
}


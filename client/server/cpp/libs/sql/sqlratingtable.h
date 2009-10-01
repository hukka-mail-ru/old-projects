#ifndef SQLRATINGTABLE_H
#define SQLRATINGTABLE_H

#include "sqltable.h"

/**
	@author WapPortal.RU <office@wapportal.ru>
*/
class CSqlRatingTable : private CSqlTable
{
public:
	CSqlRatingTable(const char* _strTableName, uint32_t _nInitialRating );
	
	~CSqlRatingTable();
	
	uint32_t	getRating( uint32_t _nPlayerID );	
	void		setRating( uint32_t _nPlayerID, uint32_t _nRating );

private:
	
	uint32_t		m_nInitialRating;

};

#endif

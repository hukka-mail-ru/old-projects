/******************************************************************************
*
*       (C) Copyright 2007, WapPortal.RU, Ltd. All Rights Reserved.
*
*       THIS SOURCE CODE IS CONFIDENTIAL AND PROPRIETARY AND MAY NOT
*       BE USED OR DISTRIBUTED WITHOUT THE WRITTEN PERMISSION OF
*       WapPortal.RU Ltd.
*
* Filename:     $Source: /home/cvs/wapserver3/libs/sql/sqltableusers.cpp,v $
* Author:       $Author: boon $
* Locker:       $Locker:  $
* State:        $State: Exp $
* Revision:     $Revision: 1.4 $
*
* History:      Use the CVS command log to display revision history
*               information.
*
* Description:
*
* Notes:
*
******************************************************************************/

#include "sqltableusers.h"
#include "../header/deferror.h"

#define MIN_USER_LOGIN_LENGTH		4
#define MAX_USER_LOGIN_LENGTH		19
#define MIN_USER_PASSWORD_LENGTH	4
#define MAX_USER_PASSWORD_LENGTH	39

CSqlTableUsers::CSqlTableUsers()
	:CSqlTable( "wsUsers", "User TEXT NOT NULL, Password TEXT NOT NULL, GUID INT PRIMARY KEY AUTO_INCREMENT, UNIQUE KEY(User(16))" )
{

}

CSqlTableUsers::~CSqlTableUsers()
{

}

char CSqlTableUsers::IsUserReg( char* _pcL, char* _pcP, uint32_t& _nID )
{
	TVecChar vecP;
	if( !GetPass( _pcL, &vecP ) )
		return ERRBADLOGIN;

	// TODO - convert to one 'if' !
        if( strlen(_pcP) == vecP.size() )
        {
		if( strncmp( _pcP, &vecP[0], vecP.size() ) )
			return ERRBADPASS;
	}
	else
		return ERRBADPASS;

	if( !GetID( _pcL, _nID ) )
		return ERRBADLOGIN;

	return NOERR;
}

char CSqlTableUsers::DoUserReg( char* _pcL, char* _pcP, uint32_t& _nID )
{
	int nSize = strlen( _pcL );

	if( ( nSize < MIN_USER_LOGIN_LENGTH ) || ( MAX_USER_LOGIN_LENGTH < nSize ) || !CheckCharSet( _pcL ) )
		return ERRBADLOGIN;

	nSize = strlen( _pcP );

	if( ( nSize < MIN_USER_PASSWORD_LENGTH ) || ( MAX_USER_PASSWORD_LENGTH < nSize ) || !CheckCharSet( _pcP ) )
		return ERRBADPASS;

	TVecChar vecP;
	if( GetPass( _pcL, &vecP ) )
		return ERRLOGINEXIST;

	Insert( _pcL, _pcP );

	if( !GetID( _pcL, _nID ) )
		return ERRBADLOGIN;

	return NOERR;
}

bool CSqlTableUsers::GetPass( char* _pcL, TVecChar* _pvecP )
{
	return SelectToStr( "Password", "User", _pcL, _pvecP );
}

bool CSqlTableUsers::GetID( char* _pcL, uint32_t& _nID )
{
	TVecChar vecData;

	if( !SelectToStr( "GUID", "User", _pcL, &vecData ) )
		return false;

	_nID = atoi( &vecData[0] );

	return true;
}

void CSqlTableUsers::Insert( char* _pcL, char* _pcP )
{
	CMyStr strValue( "'"+CMyStr( _pcL )+"' , '"+CMyStr( _pcP )+"' , 0" );

	CSqlTable::Insert( strValue.c_str() );
}

bool CSqlTableUsers::CheckCharSet( const char* _pC ) const
{
	while( *_pC != 0 )
	if( ( *_pC < 48 ) || ( 122 < *_pC++ ) )
		return false;

	return true;
}

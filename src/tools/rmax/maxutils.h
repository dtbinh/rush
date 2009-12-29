/***********************************************************************************/
//  File:   maxutils.h
//  Desc:   Max exporting related utilities
//  Date:   26.11.2006
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __MAXUTILS_H__
#define __MAXUTILS_H__

Matrix3     GetLocalTM  ( INode* node, TimeValue t );
Mat4        Convert     ( const Matrix3& m );
DWORD       ColorToDWORD( const Color& color );
float       TimeToMs    ( TimeValue t );
TimeValue   MsToTime    ( float ms );

#endif // __MAXUTILS_H__
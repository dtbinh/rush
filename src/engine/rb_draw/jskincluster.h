/***********************************************************************************/
//  File:   JSkinCluster.h
//  Date:   10.08.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __JSKINCLUSTER_H__
#define __JSKINCLUSTER_H__

/***********************************************************************************/
//  Class:  JSkinCluster
//  Desc:    
/***********************************************************************************/
struct JSkinCluster  
{
    JSkinCluster() : m_FirstVert(0), m_NVert(0), m_NWeights(0){}
    JSkinCluster( int firstV, int numV, int numWeights ) 
        : m_FirstVert(firstV), m_NVert(numV), m_NWeights(numWeights){}

   int      m_FirstVert;
   int      m_NVert;
   int      m_NWeights;

}; // class JSkinCluster

#endif //__JSKINCLUSTER_H__
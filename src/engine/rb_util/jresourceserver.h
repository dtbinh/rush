/***********************************************************************************/
//  File:   JResourceServer.h
//  Date:   14.08.2006
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#ifndef __JRESOURCESERVER_H__
#define __JRESOURCESERVER_H__

/***********************************************************************************/
//  Class:  JResourceServer
//  Desc:   Provides resource managment service, including:
/*          - fast searching for resource by filename
/*          - loading data on demand, caching files completely or partially
/*          - optionally providing file mapping for resources
/*          - keeping the memory budget using LRU scheme
/*          - reding files transparently from different sources: 
/*              directory trees, or bundled package files, or embedded resource data lump
/*          - on demand gathering statistics about file access patterns, and 
/*              providing means to reorder bundle files basing on that statistics
/*          - providing real-time statistics for monitoring resource usage
/*          - pre-fetching resources on demand
/***********************************************************************************/
class JResourceServer
{
public:   
            JResourceServer() {}

}; // class JResourceServer

extern JResourceServer* g_pResourceServer;

#endif //__JRESOURCESERVER_H__
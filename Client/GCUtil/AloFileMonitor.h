/**
 * @file	AloFileMonitor.h
 * @brief   CAloFileMonitor class 
 * @author	alones
 * @date initial version: 2007.08.22 
 * @brief http://alones.kr/wiki/wiki.php/c_cpp_windows_tip_alofilemonitor
 */

#ifndef __ALO_FILE_MONITOR_H__
#define __ALO_FILE_MONITOR_H__

//#include "windows.h"
//#include <string>

// function pointer for call-back when changed
typedef void (*PFnFileChanged)();

/**
 * @brief monitor file changed
 */
class CAloFileMonitor
{
	// attributes
public:
	std::string		m_filePath;			/**< file to monitor */	
	_WIN32_FILE_ATTRIBUTE_DATA	m_fileAttData;
	std::string		m_dirPath;			/**< directory contain the file to monitor */
	PFnFileChanged	m_pFnFileChanged;	/**< call-back when file changed */
    HANDLE dwChangeHandles[2]; 	
	// operations
public:
	/** @brief set file monitor, file path and call-back funtion */
	bool SetFileMonitor(std::string filePath, PFnFileChanged pFn);

	/** @brief start monitor */
	bool Start();

	/** @brief stop monitor */
	bool Stop();
	
};

#endif //__ALO_FILE_MONITOR_H__
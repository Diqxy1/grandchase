#pragma once

// Client crash callback
typedef void (CALLBACK *BT_ErrHandler)(INT_PTR nErrHandlerParam);

typedef enum BUGTRAP_ACTIVITY_tag
{
	/**
	* @brief Display BugTrap dialog to allow user selecting desirable
	* option. This is the default option.
	*/
	BTA_SHOWUI       = 1,
	/**
	* @brief Automatically save error report to file.
	* Use BT_SetReportFilePath() to specify report path.
	*/
	BTA_SAVEREPORT   = 2,
	/**
	* @brief Automatically send error report by e-mail.
	*/
	BTA_MAILREPORT   = 3,
	/**
	* @brief Automatically send bug report to support server.
	*/
	BTA_SENDREPORT   = 4
}
BUGTRAP_ACTIVITY;

/**
* @brief Different BugTrap options. You can use any combinations
* of these flags.
*/
typedef enum BUGTRAP_FLAGS_tag
{
	/**
	* @brief Equivalent of no options.
	*/
	BTF_NONE           = 0x00,
	/**
	* @brief In detailed mode BugTrap generates mini-dump and
	* packs custom log files within the report.
	*/
	BTF_DETAILEDMODE   = 0x01,
	/**
	* @brief BugTrap may open its own editor for e-mail messages
	* instead of the editor used by the system. Use this
	* option if you aren't aware of the type of e-mail
	* client installed on user computers.
	*/
	BTF_EDITMAIL       = 0x02,
	/**
	* @brief Specify this option to attach bug report to e-mail
	* messages. Be careful with this option. It's potentially
	* dangerous for the detailed mode because of even zipped
	* mini-dump may require huge size. It may exceed the
	* maximum size of e-mail message supported by Internet
	* provider.
	*/
	BTF_ATTACHREPORT   = 0x04,
	/**
	* @brief Set this flag to add list of all processes and loaded
	* modules to the report. Disable this option to speedup report
	* generation.
	*/
	BTF_LISTPROCESSES  = 0x08,
	/**
	* @brief By default BugTrap displays simplified dialog on the
	* screen allowing user to perform only common actions. Enable
	* this flag to immediately display dialog with advanced error
	* information.
	*/
	BTF_SHOWADVANCEDUI = 0x10,
	/**
	* @brief Bug report in detailed error mode may also include a
	* screen shot automatically captured by BugTrap. By default this
	* option is disabled to minimize report size, but it may be useful
	* if you want to know which dialogs were shown on the screen.
	*/
	BTF_SCREENCAPTURE  = 0x20,
#ifdef _MANAGED
	/**
	* @brief Generate native stack trace and modules information along
	* with managed exception information. Disable this option to speedup
	* report generation.
	*/
	BTF_NATIVEINFO    = 0x40
#endif // _MANAGED
}
BUGTRAP_FLAGS;

/**
* @brief Set of available log levels.
*/
typedef enum BUGTRAP_LOGLEVEL_tag
{
	/**
	* @brief All levels of messages are disabled.
	*/
	BTLL_NONE    = 0,
	/**
	* @brief Error message.
	*/
	BTLL_ERROR   = 1,
	/**
	* @brief Waning message.
	*/
	BTLL_WARNING = 2,
	/**
	* @brief Information message.
	*/
	BTLL_INFO    = 3,
	/**
	* @brief All levels of messages are accepted.
	*/
	BTLL_ALL     = BTLL_INFO
}
BUGTRAP_LOGLEVEL;

/**
* @brief Type of log echo mode.
*/
typedef enum BUGTRAP_LOGECHOTYPE_tag
{
	/**
	* @brief Do not duplicate log output to file.
	*/
	BTLE_NONE   = 0x00,
	/**
	* @brief Send a copy of log output to STDOUT (mutually exclusive with STDERR mode).
	*/
	BTLE_STDOUT = 0x01,
	/**
	* @brief Send a copy of log output to STDERR (mutually exclusive with STDOUT mode).
	*/
	BTLE_STDERR = 0x02,
	/**
	* @brief Send a copy of log output to the debugger (can accompany STDOUT or STDERR mode).
	*/
	BTLE_DBGOUT = 0x04
}
BUGTRAP_LOGECHOTYPE;

/**
* @brief Set of log file options.
*/
typedef enum BUGTRAP_LOGFLAGS_tag
{
	/**
	* @brief Do not show any additional entries in the log.
	*/
	BTLF_NONE          = 0x00,
	/**
	* @brief Use this option if you want to store message levels in a file.
	*/
	BTLF_SHOWLOGLEVEL  = 0x01,
	/**
	* @brief Use this option if you want to store message timestamps in a file.
	* Timestamps are stored in universal (locale independent) format: YYYY/MM/DD HH:MM:SS
	*/
	BTLF_SHOWTIMESTAMP = 0x02
}
BUGTRAP_LOGFLAGS;

/**
* @brief Format of error report.
*/
typedef enum BUGTRAP_REPORTFORMAT_tag
{
	/**
	* @brief Report stored in structured XML format.
	*/
	BTRF_XML  = 1,
	/**
	* @brief Report stored in plain text format.
	*/
	BTRF_TEXT = 2
}
BUGTRAP_REPORTFORMAT;

/**
* @brief Format of log file.
*/
typedef enum BUGTRAP_LOGFORMAT_tag
{
	/**
	* @brief Log stored in structured XML format.
	*/
	BTLF_XML  = 1,
	/**
	* @brief Log stored in plain text format.
	*/
	BTLF_TEXT = 2
}
BUGTRAP_LOGFORMAT;

/**
* @brief Type of user defined message displayed on the screen.
*/
typedef enum BUGTRAP_DIALOGMESSAGE_tag
{
	/**
	* @brief 1st line of introduction message.
	*/
	BTDM_INTRO1 = 1,
	/**
	* @brief A bunch of lines of introduction message.
	*/
	BTDM_INTRO2 = 2
}
BUGTRAP_DIALOGMESSAGE;

/**
* @brief Type of log file attached to the report.
*/
typedef enum BUGTRAP_LOGTYPE_tag
{
	/**
	* @brief Regular log file maintained by the user.
	*/
	BTLT_LOGFILE,
	/**
	* @brief Log file that's exported from the registry.
	*/
	BTLT_REGEXPORT
}
BUGTRAP_LOGTYPE;
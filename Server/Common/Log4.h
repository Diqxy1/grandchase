#ifndef __MYHEADERFILE_H__
#define __MYHEADERFILE_H__

#pragma once

#include <log4cxx/logger.h>
#include <log4cxx/basicconfigurator.h>
#include <log4cxx/propertyconfigurator.h>
#include <log4cxx/helpers/exception.h>
#include <log4cxx/logmanager.h>

namespace { log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger("GCLog")); }
//FILE_NAME_FOR_LOG

#endif
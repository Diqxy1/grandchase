#pragma once

//#include <boost/shared_ptr.hpp>

#define SmartPointer( classname ) class classname; typedef boost::shared_ptr<classname> classname##Ptr
#define SmartPointerStruct( classname ) struct classname; typedef boost::shared_ptr<classname> classname##Ptr
#define SmartPtrWithNamespace( ns, classname ) namespace ns{ class classname; } typedef boost::shared_ptr<ns##::classname> classname##Ptr

#define WeakPointer( classname ) class classname; typedef boost::weak_ptr<classname> classname##WPtr
#define WeakPointerStruct( classname ) struct classname; typedef boost::weak_ptr<classname> classname##WPtr
#define WeakPtrWithNamespace( ns, classname ) namespace ns{ class classname; } typedef boost::weak_ptr<ns##::classname> classname##WPtr
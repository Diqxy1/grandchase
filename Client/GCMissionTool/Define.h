//#include <string>
//#include <sstream>

#define MISSION_INI_FILE    "MissionTool.ini"

#define OKBOXW( msg, title ) \
{\
    std::wstringstream strstm;\
    strstm<<msg;\
    ::MessageBoxW( NULL, strstm.str().c_str() , title , MB_OK );\
}


#define W2T2(x, y)   WideCharToMultiByte( CP_ACP, 0, y, -1, x, MAX_PATH, NULL, NULL)
#define T2W2(x, y)   MultiByteToWideChar( CP_ACP, 0, y, -1, x, MAX_PATH)
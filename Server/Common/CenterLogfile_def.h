#ifdef __NEVER_DEFINED_MACRO__
#define _ENUM(VAL, FolderName) VAL,
enum
{
#endif

	_ENUM(LT_OVERLAPPED_FILE, overlapped)
	_ENUM(LT_BLOCKIPUSER_CONN, BlockIPUser)
	_ENUM(LT_DB_QUEUE_SIZE, DBLayer_queue_size)
	_ENUM(LT_CYOU_CHANGE_CASH, CYOU_ChangeCashLog)
	_ENUM(LT_MAX, LT_MAX)

#ifdef __NEVER_DEFINED_MACRO__
};
#undef _ENUM
#endif
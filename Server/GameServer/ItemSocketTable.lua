OPENED_SOCKET	= 0
CLOSED_SOCKET	= 1
RANDOM_SOCKET	= 2


-- Socket Table for each Grade
DefaultSocketTable = {
	{
		SocketNum = 0,
		SocketInfo = {
			{ OPENED_SOCKET,	0,	100	},
			{ CLOSED_SOCKET,	0,	100	},
			{ RANDOM_SOCKET,	0,	20	},
		}
	},
	{
		SocketNum = 1,
		SocketInfo = {
			{ OPENED_SOCKET,	0,	100	},
			{ CLOSED_SOCKET,	0,	100	},
			{ RANDOM_SOCKET,	0,	20	},
		}
	},
	{
		SocketNum = 2,
		SocketInfo = {
			{ OPENED_SOCKET,	2,	100	},
			{ CLOSED_SOCKET,	0,	100	},
			{ RANDOM_SOCKET,	0,	20	},
		}
	},
	{
		SocketNum = 3,
		SocketInfo = {
			{ OPENED_SOCKET,	3,	100	},
			{ CLOSED_SOCKET,	0,	100	},
			{ RANDOM_SOCKET,	0,	20	},
		}
	},
}


-- Socket Table for special items
ItemSocketTable = {
	{
		SocketNum = 1,
		SocketInfo = {
			{ OPENED_SOCKET,	3,	100	},
			{ CLOSED_SOCKET,	0,	100	},
			{ RANDOM_SOCKET,	0,	100	},
		}
	},
}



SQUARE_NORMAL = 0
SQUARE_GUILD = 1

SquareManager = GetCnSquareManager()
info = KSquareSetInfo()

---[[
info:SetInfo( "str_00", 50, SQUARE_NORMAL )
info:SetAddress( "116.120.238.39", 9700 )
SquareManager:AddSquare( info, "SquareInfo" )

info:SetInfo( "str_01", 50, SQUARE_NORMAL )
info:SetAddress( "116.120.238.39", 9700 )
SquareManager:AddSquare( info, "SquareInfo" )

info:SetInfo( "str_02", 50, SQUARE_NORMAL )
info:SetAddress( "116.120.238.39", 9700 )
SquareManager:AddSquare( info, "SquareInfo" )

info:SetInfo( "str_03", 50, SQUARE_NORMAL )
info:SetAddress( "116.120.238.39", 9700 )
SquareManager:AddSquare( info, "SquareInfo" )

info:SetInfo( "str_04", 50, SQUARE_NORMAL )
info:SetAddress( "116.120.238.39", 9700 )
SquareManager:AddSquare( info, "SquareInfo" )
--]]

---[[
SquareTRServerCounter = 1;
SquareTRServerIPList = {
	{ "116.120.238.39", 9700 },
}

function GetGuildSquareSetInfo()

	local SquareInfo = KSquareSetInfo()
	SquareInfo:SetInfo( "str_00", 50, SQUARE_GUILD )

	SquareTRServerCounter = SquareTRServerCounter + 1
	if SquareTRServerCounter > table.maxn( SquareTRServerIPList ) then
		SquareTRServerCounter = 1;
	end

	local Address = SquareTRServerIPList[SquareTRServerCounter];

	if type( Address ) == "table" then
		SquareInfo:SetAddress( Address[1], Address[2] );
	else
		SquareInfo:SetAddress( "116.120.238.39", 9700 );
	end

	return SquareInfo;
end

--]]

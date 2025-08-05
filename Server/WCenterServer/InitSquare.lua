SQUARE_NORMAL = 0
SQUARE_GUILD = 1

SquareManager = GetCnSquareManager()
info = KSquareSetInfo()

---[[
info:SetInfo( "str_00", 50, SQUARE_NORMAL )
info:SetAddress( "192.168.199.104", 9700 )
SquareManager:AddSquare( info, "SquareInfo" )

info:SetInfo( "str_01", 50, SQUARE_NORMAL )
info:SetAddress( "192.168.199.104", 9710 )
SquareManager:AddSquare( info, "SquareInfo" )

info:SetInfo( "str_02", 50, SQUARE_NORMAL )
info:SetAddress( "192.168.199.104", 9700 )
SquareManager:AddSquare( info, "SquareInfo" )

info:SetInfo( "str_03", 50, SQUARE_NORMAL )
info:SetAddress( "192.168.199.104", 9700 )
SquareManager:AddSquare( info, "SquareInfo" )

info:SetInfo( "str_04", 50, SQUARE_NORMAL )
info:SetAddress( "192.168.199.104", 9700 )
SquareManager:AddSquare( info, "SquareInfo" )
--]]

---[[
info:SetInfo( "str_00", 50, SQUARE_GUILD )
info:SetAddress( "192.168.199.104", 9700 )
SquareManager:AddGuildSquareSetInfo( info )

info:SetInfo( "str_01", 50, SQUARE_GUILD )
info:SetAddress( "192.168.199.104", 9700 )
SquareManager:AddGuildSquareSetInfo( info )

info:SetInfo( "str_02", 50, SQUARE_GUILD )
info:SetAddress( "192.168.199.104", 9700 )
SquareManager:AddGuildSquareSetInfo( info )

info:SetInfo( "str_03", 50, SQUARE_GUILD )
info:SetAddress( "192.168.199.104", 9700 )
SquareManager:AddGuildSquareSetInfo( info )

info:SetInfo( "str_04", 50, SQUARE_GUILD )
info:SetAddress( "192.168.199.104", 9700 )
SquareManager:AddGuildSquareSetInfo( info )

--]]


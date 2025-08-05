
ChannelDrop = GetChannelDrop()
ChannelDrop:Clear();
ChannelDrop:AddDropInfo{
    Channel = { 1, 4, 3, 5, },
    character = { 0, 3, },
    Level = { 0, 75 },
    win = 100,
    lose = 100,
    Items = { {4200, 1}, {4320, -1} , {5810, 5}, },
}

print( ChannelDrop )


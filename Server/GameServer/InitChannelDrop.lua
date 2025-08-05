
ChannelDrop = GetChannelDrop()
ChannelDrop:Clear();
ChannelDrop:AddChannelDrop{
    channel = { 1, 4, 3, 5, },
    character = { 0, 3, },
    level = { 0, 75 },
    win = 100,
    lose = 100,
    items = { {4200, 1}, {4320, -1} , {5810, 5}, },
    bselect = true,
    ratio = { 30, 20, 10, }
}

print( ChannelDrop )


#pragma once

template <class T>
std::wostream& operator<<( std::wostream& stm_, const std::pair<const int,T>& data_ )
{
    return stm_ << data_.first;
}

#pragma once

#ifndef __FUNTION_H__
#define __FUNTION_H__

template <typename T>
void Safe_Delete(T& _Dst)
{
	if (_Dst)
	{
		delete _Dst;
		_Dst = nullptr;
	}
}

#endif // !__FUNTION_H__

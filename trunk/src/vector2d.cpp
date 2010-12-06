//#include "vector2d.h"
//template <typename T>
//void vector2d<T>::clear()
//{
//	std::vector< std::vector<T> >::iterator itr;
//	for(itr= mVector.begin();itr != mVector.end();itr++)
//	{
//		std::vector<T2D> cur = *itr;
//		cur.clear();
//	}
//	mVector.clear();
//	maxX = 0;
//	maxY = 0;
//}
//template <typename T>
//T vector2d<T>::getAt(unsigned int x, unsigned int y)
//{
//	unsigned int maxX = mVector.size()-1;
//	if(x<=maxX)
//	{
//		unsigned int maxY = mVector[x].size()-1;
//		if(y <= maxY)
//		{
//			return mVector[x][y];
//		}
//		else
//			return valEmpty;
//	}
//	else
//	{
//		return valEmpty;
//	}
//}
//template <typename T>
//void vector2d<T>::setAt(unsigned int x, unsigned int y, T val)
//{
//	
//	unsigned int sizeX = mVector.size();
//	if(sizeX <= y)
//	{
//		for(unsigned int i = sizeX;i<=x;i++)
//		{
//			std::vector<T> temp;
//			
//			mVector.push_back(temp);
//		}
//		
//
//	}
//	unsigned int sizeY = mVector[x].size();
//	if(sizeY <= y)
//	{
//		//dh hier Y nachfüllen
//		for(unsigned int i=sizeY;i<=y;i++)
//		{
//			mVector[x].push_back(valEmpty);
//		}
//	}
//	
//	mVector[x][y] = val;
//
//}
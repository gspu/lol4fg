#ifndef __bdvector2d
#define __bdvector2d

#include <vector>

template <typename T>
class BDVector2d
{
public:	
	//clears everything
	void clear()
	{
		std::vector< std::vector<T> >::iterator itr;
		for(itr= pVector.begin();itr != pVector.end();itr++)
		{
			std::vector<T2D> cur = *itr;
			cur.clear();
		}
		pVector.clear();

		for(itr= nVector.begin();itr != nVector.end();itr++)
		{
			std::vector<T2D> cur = *itr;
			cur.clear();
		}
		nVector.clear();
		maxX = 0;
		maxY = 0;
	}
	
	//retrieves the value at given coordinates. 
	//returns valEmpty if value not specified or 
	//coords out of range
	T getAt(int x, int y)
	{
		//schaun 
		size_t sizeX = mVector.size();
		if(x<sizeX)
		{
			size_t sizeY = mVector[x].size();
			if(y < sizeY)
			{
				return mVector[x][y];
			}
			else
				return valEmpty;
		}
		else
		{
			return valEmpty;
		}
	}
	bool isEmpty(unsigned int x, unsigned int y)
	{
		if(getAt(x,y) == valEmpty)
			return true;
		else
			return false;
	}
	//checks if all elements have the same value
	bool allElements(T val)
	{
		//bool result = true;
		for(size_t x=0;x<mVector.size();x++)
		{
			for(size_t y=0;y<mVector[x].size();y++)
			{
				if(mVector[x][y] != val)
				{
					//result = false;
					return false;
					
				}
			}
		}
		return true;
	}

	//assigns a value to given coordinates
	void setAt(size_t x, size_t y, T val)
	{
		
		size_t sizeX = mVector.size();
		if(sizeX <= x)
		{
			for(size_t i = sizeX;i<=x;i++)
			{
				std::vector<T> temp;
				
				mVector.push_back(temp);
			}
			

		}
		size_t sizeY = mVector[x].size();
		if(sizeY <= y)
		{
			//dh hier Y nachfüllen
			for(size_t i=sizeY;i<=y;i++)
			{
				mVector[x].push_back(valEmpty);
			}
		}
		
		mVector[x][y] = val;

	}

	//this value will be returned if there is no value
	//at given coords
	void setEmptyVal(T val)
	{
		valEmpty = val;
	}
	//positiver Vektor
	std::vector< std::vector<T> > pVector;
	//negativer Vektor
	std::vector< std::vector<T> > nVector;
private:
	T valEmpty;

};

#endif
#pragma once

template <typename  ElementType>
struct TNonNullCheckedPointerIterator
{
	explicit TNonNullCheckedPointerIterator(TCheckedPointerIterator<ElementType, int> Iterator, TCheckedPointerIterator<ElementType, int> End): Iterator(Iterator), End(End)
	{
	}

	FORCEINLINE ElementType& operator*() const
	{
		return *Iterator;
	}

	FORCEINLINE TNonNullCheckedPointerIterator& operator++()
	{
		while(Iterator != End && !IsValid(*(++Iterator))){}
		return *this;
	}

private:
	TCheckedPointerIterator<ElementType, int> Iterator;
	TCheckedPointerIterator<ElementType, int> End;

	FORCEINLINE friend bool operator!=(const TNonNullCheckedPointerIterator& Lhs, const TCheckedPointerIterator<ElementType, int>& Rhs)
	{
		return Lhs.Iterator != Rhs;
	}
};

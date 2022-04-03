#pragma once

template <typename  ElementType, typename IteratorType>
struct TNonNullIterator
{
	explicit TNonNullIterator(IteratorType Iterator, IteratorType End): Iterator(Iterator), End(End)
	{
	}

	FORCEINLINE ElementType operator*() const
	{
		return *Iterator;
	}

	FORCEINLINE TNonNullIterator& operator++()
	{
		while(Iterator != End && !IsValid(*(++Iterator))){}
		return *this;
	}

private:
	IteratorType Iterator;
	IteratorType End;

	FORCEINLINE friend bool operator!=(const TNonNullIterator& Lhs, const IteratorType& Rhs)
	{
		return Lhs.Iterator != Rhs;
	}
};

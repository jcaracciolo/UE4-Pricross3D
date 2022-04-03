// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Definitions.h"
#include "TNonNullCheckedPointerIterator.h"
#include "Picross3D/GameLogic/PiCube.h"
#include "FPuzzleMatrix.generated.h"

class APiCube;
/**
 * 
 */
USTRUCT()
struct PICROSS3D_API FPuzzleMatrix
{
	GENERATED_BODY()

	FPuzzleMatrix();

	UPROPERTY()
	TArray<APiCube*> Array;

	void Add(APiCube* Cube)
	{
		Array.Add(Cube);
	}

	void SetupMatrix(const uint32 X, const uint32 Y, const uint32 Z);
	APiCube* operator()(FIntVector Position) const;
	uint32 inline GetMaxSize(EPiAxis Axis) const;
	void Remove(FIntVector Position);

private:
	template <typename T>
	struct TMatrixIterator
	{
		TMatrixIterator(const FPuzzleMatrix* Matrix, const FIntVector StartingPosition, const EPiAxis Direction) noexcept : Matrix(Matrix),
			CurrentPosition(StartingPosition), Direction(Direction)
		{
		}

		TMatrixIterator() noexcept : bIsEndIterator(true), Direction()
		{
		}

		uint32 GetCurrentDirectionPosition() noexcept;
		TMatrixIterator<T>& operator++() noexcept;
		bool operator!=(const TMatrixIterator<T>& Other) const noexcept;

		T operator*() const noexcept
		{
			return this->Matrix->operator()(CurrentPosition);
		}

	private:
		bool bIsEndIterator = false;
		const FPuzzleMatrix* Matrix = nullptr;
		FIntVector CurrentPosition = {0, 0, 0};
		const EPiAxis Direction;
	};

public:
	
	typedef TMatrixIterator<APiCube*> DirIterator; 
	typedef TMatrixIterator<APiCube* const> ConstDirIterator; 

	FORCEINLINE DirIterator begin(const FIntVector StartingPosition, const EPiAxis Direction) noexcept
	{
		return DirIterator(this, StartingPosition, Direction);
	}

	FORCEINLINE ConstDirIterator begin(const FIntVector StartingPosition, const EPiAxis Direction) const noexcept
	{
		return ConstDirIterator(this, StartingPosition, Direction);
	}

	FORCEINLINE DirIterator DirEnd() noexcept
	{
		return DirIterator();
	};

	FORCEINLINE ConstDirIterator DirEnd() const noexcept
	{
		return ConstDirIterator();
	};

	FORCEINLINE auto begin() const noexcept
	{
		return TNonNullCheckedPointerIterator<APiCube* const>(Array.begin(), Array.end());
	}

	FORCEINLINE auto begin() noexcept
	{
		return TNonNullCheckedPointerIterator<APiCube*>(Array.begin(), Array.end());
	}


	FORCEINLINE auto end() const noexcept
	{
		return Array.end();
	};

	auto end() noexcept
	{
		return Array.end();
	}

private:
	int32 SizeX = 0, SizeY = 0, SizeZ = 0;
	inline uint32 ToIndex(FIntVector Position) const;
};

template <typename T>
uint32 FPuzzleMatrix::TMatrixIterator<T>::GetCurrentDirectionPosition() noexcept
{
	switch (Direction)
	{
	case EPiAxis::X:
		return CurrentPosition.X;
	case EPiAxis::Y:
		return CurrentPosition.Y;
	case EPiAxis::Z:
		return CurrentPosition.Z;
	default: FAIL_UNKNOWN_AXIS;
	}

	return -1;
}

template <typename T>
FPuzzleMatrix::TMatrixIterator<T>& FPuzzleMatrix::TMatrixIterator<T>::operator++() noexcept

{
	if (GetCurrentDirectionPosition() + 1 < Matrix->GetMaxSize(Direction))
	{
		switch (Direction)
		{
		case EPiAxis::X:
			CurrentPosition += FIntVector{1, 0, 0};
			break;
		case EPiAxis::Y:
			CurrentPosition += FIntVector{0, 1, 0};
			break;
		case EPiAxis::Z:
			CurrentPosition += FIntVector{0, 0, 1};
			break;
		default: FAIL_UNKNOWN_AXIS;
		}
	}
	else
	{
		bIsEndIterator = true;
	}

	return *this;
}

template <typename T>
bool FPuzzleMatrix::TMatrixIterator<T>::operator!=(const TMatrixIterator<T>& Other) const noexcept
{
	if (bIsEndIterator && Other.bIsEndIterator)
	{
		return false;
	}
	return bIsEndIterator != Other.bIsEndIterator || Direction != Other.Direction || CurrentPosition != Other.CurrentPosition;
}

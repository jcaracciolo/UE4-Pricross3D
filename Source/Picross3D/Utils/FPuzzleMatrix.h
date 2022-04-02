// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Definitions.h"
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
	explicit FPuzzleMatrix(TArray<APiCube*>* NewArray);

	void SetupMatrix(const uint32 X, const uint32 Y, const uint32 Z);
	APiCube* Get(const uint32 X, const uint32 Y, const uint32 Z) const;
	uint32 inline GetMaxSize(EPiAxis Axis) const;

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
			return this->Matrix->Get(CurrentPosition.X, CurrentPosition.Y, CurrentPosition.Z);
		}

	private:
		bool bIsEndIterator = false;
		const FPuzzleMatrix* Matrix = nullptr;
		FIntVector CurrentPosition = {0, 0, 0};
		const EPiAxis Direction;
		
	};

public:
	TMatrixIterator<APiCube*> begin(FIntVector StartingPosition, EPiAxis Direction) const noexcept
	{
		return TMatrixIterator<APiCube*>(this, StartingPosition, Direction);
	}

	static TMatrixIterator<APiCube*> end() noexcept
	{
		return TMatrixIterator<APiCube*>();
	};

	TMatrixIterator<const APiCube*> cbegin(FIntVector StartingPosition, EPiAxis Direction) const noexcept
	{
		return TMatrixIterator<const APiCube*>(this, StartingPosition, Direction);
	}

	static TMatrixIterator<const APiCube*> cend() noexcept
	{
		return TMatrixIterator<const APiCube*>();
	};

private:
	uint32 SizeX = 0, SizeY = 0, SizeZ = 0;
	TArray<APiCube*>* Array;
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
	default:
		checkf(0, TEXT("Unknown Axis"));
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
		default:
			checkf(0, TEXT("Unknown Axis"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ITERATOR DONE"));
		bIsEndIterator = true;
	}

	return *this;
}

template <typename T>
bool FPuzzleMatrix::TMatrixIterator<T>::operator!=(const TMatrixIterator<T>& Other) const noexcept
{
	if(bIsEndIterator && Other.bIsEndIterator)
	{
		return false;
	}
	return bIsEndIterator != Other.bIsEndIterator || Direction != Other.Direction || CurrentPosition != Other.CurrentPosition;
}
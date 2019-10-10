#pragma once

#define TOCH(str) QString::fromLocal8Bit(str)

enum TableHeader
{
	Oder = 0,
	ProductionId,
	ProductionName,
	BorrowerName,
	BorrowDate,
	Status
};

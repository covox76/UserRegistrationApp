#pragma once

#include "pch.h"

constexpr auto AppName = L"UserRegistrationApp";

namespace FileExtension
{
  constexpr auto Json = L".json";
}

namespace InfoText
{
  constexpr auto OutputFileSaved = L"Zapisano dane do pliku wynikowego.";
  constexpr auto InputDataLoaded = L"Pobrano dane wejœciowe.";
}

namespace WarningText
{
  constexpr auto RequiredDataMissing = L"Brak wymaganych danych. Proszê wype³niæ wszystkie pola edycyjne.";
}

namespace ErrorText
{
  constexpr auto UnexpectedProblem = L"Nieoczekiwany problem.";
  constexpr auto GuidGenerateProblem = L"B³¹d generowania GUID.";
}

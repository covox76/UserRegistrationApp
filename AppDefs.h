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
  constexpr auto InputDataLoaded = L"Pobrano dane wej�ciowe.";
}

namespace WarningText
{
  constexpr auto RequiredDataMissing = L"Brak wymaganych danych. Prosz� wype�ni� wszystkie pola edycyjne.";
}

namespace ErrorText
{
  constexpr auto UnexpectedProblem = L"Nieoczekiwany problem.";
  constexpr auto GuidGenerateProblem = L"B��d generowania GUID.";
}

// Fill out your copyright notice in the Description page of Project Settings.

#include "ExcelImporter.h"
#include "IDesktopPlatform.h"
#include "DesktopPlatformModule.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/EditableText.h"
#include "Components/TextBlock.h"

#include "Engine/CurveTable.h"
#include "AssetCreator.h"
#include "Engine/CurveTable.h"

#define ASSET_SAVE_PATH_PREFIX	TEXT("/Game/")
#define ASSET_SAVE_PATH_PROMPT	TEXT("The asset path must start with /Game/. Please ensure your input begins with this prefix.")
#define EXCEL_PATH_PROMPT		TEXT("The excel file to be loaded is not found. Please input a valid path.")
#define SHEET_DEFAULT_NUM		TEXT("0")
#define SHEET_NUMBER_PROMPT		TEXT("It takes number as input for sheet number.")
#define MAX_SHEET_NUM			99
#define LIBXL_NAME				L"Aleidi"
#define	LIBXL_KEY				L"windows-2b2b21050cc1e30b6ab3606ea9p1rdg1"

UE_DISABLE_OPTIMIZATION
UExcelImporter::UExcelImporter()
{
	IsExcelPathValid = false;
}

void UExcelImporter::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void UExcelImporter::NativeConstruct()
{
	Super::NativeConstruct();

	BtnFindExcelFiles->OnClicked.AddDynamic(this, &UExcelImporter::FindPath);

	BtnExecute->OnClicked.AddDynamic(this, &UExcelImporter::Execute);

	AssetSavePath->OnTextChanged.AddDynamic(this, &UExcelImporter::OnAssetPathChanged);
	AssetSavePath->SetText(FText::FromString(ASSET_SAVE_PATH_PREFIX));

	ExcelPath->OnTextChanged.AddDynamic(this, &UExcelImporter::OnExcelPathChanged);
}

void UExcelImporter::FindPath()
{
	IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
	if (DesktopPlatform == nullptr)
		return;

	static const FString DefaultPath = FPaths::GetProjectFilePath();
	static const FString Filter = TEXT("excel file|*.xlsx;*.xls");
	TArray<FString> OutFiles;
	if (!DesktopPlatform->OpenFileDialog(nullptr, TEXT("Select excel file"), DefaultPath,
		TEXT(""), Filter, EFileDialogFlags::None, OutFiles))
		return;

	if (OutFiles.Num() <= 0)
		return;

	const FText text = FText::FromString(OutFiles[0]);
	ExcelPath->SetText(text);
	// manually broadcast the text change event since SetText would not trigger it.
	ExcelPath->OnTextChanged.Broadcast(text);
}

void UExcelImporter::Execute()
{
	if (!IsExcelPathValid)
	{
		ResultDisplay->SetText(FText::FromString(EXCEL_PATH_PROMPT));
		return;
	}

	LoadExcel();
}

void UExcelImporter::LoadExcel()
{
	FString DisplayMessage;

	// check asset path
	const FString asset_name = FPaths::GetBaseFilename(ExcelPath->GetText().ToString());
	const FString asset_path = AssetSavePath->GetText().ToString();
	const FString complete_asset_path = FPaths::ProjectContentDir() + asset_path.Replace(TEXT("/game/"), TEXT("")).Replace(TEXT("\\game\\"), TEXT("")) + asset_name + TEXT(".uasset");
	const bool is_file_exist = FPaths::FileExists(complete_asset_path);

	//  load or create a curve table asset
	UCurveTable* curve_table = nullptr;

	if (is_file_exist)
	{
		DisplayMessage = TEXT("There is already a file with the same name.");
		ResultDisplay->SetText(FText::FromString(DisplayMessage));
		return;
	}
	else
	{
		bool is_success = false;
		curve_table = Cast<UCurveTable>(UAssetCreator::CreateAsset(FPaths::Combine(asset_path, asset_name), UCurveTable::StaticClass(), nullptr, is_success, DisplayMessage));
		if (!curve_table)
		{
			DisplayMessage = TEXT("Curve table creation is failed beause of : ") + DisplayMessage;
			ResultDisplay->SetText(FText::FromString(DisplayMessage));
			return;
		}
	}
	curve_table->EmptyTable();

	TArray<FCurveInfo> curves;

	// load excel file and parse data
	libxl::Book* book = xlCreateXMLBook();
	book->setKey(LIBXL_NAME, LIBXL_KEY);
	if (!book)
	{
		return;
	}
	
	const TCHAR* excel_file = *(ExcelPath->GetText().ToString());
	if (book->load(excel_file))
	{
		LoadSheets(book, curves);
	}

	book->release();

	// update curve table data
	if (curves.Num() == 0)
	{
		DisplayMessage = TEXT("No curve loaded.");
		ResultDisplay->SetText(FText::FromString(DisplayMessage));
		return;
	}

	for (FCurveInfo& curve : curves)
	{
		FSimpleCurve& tmp_curve = curve_table->AddSimpleCurve(*curve.CurveName);
		tmp_curve = curve.Curve;
	}

	DisplayMessage = FString::Printf(TEXT("%d curves loaded."), curves.Num());
	ResultDisplay->SetText(FText::FromString(DisplayMessage));
}

void UExcelImporter::OnAssetPathChanged(const FText& Text)
{
	const FString str = Text.ToString();
	
	if (!str.StartsWith(TEXT("/Game/")) && !str.StartsWith("\\Game\\"))
	{
		// fix the format and output prompt
		AssetSavePath->SetText(FText::FromString(ASSET_SAVE_PATH_PREFIX));
		ResultDisplay->SetText(FText::FromString(ASSET_SAVE_PATH_PROMPT));
	}
}

void UExcelImporter::OnExcelPathChanged(const FText& Text)
{
	const FString excel_path = Text.ToString();

	if (FPaths::FileExists(excel_path))
	{
		const FString ext = FPaths::GetExtension(excel_path);
		if (ext.Contains(TEXT("xls")))
		{
			IsExcelPathValid = true;
			return;
		}
	}
	
	IsExcelPathValid = false;
}

void UExcelImporter::LoadSheets(libxl::Book* Book, TArray<FCurveInfo>& OutCurves)
{
	OutCurves.Empty();

	for (int i = 0; i < MAX_SHEET_NUM; ++i)
	{
		libxl::Sheet* sheet = Book->getSheet(i);
		if (!sheet)
		{
			UE_LOG(LogTemp, Warning, TEXT("sheet index %d is not existed."), i);
			break;
		}

		LoadSingleSheet(sheet, OutCurves);
	}
}

void UExcelImporter::LoadSingleSheet(libxl::Sheet* Sheet, TArray<FCurveInfo>& OutCurves)
{
	if (!Sheet)
	{
		return;
	}

	const int max_row = Sheet->lastFilledRow();
	const int max_col = Sheet->lastFilledCol();

	for (int col = 1; col < max_col; ++col)
	{
		FCurveInfo curve;
		curve.CurveName = ReadStrData(Sheet, 1, col);
		if (curve.CurveName.IsEmpty())
		{
			curve.CurveName = FString::Printf(TEXT("Curve%d"), col);
		}
		for (int row = 2; row < max_row; ++row)
		{
			curve.Curve.AddKey(ReadNumData(Sheet, row, 0), ReadNumData(Sheet, row, col));
		}

		OutCurves.Emplace(curve);
	}
}

float UExcelImporter::ReadNumData(libxl::Sheet* Sheet, int32 Row, int32 Col)
{
	if (Sheet)
	{
		// this is a trail version of libxl, it's first row can not used.
		// so reading data starts from second row.
		// make sure that do not put data at the first row!
		libxl::CellType cell_type = Sheet->cellType(Row, Col);
		if (cell_type != libxl::CELLTYPE_NUMBER)
		{
			UE_LOG(LogTemp, Warning, TEXT("sheet data [%d,%d] is not number"), Row, Col);
			return 0.f;
		}

		return (float)Sheet->readNum(Row, Col);
	}

	return 0.f;
}

FString UExcelImporter::ReadStrData(libxl::Sheet* Sheet, int32 Row, int32 Col)
{
	if (Sheet)
	{
		// this is a trail version of libxl, it's first row can not used.
		// so reading data starts from second row.
		// make sure that do not put data at the first row!
		libxl::CellType cell_type = Sheet->cellType(Row, Col);
		if (cell_type != libxl::CELLTYPE_STRING)
		{
			UE_LOG(LogTemp, Warning, TEXT("sheet data [%d,%d] is not string"), Row, Col);
			return FString();
		}

		FString str = Sheet->readStr(Row, Col);
		// if we can not read str, try to do again with rich str method.
		if (str.IsEmpty())
		{
			str = Sheet->readRichStr(Row, Col)->getText(0);
		}
		return str;
	}

	return FString();
}
UE_ENABLE_OPTIMIZATION
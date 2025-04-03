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
#include "Editor/UnrealEd/Public/FileHelpers.h"

#define LOCTEXT_NAMESPACE "CommonImporter"

#define ASSET_SAVE_PATH_PREFIX	TEXT("/Game/")
#define SHEET_DEFAULT_NUM		TEXT("0")
#define MAX_SHEET_NUM			99
#define LIBXL_NAME				L"Aleidi"
#define	LIBXL_KEY				L"windows-2b2b21050cc1e30b6ab3606ea9p1rdg1"


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
	IDesktopPlatform* desktopPlatform = FDesktopPlatformModule::Get();
	if (desktopPlatform == nullptr)
		return;

	static const FString DefaultPath = FPaths::GetProjectFilePath();
	static const FString Filter = TEXT("excel file|*.xlsx;*.xls");

	TArray<FString> outFiles;
	if (!desktopPlatform->OpenFileDialog(nullptr, TEXT("Select excel file"), DefaultPath,
		TEXT(""), Filter, EFileDialogFlags::None, outFiles))
		return;

	if (outFiles.Num() <= 0)
		return;

	const FText text = FText::FromString(outFiles[0]);

	ExcelPath->SetText(text);

	// manually broadcast the text change event since SetText would not trigger it.
	ExcelPath->OnTextChanged.Broadcast(text);
}

void UExcelImporter::Execute()
{
	if (!IsExcelPathValid)
	{
		ResultDisplay->SetText(LOCTEXT("CommonImporter.ExcelPathPrompt", "The excel file to be loaded is not found. Please input a valid path."));
		return;
	}

	LoadExcel();
}

void UExcelImporter::LoadExcel()
{
	// check asset path
	const FString assetName = FPaths::GetBaseFilename(ExcelPath->GetText().ToString());
	FString assetPath = AssetSavePath->GetText().ToString();

	CommonImporter::FormatPathStr(assetPath);

	assetPath = FPaths::ProjectContentDir() / assetPath.Replace(TEXT("/game/"), TEXT("")) / assetName + TEXT(".uasset");

	const bool bIsFileExist = FPaths::FileExists(assetPath);

	//  load or create a curve table asset
	UCurveTable* curveTable = nullptr;

	if (bIsFileExist)
	{
		EAppReturnType::Type Response = FMessageDialog::Open(
			EAppMsgType::YesNo,
			LOCTEXT("CommonImporter.OverwriteFilePrompt", "There is already a file with the same name.\nDo you want to overwrite it?")
		);

		if (Response == EAppReturnType::No)
		{
			ResultDisplay->SetText(LOCTEXT("CommonImporter.ExistFilePrompt", "There is already a file with the same name."));
			return;
		}

		// try to load object
		const FString curveTablePath = CommonImporter::ConvertToAssetPath(assetPath);

		curveTable = LoadObject<UCurveTable>(nullptr, *curveTablePath);

		if (!curveTable)
		{
			ResultDisplay->SetText(LOCTEXT("CommonImporter.LoadCurveTableFailed", "Curve table is existed but failed to load."));
			return;
		}
	}
	else
	{
		bool is_success = false;
		curveTable = Cast<UCurveTable>(UAssetCreator::CreateAsset(FPaths::Combine(assetPath, assetName), UCurveTable::StaticClass(), nullptr, is_success));
		if (!curveTable)
		{
			ResultDisplay->SetText(LOCTEXT("CommonImporter.CurveTableCreateFailPrompt", "Curve table creation is failed."));
			return;
		}
	}
	curveTable->EmptyTable();

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
		ResultDisplay->SetText(LOCTEXT("CommonImporter.NoCurveTablePrompt", "No curve loaded."));
		return;
	}

	for (FCurveInfo& curve : curves)
	{
		FSimpleCurve& tmp_curve = curveTable->AddSimpleCurve(*curve.CurveName);
		tmp_curve = curve.Curve;
	}

	// mark dirty
	UPackage* Package = curveTable->GetOutermost();
	if (Package) 
	{
		Package->MarkPackageDirty();
	}

	// reopen editor asset
	if (UAssetEditorSubsystem* AssetEditorSubsystem = GEditor->GetEditorSubsystem<UAssetEditorSubsystem>())
	{
		if (AssetEditorSubsystem->FindEditorForAsset(curveTable, false))
		{
			AssetEditorSubsystem->CloseAllEditorsForAsset(curveTable);
			AssetEditorSubsystem->OpenEditorForAsset(curveTable);
		}
	}

	ResultDisplay->SetText(FText::Format(LOCTEXT("CommonImporter.LoadSucceedPrompt", "{0} curves loaded"), FText::AsNumber(curves.Num())));
}

void UExcelImporter::OnAssetPathChanged(const FText& Text)
{
	FString str = Text.ToString();

	CommonImporter::FormatPathStr(str);

	if (!str.StartsWith(TEXT("/Game/")))
	{
		// fix the format and output prompt
		AssetSavePath->SetText(FText::FromString(ASSET_SAVE_PATH_PREFIX));
		ResultDisplay->SetText(LOCTEXT("CommonImporter.AssetSavePathPrompt", "The asset path must start with /Game/. Please ensure your input begins with this prefix."));
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
			UE_LOG(LogCommonImporter, Warning, TEXT("sheet index %d is not existed."), i);
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
			UE_LOG(LogCommonImporter, Warning, TEXT("sheet data [%d,%d] is not number"), Row, Col);
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
			UE_LOG(LogCommonImporter, Warning, TEXT("sheet data [%d,%d] is not string"), Row, Col);
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

#undef LOCTEXT_NAMESPACE
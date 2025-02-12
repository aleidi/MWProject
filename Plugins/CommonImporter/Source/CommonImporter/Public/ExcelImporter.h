// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "libxl.h"
#include "ExcelImporter.generated.h"

class UButton;
class UEditableText;
class UTextBlock;

/* UExcelImporter
 *  
 * This tool is used to load excel file into UE as a curve table.
 * The excel file must save data in the structure as follows:
 *		col1	col2	col3	...		colx
 * row1 ---------------------------------- Do not put any data in this row since it will be skipped or you can put title here
 * row2 KeyCol	Curve1	Curve2	...		Curvex
 * row3	Key1	data1	data1	...		data1
 * row4	Key2	data2	data2	...		data2
 * .	 .		 .		 .		...		 .
 * .	 .		 .		 .		...		 .
 * .	 .		 .		 .		...		 .
 * rowx KeyX	datax	datax	...		datax
 * 
 */
UCLASS(Abstract)
class COMMONIMPORTER_API UExcelImporter : public UEditorUtilityWidget
{
	GENERATED_BODY()

private:
	struct FCurveInfo
	{
		FString CurveName;
		FSimpleCurve Curve;
	};

protected:
	UExcelImporter();

	void NativePreConstruct() override;
	void NativeConstruct() override;

	UFUNCTION()
	void FindPath();

	UFUNCTION()
	void Execute();

	void LoadExcel();

	UFUNCTION()
	void OnAssetPathChanged(const FText& Text);

	UFUNCTION()
	void OnExcelPathChanged(const FText& Text);

	/* Load sheets, max 99 sheets are supported
	*  out LoadNum : the number of loaded sheets
	*/
	void LoadSheets(libxl::Book* Book, TArray<FCurveInfo>& OutCurves);

	void LoadSingleSheet(libxl::Sheet* Sheet, TArray<FCurveInfo>& OutCurves);

	float ReadNumData(libxl::Sheet* Sheet, int32 Row, int32 Col);

	FString ReadStrData(libxl::Sheet* Sheet, int32 Row, int32 Col);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEditableText> ExcelPath;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEditableText> AssetSavePath;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> BtnFindExcelFiles;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> BtnExecute;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ResultDisplay;

	UPROPERTY()
	bool IsExcelPathValid;
};

"""
============================================================
Table (.xlsx / .xls / .csv) → JSON Batch Export Tool
============================================================

[Core Rules]
1. Each file:
   - First column field name = Key
   - The entire table uses only this column as the Key

2. Export failure conditions:
   - First column field name is empty
   - Key value is empty / NaN
   - Duplicate Key

3. Table structure conventions:
   - Row 1: Field names
   - Row 2: Field types
   - Row 3: Comments (ignored)
   - Row 4+: Data

4. Supported formats:
   - .xlsx
   - .xls
   - .csv

5. Output:
   - Same-name .json file
   - Output to the same directory as the source file
============================================================
"""

import os
import json
import configparser
import pandas as pd


# ------------------------------------------------------------
# Read table (based on file extension)
# ------------------------------------------------------------
def read_table(file_path):
    ext = os.path.splitext(file_path)[1].lower()

    if ext == ".xlsx":
        return pd.read_excel(file_path, engine="openpyxl", header=0)

    if ext == ".xls":
        return pd.read_excel(file_path, engine="xlrd", header=0)

    if ext == ".csv":
        # utf-8-sig for compatibility with Excel-exported CSV (with BOM)
        return pd.read_csv(file_path, header=0, encoding="utf-8-sig")

    raise ValueError(f"Unsupported file type: {ext}")


# ------------------------------------------------------------
# Parse cell value
# ------------------------------------------------------------
def parse_value(value, value_type):
    if pd.isna(value):
        return None

    value_type = value_type.strip()

    if value_type == "int":
        return int(value)

    if value_type == "float":
        return float(value)

    if value_type == "bool":
        return str(value).lower() in ("true", "1")

    if value_type == "string":
        return str(value)

    # Array types: int[] / float[] / string[]
    if value_type.endswith("[]"):
        base = value_type[:-2]
        parts = str(value).split("|")

        if base == "int":
            return [int(p) for p in parts if p]
        if base == "float":
            return [float(p) for p in parts if p]
        if base == "string":
            return [p for p in parts if p]

    # Fallback: string
    return str(value)


# ------------------------------------------------------------
# Use first column as Key
# ------------------------------------------------------------
def choose_key_field(df, file_path):
    columns = list(df.columns)

    if not columns:
        raise ValueError(f"{file_path} has no columns")

    key_field = columns[0]

    if not key_field or str(key_field).strip() == "":
        raise ValueError(f"{file_path} first column field name is empty and cannot be used as Key")

    print(f"🔑 {os.path.basename(file_path)} using first column as Key: {key_field}")
    return key_field


# ------------------------------------------------------------
# Single file → JSON
# ------------------------------------------------------------
def convert_file_to_json(file_path):
    print(f"\n▶ Processing: {file_path}")

    df = read_table(file_path)

    key_field = choose_key_field(df, file_path)

    # Row 2: type definitions
    type_row = df.iloc[0].to_dict()

    # Skip type row + comment row
    df = df.iloc[2:]

    result = {}

    for row_index, row in df.iterrows():
        if row.isnull().all():
            continue

        item = {}

        for col, value in row.items():
            value_type = type_row.get(col, "string")
            item[col] = parse_value(value, value_type)

        key_value = item.get(key_field)

        if key_value is None or key_value == "":
            raise ValueError(
                f"{file_path} row {row_index + 1} Key is empty (field: {key_field})"
            )

        key = str(key_value)

        if key in result:
            raise ValueError(
                f"{file_path} duplicate Key: {key}"
            )

        result[key] = item

    json_path = os.path.splitext(file_path)[0] + ".json"

    with open(json_path, "w", encoding="utf-8") as f:
        json.dump(result, f, ensure_ascii=False, indent=2)

    print(f"✅ Exported: {json_path}")


# ------------------------------------------------------------
# Recursively scan directory
# ------------------------------------------------------------
def scan_and_convert(root_dir, extensions):
    for root, _, files in os.walk(root_dir):
        for file in files:
            if any(file.lower().endswith(ext) for ext in extensions):
                file_path = os.path.join(root, file)
                convert_file_to_json(file_path)


# ------------------------------------------------------------
# Main entry point
# ------------------------------------------------------------
def main():
    config = configparser.ConfigParser()
    config.read("config.ini", encoding="utf-8")

    section = config["ExcelExport"]

    source_dir = section.get("SourceDir", "./Configs")
    extensions = [
        ext.strip().lower()
        for ext in section.get("Extensions", ".xlsx,.xls,.csv").split(",")
    ]

    source_dir = os.path.abspath(source_dir)

    print("===================================")
    print(f"SourceDir  : {source_dir}")
    print(f"Extensions : {extensions}")
    print("===================================")

    if not os.path.isdir(source_dir):
        raise RuntimeError(f"Directory does not exist: {source_dir}")

    scan_and_convert(source_dir, extensions)

    print("\n🎉 All tables converted successfully.")


if __name__ == "__main__":
    main()
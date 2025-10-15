import bpy
import os

export_folder = r"D:\Download\Deviart\xenoblade\xb1\Shulk - Xenoblade Chronicles Definitive Edition\materials\models\xenoblade1\shulk"

def get_textures_used_by_material(mat):
    images = set()
    if not mat.use_nodes:
        return images
    for node in mat.node_tree.nodes:
        if node.type == 'TEX_IMAGE' and node.image is not None:
            images.add(node.image)
    return images

all_images = set()

for mat in bpy.data.materials:
    images = get_textures_used_by_material(mat)
    if images:
        print(f"Material: {mat.name}")
        for img in images:
            print(f" - Image: {img.name}  Path: {img.filepath}")
        all_images.update(images)
        
for img in all_images:
    # 只导出类型为 IMAGE 的图像
    if img.type == 'IMAGE' and img.has_data:
        # 计算图片导出路径
        filename = "_" + img.name + ".png"
        export_filepath = os.path.join(export_folder, filename)
        
        img.filepath_raw = export_filepath
        img.file_format = 'PNG'
        img.save(filepath=export_filepath, quality=90)
        print(f"Exported {img.name} to {export_filepath}")
    else:
        print(f"Skipped {img.name}, type={img.type}, has_data={img.has_data}")

print(f"\nTotal unique images used by all materials: {len(all_images)}")
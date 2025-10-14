import bpy

# 设置长度单位为厘米
bpy.context.scene.unit_settings.system = 'METRIC'
bpy.context.scene.unit_settings.length_unit = 'CENTIMETERS'

output_path = "D:/test.fbx"  # 请修改为你需要的输出路径和文件名

bpy.ops.export_scene.fbx(
    filepath=output_path,
    use_selection=False,          # 导出整个场景
    embed_textures=True,          # 开启Embed Textures
    path_mode='COPY',             # PathMode 设置为 Copy
    object_types={'ARMATURE', 'MESH'},  # Include 中选择 Armature 和 Mesh
    axis_forward='X',             # Transform 中 Forward 设置为 X Forward
    axis_up='Z',                  # Transform 中 Up 设置为 Z Up
    use_armature_deform_only=False,
    add_leaf_bones=False,         # Armature 中 Add Leaf Bones 不勾选
    mesh_smooth_type='EDGE'       # Geometry 中 Smoothing 设置为 Edge
);
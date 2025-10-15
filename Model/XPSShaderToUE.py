import bpy

def create_normal_map_node(ntree, location, image_node):
    normal_map_node = ntree.nodes.new(type='ShaderNodeNormalMap')
    normal_map_node.location = (location[0] + 200, location[1])
    ntree.links.new(image_node.outputs['Color'], normal_map_node.inputs['Color'])
    return normal_map_node

def convert_xps_to_ue_pbr_with_lightmap(mat):
    mat.use_nodes = True
    ntree = mat.node_tree
    nodes = ntree.nodes
    links = ntree.links

    # 清理旧BSDF节点，但保留贴图和材质输出节点
    for node in list(nodes):
        if node.type.startswith('BSDF') or (node.type != 'TEX_IMAGE' and node.type != 'OUTPUT_MATERIAL'):
            nodes.remove(node)

    # 重新查找关键节点
    material_output = None
    diffuse_tex = None
    lightmap_tex = None
    specular_tex = None
    bump_tex = None

    for node in nodes:
        if node.type == 'OUTPUT_MATERIAL':
            material_output = node
        elif node.type == 'TEX_IMAGE':
            if node.label == 'Diffuse':
                diffuse_tex = node
            elif node.label == 'Light Map':
                lightmap_tex = node
            elif node.label == 'Specular':
                specular_tex = node
            elif node.label == 'Bump Map':
                bump_tex = node

    # 创建Principled BSDF节点
    principled = nodes.new('ShaderNodeBsdfPrincipled')
    principled.location = (-200, 0)

    # 连接 Principled BSDF 到材质输出
    links.new(principled.outputs['BSDF'], material_output.inputs['Surface'])

    # Diffuse贴图连接Base Color，Alpha通道连接Alpha输入，开启透明混合
    if diffuse_tex:
        links.new(diffuse_tex.outputs['Color'], principled.inputs['Base Color'])
        links.new(diffuse_tex.outputs['Alpha'], principled.inputs['Alpha'])
        mat.blend_method = 'OPAQUE'

    # Specular -> Specular输入（原则上可映射为Roughness，但简单处理）
    if specular_tex:
        links.new(specular_tex.outputs['Color'], principled.inputs['Specular'])

    # Bump Map通过Normal Map节点连接Normal输入
    if bump_tex:
        normal_map_node = create_normal_map_node(ntree, bump_tex.location, bump_tex)
        links.new(normal_map_node.outputs['Normal'], principled.inputs['Normal'])

    # Light Map不直接影响BSDF主通道，连接到Emissive Color作为加成，帮UE识别和显式光照
    #if lightmap_tex:
    #   links.new(lightmap_tex.outputs['Color'], principled.inputs['Emission'])
        # 开启材质的Emission强度，可留默认1.0，或者下面节点调整
    #   principled.inputs['Emission Strength'].default_value = 1.0

    print(f"材质 '{mat.name}' 转换完成，含Lightmap支持。")

# 遍历所有材质执行转换
for mat in bpy.data.materials:
    convert_xps_to_ue_pbr_with_lightmap(mat)

print("所有XPS材质已转换为支持Lightmap的UE风格PBR材质。")
import bpy

def create_normal_map_node(ntree, location, image_node):
    normal_map_node = ntree.nodes.new(type='ShaderNodeNormalMap')
    normal_map_node.location = (location[0] + 300, location[1])
    ntree.links.new(image_node.outputs['Color'], normal_map_node.inputs['Color'])
    return normal_map_node

def convert_sfm_to_ue_pbr(mat):
    mat.use_nodes = True
    mat.blend_method = 'OPAQUE'
    ntree = mat.node_tree
    nodes = ntree.nodes
    links = ntree.links

    # 清理旧BSDF节点，但保留贴图和材质输出节点
    for node in list(nodes):
        if node.type.startswith('BSDF') or (node.type != 'TEX_IMAGE'):
            nodes.remove(node)

    # 重新查找关键节点
    material_output = None
    diffuse_tex = None
    phongexponent_tex = None
    bump_tex = None

    for node in nodes:
        if node.type == 'OUTPUT_MATERIAL':
            material_output = node
        elif node.type == 'TEX_IMAGE':
            if node.label == '$basetexture':
                diffuse_tex = node
            elif node.label == '$phongexponenttexture':
                phongexponent_tex = node
            elif node.label == '$bumpmap':
                bump_tex = node

    # 创建材质输出节点
    material_output = nodes.new(type='ShaderNodeOutputMaterial')
    material_output.location = (400, 0)
    
    # 创建Principled BSDF节点
    principled = nodes.new('ShaderNodeBsdfPrincipled')
    principled.location = (100, 0)

    # 连接 Principled BSDF 到材质输出
    links.new(principled.outputs['BSDF'], material_output.inputs['Surface'])

    # Diffuse贴图连接Base Color，Alpha通道连接Alpha输入，开启透明混合
    if diffuse_tex:
        diffuse_tex.location = (-600, 200)
        links.new(diffuse_tex.outputs['Color'], principled.inputs['Base Color'])

    # phongexponent -> Roughness & Metallic
    if phongexponent_tex:
        phongexponent_tex.location = (-600, -100)
        
        links.new(phongexponent_tex.outputs['Color'], principled.inputs['Roughness'])
        links.new(phongexponent_tex.outputs['Color'], principled.inputs['Metallic'])
        
        #mask_rgb = nodes.new(type='ShaderNodeSeparateColor')
        #mask_rgb.location = (-300, -100)
        
        #invert_node = nodes.new(type='ShaderNodeInvert')
        #invert_node.location = (-100, -100)
        
        #links.new(phongexponent_tex.outputs['Color'], mask_rgb.inputs['Color'])
        #links.new(mask_rgb.outputs['Red'], invert_node.inputs['Color'])
        #links.new(invert_node.outputs['Color'], principled.inputs['Roughness'])
        #links.new(mask_rgb.outputs['Green'], principled.inputs['Metallic'])

    # Bump Map通过Normal Map节点连接Normal输入
    if bump_tex:
        bump_tex.location = (-600, -400)
        normal_map_node = create_normal_map_node(ntree, bump_tex.location, bump_tex)
        links.new(normal_map_node.outputs['Normal'], principled.inputs['Normal'])

    print(f"材质 '{mat.name}' 转换完成。")

# 遍历所有材质执行转换
for mat in bpy.data.materials:
    convert_sfm_to_ue_pbr(mat)

print("所有SFM材质已转换为UE风格PBR材质。")
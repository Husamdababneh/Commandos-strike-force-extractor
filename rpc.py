import bpy
import os
import struct
import math
from mathutils import Matrix

class HD_PT_RPCImporterPanel(bpy.types.Panel):
    bl_label = "RPC Importer"
    bl_idname = "HD_PT_RPCImporterPanel"
    bl_space_type = 'VIEW_3D'
    bl_region_type = 'UI'
    bl_category = 'Tool'

    def draw(self, context):
        layout = self.layout
        layout.label(text="Commandos: Strike Force")
        layout.label(text="RPC Importer v0.01 01/25")
        layout.label(text="by Husam Dababneh")
        
        layout.separator()
        
        layout.prop(context.scene, "import_hierarchy")
        layout.operator("rpc.import")

class ImportRPC(bpy.types.Operator):
    bl_idname = "rpc.import"
    bl_label = "Import RPC"
    
    filepath: bpy.props.StringProperty(subtype="FILE_PATH")
    
    def execute(self, context):
        import_hierarchy = context.scene.import_hierarchy
        
        if not self.filepath:
            self.report({'ERROR'}, "No file selected")
            return {'CANCELLED'}
        
        if not self.filepath.lower().endswith('.rpc'):
            self.report({'ERROR'}, "File is not an RPC file")
            return {'CANCELLED'}
        
        with open(self.filepath, 'rb') as file:
            # Read the file identifier
            identifier = struct.unpack('<I', file.read(4))[0]
            if identifier != 16:
                self.report({'ERROR'}, "Not a valid RPC file")
                return {'CANCELLED'}
            
            file.seek(24)
            num_models = struct.unpack('<I', file.read(4))[0]
            file.seek(32, 1)
            num_objects = struct.unpack('<I', file.read(4))[0]
            
            objects = [None] * num_objects
            models = []
            skipped = [False] * num_models
            model_transform = []
            parents = []
            
            for i in range(num_objects):
                transformation = [struct.unpack('<f', file.read(4))[0] for _ in range(12)]
                # matrix = [transformation[0:3], transformation[3:6], transformation[6:9], transformation[9:12]]
                matrix = Matrix((
                    (transformation[0], transformation[1], transformation[2], 0),
                    (transformation[3], transformation[4], transformation[5], 0),
                    (transformation[6], transformation[7], transformation[8], 0),
                    (transformation[9], transformation[10], transformation[11], 1)  # Last row for homogeneous coordinates
                ))
                
                if i == 0:
                    # Fix rotation of root object
                    print(f"Fixing the rotation {i}, \n{matrix}")
                    rotationMatrix = Matrix.Rotation(math.radians(-90.0), 4, 'Y')
                    matrix = matrix @ rotationMatrix
                    print(f"Fixing the rotation {i}, \n{matrix}")
                    #pass  # Implement rotation fix if needed
                
                parent = struct.unpack('<i', file.read(4))[0]
                print(f"[{i}] Parent Idx : {parent}, {len(model_transform)}")
                if parent != -1:
                    matrix = Matrix(matrix) @ Matrix(model_transform[parent])
                    # pass  # Implement transformation multiplication if needed
                
                parents.append(parent)
                model_transform.append(matrix)
                file.read(4)  # Skip unknown data
            
            for i in range(num_objects):
                file.seek(12, 1)
                section_type = struct.unpack('<I', file.read(4))[0]
                
                while section_type != 0xFFFFFF00:
                    section_size = struct.unpack('<I', file.read(4))[0]
                    file.seek(section_size + 4, 1)
                    section_type = struct.unpack('<I', file.read(4))[0]
                
                file.seek(12, 1)
                has_name = struct.unpack('<I', file.read(4))[0]
                
                if has_name == 1:
                    id = struct.unpack('<I', file.read(4))[0]
                    name_length = struct.unpack('<I', file.read(4))[0]
                    obj_name = file.read(name_length).decode('utf-8')
                    
                    dummy_obj = bpy.data.objects.new(obj_name, None)
                    bpy.context.collection.objects.link(dummy_obj)
                    objects[i] = dummy_obj
                
                if i == 0 and import_hierarchy:
                    dummy_obj = bpy.data.objects.new("Root", None)
                    bpy.context.collection.objects.link(dummy_obj)
                    objects[i] = dummy_obj
            
            file.seek(28, 1)
            
            for m in range(num_models):
                file.read(4)
                section_size = struct.unpack('<I', file.read(4))[0]
                file.seek(16, 1)
                
                unknown1 = struct.unpack('<H', file.read(2))[0]
                has_uv = struct.unpack('<H', file.read(2))[0]
                
                if has_uv == 0:
                    file.seek(section_size - 16, 1)
                    skipped[m] = True
                    continue
                
                num_faces = struct.unpack('<I', file.read(4))[0]
                print(f"Number of faces: {num_faces}")
                num_vertices = struct.unpack('<I', file.read(4))[0]
                file.read(4)
                
                texcoords = []
                faces = []
                face_material_ids = []
                
                for i in range(num_vertices):
                    u = struct.unpack('<f', file.read(4))[0]
                    v = 1.0 - struct.unpack('<f', file.read(4))[0]
                    texcoords.append((u, v, 0))
                
                for i in range(num_faces):
                    index1 = struct.unpack('<H', file.read(2))[0] + 1
                    index2 = struct.unpack('<H', file.read(2))[0] + 1
                    face_mat = struct.unpack('<H', file.read(2))[0] + 1
                    index3 = struct.unpack('<H', file.read(2))[0] + 1
                    faces.append((index1, index3, index2))
                    face_material_ids.append(face_mat)
                
                file.seek(24, 1)
                
                vertex_array = []
                vertex_normal = []
                
                for i in range(num_vertices):
                    x = struct.unpack('<f', file.read(4))[0]
                    y = struct.unpack('<f', file.read(4))[0]
                    z = struct.unpack('<f', file.read(4))[0]
                    vertex_array.append((x, y, z))
                
                for i in range(num_vertices):
                    vnx = struct.unpack('<f', file.read(4))[0]
                    vny = struct.unpack('<f', file.read(4))[0]
                    vnz = struct.unpack('<f', file.read(4))[0]
                    vertex_normal.append((vnx, vny, vnz))
                
                file.seek(24, 1)
                
                num_texture = struct.unpack('<I', file.read(4))[0]
                file.seek(4 * num_texture, 1)
                
                for i in range(num_texture):
                    file.read(4)
                    section_size = struct.unpack('<I', file.read(4))[0]
                    file.seek(76, 1)
                    num_texlong = struct.unpack('<I', file.read(4))[0]
                    file.read(4)
                    texture_name = file.read(num_texlong).decode('utf-8')
                    file.seek(section_size - num_texlong - 80, 1)
                    
                    tex_filename = os.path.join(os.path.dirname(self.filepath), "textures", texture_name + ".dds")
                    if not os.path.exists(tex_filename):
                        tex_filename = os.path.join(os.path.dirname(self.filepath), "textures", texture_name + ".png")
                    
                    print(f'tex_filename : {tex_filename}')
                    # Assign texture to material
                    material = bpy.data.materials.new(name=texture_name)
                    material.use_nodes = True
                    bsdf = material.node_tree.nodes["Principled BSDF"]
                    tex_image = material.node_tree.nodes.new('ShaderNodeTexImage')
                    # tex_image.image = bpy.data.images.load(tex_filename)
                    material.node_tree.links.new(bsdf.inputs['Base Color'], tex_image.outputs['Color'])
                
                file.read(4)
                section_size = struct.unpack('<I', file.read(4))[0]
                file.seek(section_size + 4, 1)
                
                mesh = bpy.data.meshes.new(name="Mesh" + str(m))
                mesh.from_pydata(vertex_array, [], faces)
                obj = bpy.data.objects.new("Mesh" + str(m), mesh)
                bpy.context.collection.objects.link(obj)
                models.append(obj)
            
            for m in range(num_models):
                file.read(4)
                section_size = struct.unpack('<I', file.read(4))[0]
                file.seek(16, 1)
                info_id = struct.unpack('<I', file.read(4))[0] + 1
                model_id = struct.unpack('<I', file.read(4))[0] + 1
                file.seek(section_size - 20, 1)
                
                if not skipped[m] and model_id < len(models):
                    models[model_id].matrix_world = model_transform[info_id]
                    objects[info_id] = models[model_id]
                elif info_id >= len(objects):
                    continue
                else:
                    dummy_obj = bpy.data.objects.new("Skipped", None)
                    bpy.context.collection.objects.link(dummy_obj)
                    objects[info_id] = dummy_obj
            
            if import_hierarchy:
                for i in range(num_objects):
                    if objects[i] is None:
                        dummy_obj = bpy.data.objects.new("undefined", None)
                        bpy.context.collection.objects.link(dummy_obj)
                        objects[i] = dummy_obj
                    if parents[i] != 0:
                        objects[i].parent = objects[parents[i]]
        
        return {'FINISHED'}
    
    def invoke(self, context, event):
        self.filepath = "c:/trials/Commandos-strike-force-extractor/bin/Models/Char/"
        context.window_manager.fileselect_add(self)
        return {'RUNNING_MODAL'}

def register():
    bpy.utils.register_class(HD_PT_RPCImporterPanel)
    bpy.utils.register_class(ImportRPC)
    bpy.types.Scene.import_hierarchy = bpy.props.BoolProperty(name="Import Object Hierarchy", default=True)

def unregister():
    bpy.utils.unregister_class(HD_PT_RPCImporterPanel)
    bpy.utils.unregister_class(ImportRPC)
    del bpy.types.Scene.import_hierarchy

if __name__ == "__main__":
    register()

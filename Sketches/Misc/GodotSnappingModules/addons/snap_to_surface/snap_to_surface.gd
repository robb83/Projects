@tool
extends EditorPlugin

# references:
# https://github.com/remorse107/vertexsnap/blob/master/addons/vertexsnap/plugin.gd
# https://forums.unrealengine.com/t/rotation-from-normal/11543/3

const Anchor = preload("res://addons/snap_to_surface/anchor.gd")
const MyCustomGizmoPlugin = preload("res://addons/snap_to_surface/gizmo_area3d_orientation.gd")

var gizmo_plugin = MyCustomGizmoPlugin.new()
var selected_object = null
	
func add_anchor(node, gp, n):
	var anchor = Anchor.new()
	node.add_child(anchor)
	anchor.owner = node.owner
	anchor.global_position = gp
	
	# I don't get it, but works! [1]
	var up = Vector3.MODEL_FRONT
	var rv = up.cross(n).normalized()
	if rv.length() != 0:
		var ra = acos(up.dot(n))
		var q = Quaternion(rv, ra)
		anchor.quaternion = anchor.quaternion * q
		
func get_hypotenuse_middle_point(p1, p2, p3):
	var l1 = (p2 - p1).length()
	var l2 = (p3 - p1).length()
	var l3 = (p2 - p3).length()
	
	if l1 > l2 and l1 > l2:
		return p1.lerp(p2, 0.5)
	elif l3 > l2:
		return p3.lerp(p3, 0.5)
	else:
		return p1.lerp(p3, 0.5)
	
func _forward_3d_gui_input(camera, event):
	if not (event is InputEventMouse):
		return
		
	# search root CSGShape
	var current = selected_object
	while current.get_parent() is CSGShape3D:
		current = selected_object.get_parent()
	
	#Compute camera ray into scene
	var mpos = event.position
	var origin = camera.project_ray_origin(mpos)
	var dir = camera.project_ray_normal(mpos)
	
	var meshes = current.get_meshes()
	if len(meshes) != 2:
		print("Invalid shape.")
		return
	
	var mdt = MeshDataTool.new()
	mdt.create_from_surface(meshes[1], 0)
	var closest_face = []
	
	for f in mdt.get_face_count():
		var gp1 = current.global_transform * mdt.get_vertex(mdt.get_face_vertex(f, 0))
		var gp2 = current.global_transform * mdt.get_vertex(mdt.get_face_vertex(f, 1))
		var gp3 = current.global_transform * mdt.get_vertex(mdt.get_face_vertex(f, 2))
		
		var hit = Geometry3D.ray_intersects_triangle(origin, dir, gp1, gp2, gp3 )
		if hit:
			var d = origin.distance_to(hit)
			if len(closest_face) == 0 || d < closest_face[0]:
				var n = mdt.get_face_normal(f)
				closest_face = [ d, get_hypotenuse_middle_point(gp1, gp2, gp3), f, n]
	
	if len(closest_face) > 0:
		var hit = closest_face[1]
		var f = closest_face[2]
		var normal = closest_face[3]
		if event.button_mask == MOUSE_BUTTON_MASK_LEFT:
			add_anchor(current, hit, normal)

func _handles(object):
	if object is CSGShape3D:
		if selected_object == null or object != selected_object:
			return true
	return false

func _edit(object):
	selected_object = object
	return true

func _enter_tree():
	# Initialization of the plugin goes here.
	add_node_3d_gizmo_plugin(gizmo_plugin)
	add_custom_type("Modul Anchor", "Area3D", Anchor, null)
	
func _exit_tree():
	# Clean-up of the plugin goes here.
	remove_node_3d_gizmo_plugin(gizmo_plugin)
	remove_custom_type("Modul Anchor")

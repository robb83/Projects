extends EditorNode3DGizmoPlugin

func _get_gizmo_name():
	return "Area3D Orientation"

func _init():
	create_material("main", Color(1, 1,0))

func _has_gizmo(node):
	return node is Area3D

func _redraw(gizmo):
	gizmo.clear()

	var node3d = gizmo.get_node_3d()
	print(node3d)
	print(node3d.global_position)
	print(node3d.global_position)
	var lines = PackedVector3Array()
	lines.push_back(Vector3())
	lines.push_back( node3d.quaternion * Vector3.MODEL_FRONT)
	
	gizmo.add_lines(lines, get_material("main", gizmo), false)

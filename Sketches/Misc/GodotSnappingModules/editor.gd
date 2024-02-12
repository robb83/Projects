extends Node3D

const RAY_LENGTH = 100.0

@onready var camera3d = $Camera3D

var modules = []
var current_module = -1
var current_block : PackedScene = null
var current_instance = null
var current_attached = false
var current_anchor = 0
var current_attached_from = null
var current_attached_to = null

func place_cancel():
	if current_instance:
		remove_child(current_instance)
		current_instance.queue_free()
	current_block = null
	current_instance = null
	current_attached = false
	current_anchor = 0
	current_attached_from = null
	current_attached_to = null
	
func place_module():
	if current_instance:
		current_instance.set_transparency(0)
		
	current_instance = current_block.instantiate()
	current_instance.set_transparency(0.5)
	current_instance.visible = false
	current_attached = false
	current_anchor = 0
	current_attached_from = null
	current_attached_to = null
	add_child(current_instance)
	
func change_module(n):
	if current_instance:
		remove_child(current_instance)
		current_instance.queue_free()
		
	if n > -1 and n < len(modules):
		current_module = n
		current_block = modules[n]
		current_instance = current_block.instantiate()
		current_instance.set_transparency(0.5)
		current_instance.visible = false
		current_attached = false
		current_anchor = 0
		current_attached_from = null
		current_attached_to = null
		add_child(current_instance)
	else:
		current_block = null
		current_instance = null
		current_attached = false
		current_anchor = 0
		current_attached_to = null
		current_module = -1
		
func mouse_pick():
	var mpos = get_viewport().get_mouse_position()
	var origin = camera3d.project_ray_origin(mpos)
	var end = origin + camera3d.project_ray_normal(mpos) * RAY_LENGTH
	
	var params = PhysicsRayQueryParameters3D.create(origin, end, 1)
	params.collide_with_areas = true
	params.collide_with_bodies = false
	
	var hit = get_world_3d().direct_space_state.intersect_ray(params)
	if hit.has("collider") and hit.collider and (current_instance == null || current_instance != hit.collider.get_parent()):
		return hit.collider
	return null

# Called when the node enters the scene tree for the first time.
func _ready():
	modules.push_back(preload("res://modules/module_rectangle_one_one.tscn"))
	modules.push_back(preload("res://modules/module_rectangle_one_two.tscn"))
	modules.push_back(preload("res://modules/module_rectangle_one.tscn"))
	modules.push_back(preload("res://modules/module_rectangle_omni.tscn"))
	modules.push_back(preload("res://modules/module_rectangle_long_two.tscn"))
	modules.push_back(preload("res://modules/module_rectangle_long_two_half.tscn"))
	modules.push_back(preload("res://modules/module_capsule_end.tscn"))
	modules.push_back(preload("res://modules/module_rectangle_special_1.tscn"))
	modules.push_back(preload("res://modules/module_rectangle_special_2.tscn"))
	modules.push_back(preload("res://modules/module_rectangle_special_3.tscn"))
	change_module(0)

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	
	if Input.is_action_just_pressed("module_down"):
		change_module((len(modules) + current_module - 1) % len(modules))
	
	if Input.is_action_just_pressed("module_up"):
		change_module((current_module + 1) % len(modules))
		
	if Input.is_action_just_pressed("rotate_down") and current_attached:
		var a1 = (current_attached_from.global_position - current_instance.global_position)
		current_instance.rotate(a1, PI / 2)
		
	if Input.is_action_just_pressed("rotate_up"):
		var a1 = (current_attached_from.global_position - current_instance.global_position)
		current_instance.rotate(a1, -PI / 2)
		
	if Input.is_action_just_pressed("anchor_down"):
		var anchors = get_achors(current_instance)
		current_anchor = (len(anchors) + (current_anchor - 1)) % len(anchors)
		print(current_anchor)
		
	if Input.is_action_just_pressed("anchor_up"):
		var anchors = get_achors(current_instance)
		current_anchor = (current_anchor + 1) % len(anchors)
		print(current_anchor)
		
	if Input.is_action_just_pressed("cancel"):
		place_cancel()
	
	if Input.is_action_just_pressed("confirm") and current_attached:
		place_module()
		
	if current_instance:
		var result = current_attached_to
		
		var picked = mouse_pick()
		if picked:
			result = picked
		
		var module = null
		if result != null:
			module = result.get_parent()
		else:
			return
			
		var anchors = get_achors(current_instance)
		if len(anchors) > 0 and anchors[current_anchor].get_parent() != module and (anchors[current_anchor] != current_attached_from or current_attached_to != result):
			var anchor = anchors[current_anchor]
			current_instance.global_position = Vector3()
			current_instance.global_rotation = Vector3()
			
			current_instance.global_transform = anchor.global_transform.inverse()
			var a = anchor.global_transform.basis.z
			var b = result.global_transform.basis.z
			var c = a.cross(b).normalized()
			var d = a.angle_to(b)
			if c.length() != 0:
				current_instance.global_rotate(c, d - PI)
			else:
				current_instance.global_rotate(anchor.global_transform.basis.y.normalized(), PI)
			current_instance.global_position = module.global_position + (result.global_position - module.global_position) - (anchor.global_position - current_instance.global_position)
			
			current_instance.visible = true
			current_attached = true
			current_attached_to = result
			current_attached_from = anchor

func get_achors(node):
	var open = [ node ]
	var result = []
	while len(open) > 0:
		var current = open.pop_front()
		var children = current.get_children()
		for c in children:
			if c.is_in_group("anchor"):
				result.push_back(c)
			else:
				open.push_back(c)
	return result

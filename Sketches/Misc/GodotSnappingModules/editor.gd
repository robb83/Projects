extends Node3D

const RAY_LENGTH = 100.0

@onready var camera3d = $Camera3D

var modules = []
var current_block : PackedScene = null
var current_instance = null
var current_attached = false
var current_anchor = 0

func place_cancel():
	if current_instance:
		remove_child(current_instance)
		current_instance.queue_free()
	current_block = null
	current_instance = null
	current_attached = false
	current_anchor = 0
	
func place_module():
	if current_instance:
		current_instance.set_transparency(0)
		
	current_instance = current_block.instantiate()
	current_instance.set_transparency(0.5)
	current_instance.visible = false
	current_attached = false
	current_anchor = 0
	add_child(current_instance)
	
func change_module(n):
	if current_instance:
		remove_child(current_instance)
		current_instance.queue_free()
		
	if n > -1 and n < len(modules):
		current_block = modules[n]
		current_instance = current_block.instantiate()
		current_instance.set_transparency(0.5)
		current_instance.visible = false
		current_attached = false
		current_anchor = 0
		add_child(current_instance)
	else:
		current_block = null
		current_instance = null
		current_attached = false
		current_anchor = 0
		
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
	modules.push_back(preload("res://modules/module_rectangle_omni.tscn"))
	modules.push_back(preload("res://modules/module_rectangle_long_two.tscn"))
	modules.push_back(preload("res://modules/module_rectangle_long_two_half.tscn"))
	modules.push_back(preload("res://modules/module_capsule_end.tscn"))
	modules.push_back(preload("res://modules/module_rectangle_special.tscn"))
	change_module(0)

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	
	if Input.is_action_just_pressed("num_0"):
		change_module(0)
	
	if Input.is_action_just_pressed("num_1"):
		change_module(1)
	
	if Input.is_action_just_pressed("num_2"):
		change_module(2)
		
	if Input.is_action_just_pressed("num_3"):
		change_module(3)
	
	if Input.is_action_just_pressed("num_4"):
		change_module(4)
		
	if Input.is_action_just_pressed("cancel"):
		place_cancel()
		
	if Input.is_action_just_pressed("char_a"):
		var anchors = get_achors(current_instance)
		current_anchor = (len(anchors) + (current_anchor - 1)) % len(anchors)
		print(current_anchor)
		
	if Input.is_action_just_pressed("char_d"):
		var anchors = get_achors(current_instance)
		current_anchor = (current_anchor + 1) % len(anchors)
		print(current_anchor)
	
	if Input.is_action_just_pressed("click_left") and current_attached:
		place_module()
		
	if current_instance:
		var result = mouse_pick()
		if result:
			var module = result.get_parent()
			var direction = ( result.global_position - module.global_position).normalized()
			var length = ( result.global_position - module.global_position).length()
			var anchors = get_achors(current_instance)
			if len(anchors) > 0:
				print(current_anchor)
				var anchor = anchors[current_anchor]
				var position = anchor.position
				current_instance.global_position = module.global_position + direction * ( length + anchor.position.length() )
				current_instance.look_at(result.global_position, Vector3.UP)
				current_instance.look_at(result.global_position, Vector3.FORWARD)
				current_instance.visible = true
				current_attached = true

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

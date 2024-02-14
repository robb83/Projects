extends Node3D

func _ready():
	add_to_group("is_module")

func set_transparency(value):
	$CSGCombiner3D.transparency = value
